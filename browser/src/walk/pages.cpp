#include <sstream>
#include <stack>

#include <tangle/browser/walk/pages.hpp>
#include <tangle/str.hpp>

#include "action_parser.hpp"
#include "selector_parser.hpp"

namespace tangle::browser::walk {
	namespace {
		template <typename Final>
		struct base_paths_parser
		    : base_action_parser<Final, base_selector_parser<Final>> {
			using base = base_action_parser<Final, base_selector_parser<Final>>;

			base_paths_parser(std::string_view text) : base{text} {}
		};

		struct paths_parser : base_paths_parser<paths_parser> {
			selector_action* current_action{nullptr};
			selector* current_selector{nullptr};

			paths_parser(std::string_view text)
			    : base_paths_parser<paths_parser>{text} {}

			bool set_action(std::string&& name,
			                std::vector<std::string>&& arguments) {
				if (!current_action) return false;
				*current_action = {std::move(name), std::move(arguments)};
				return true;
			}

			bool set_id(std::string_view name) {
				if (!current_selector) return false;
				current_selector->id.assign(name);
				tolower_inplace(current_selector->id);
				return true;
			}

			bool set_type_name(std::string_view name) {
				if (!current_selector) return false;
				current_selector->name.assign(name);
				tolower_inplace(current_selector->name);
				return true;
			}

			bool add_class_selector(std::string_view name,
			                        attr_match function) {
				if (!current_selector) return false;
				class_token_selector sel{{}, function};
				sel.value.assign(name);
				tolower_inplace(sel.value);
				current_selector->class_list.push_back(std::move(sel));
				return true;
			}

			bool add_attr_selector(std::string&& name,
			                       std::string&& value,
			                       attr_match function) {
				if (!current_selector) return false;
				tolower_inplace(name);
				tolower_inplace(value);
				current_selector->attr_list.push_back(
				    {std::move(name), std::move(value), function});
				return true;
			}

			bool read_matchers(std::vector<matcher>& level) {
				skip_ws();
				if (!get('{')) return false;
				while (!eof()) {
					skip_ws();
					if (peek('}')) break;

					if (peek('!')) {
						selector_action action{};

						current_action = &action;
						if (!parse_action()) return false;
						current_action = nullptr;

						level.push_back(std::move(action));
						continue;
					}

					walk::step step{};

					current_selector = &step.current;
					if (!parse_selector()) return false;
					current_selector = nullptr;
					if (!read_matchers(step.submatchers)) return false;

					level.push_back(std::move(step));
				}
				return eof() || get('}');
			}

			bool parse(std::vector<matcher>& dst) { return read_matchers(dst); }
		};

		struct pages_parser : paths_parser {
			pages::map& dst;

			pages_parser(pages::map& dst, std::string_view text)
			    : paths_parser{text}, dst{dst} {}

			bool parse() {
				skip_ws();
				while (!eof()) {
					auto name = action_token();
					if (name.empty()) return false;

					paths top_level{};
					if (!read_matchers(top_level.tracks)) return false;

					std::string page_name{};
					page_name.assign(name);
					dst[std::move(page_name)] = std::move(top_level);
					skip_ws();
				}
				return true;
			}
		};

		void print_matchers(std::ostream& out,
		                    std::vector<matcher> const& matchers,
		                    bool multiline,
		                    size_t depth);

		inline void print_matcher(std::ostream& out,
		                          matcher const& m,
		                          bool multiline,
		                          size_t depth) {
			auto* step_p = std::get_if<step>(&m);
			auto* action = std::get_if<selector_action>(&m);
			if (action) {
				out << *action;
				return;
			}

			if (!step_p) return;
			auto& step = *step_p;
			out << step.current << ' ';
			print_matchers(out, step.submatchers, multiline, depth);
		}

		void print_prefix(std::ostream& out, bool multiline, size_t depth) {
			if (!multiline) return;
			out << '\n';
			for (size_t i = 0; i < depth; ++i)
				out << "    ";
		}

		void print_matchers(std::ostream& out,
		                    std::vector<matcher> const& matchers,
		                    bool multiline,
		                    size_t depth) {
			out << '{';
			if (matchers.empty()) {
				out << '}';
				return;
			}
			if (matchers.size() == 1 || !multiline) {
				for (auto& matcher : matchers) {
					out << ' ';
					print_matcher(out, matcher, multiline, depth);
				}
				out << " }";
				return;
			}
			++depth;
			for (auto& matcher : matchers) {
				print_prefix(out, multiline, depth);
				print_matcher(out, matcher, multiline, depth);
			}
			print_prefix(out, multiline, depth - 1);
			out << '}';
		}

		struct purecall_rt_error : action_callback {
			bool on_action(selector_action const& action, cursor&) override {
				std::ostringstream os;
				os << "no action for " << action;
				throw std::runtime_error(os.str());
			}
		} through_runtime_error{};

		struct walker {
			action_callbacks const& handlers;
			action_callback* purecall = &through_runtime_error;

			bool visit(cursor& scope,
			           std::vector<matcher> const& matchers,
			           std::ostream* debug,
			           size_t depth) const;
			bool apply_actions(cursor const& scope,
			                   std::vector<matcher> const& matchers) const;
			bool apply(cursor const& scope,
			           selector_action const& action) const;
			bool lookup(cursor& scope,
			            std::vector<matcher> const& matchers,
			            std::ostream* debug,
			            size_t depth) const;
		};
	}  // namespace

	std::ostream& operator<<(std::ostream& out, paths_print const& descr) {
		print_matchers(out, descr.ref, descr.multiline, 0);
		return out;
	}

	paths paths::parse_paths(std::string_view text) {
		paths dst{};
		if (!paths_parser{text}.parse(dst.tracks)) {
			dst.tracks.clear();
		}
		return dst;
	}

	bool paths::visit(cursor& root,
	                  action_callbacks const& handlers,
	                  action_callback* on_purecall,
	                  std::ostream* debug) const {
		if (on_purecall)
			return walker{handlers, on_purecall}.visit(root, tracks, debug, 0);
		return walker{handlers}.visit(root, tracks, debug, 0);
	}

	pages pages::parse_definition(std::string_view text) {
		pages dst{};
		if (!pages_parser{dst.defs, text}.parse()) {
			dst.defs.clear();
		}
		return dst;
	}

	bool pages::visit(cursor& root,
	                  std::string const& page_id,
	                  action_callbacks const& handlers,
	                  action_callback* on_purecall,
	                  std::ostream* debug) const {
		auto it = defs.find(page_id);
		if (it == defs.end()) return false;

		return it->second.visit(root, handlers, on_purecall, debug);
	}

	bool walker::visit(cursor& scope,
	                   std::vector<matcher> const& matchers,
	                   std::ostream* debug,
	                   size_t depth) const {
		return apply_actions(scope, matchers) &&
		       lookup(scope, matchers, debug, depth);
	}

	bool walker::apply_actions(cursor const& scope,
	                           std::vector<matcher> const& matchers) const {
		for (auto& matcher : matchers) {
			if (std::holds_alternative<step>(matcher)) continue;
			auto& action = std::get<selector_action>(matcher);
			if (!apply(scope, action)) return false;
		}
		return true;
	}

	bool walker::apply(cursor const& scope,
	                   selector_action const& action) const {
		auto it = handlers.find(action.name);
		auto ptr =
		    (it == handlers.end() || !it->second) ? purecall : it->second.get();
		auto copy = scope;
		return ptr->on_action(action, copy);
	}

	bool walker::lookup(cursor& scope,
	                    std::vector<matcher> const& matchers,
	                    std::ostream* debug,
	                    size_t depth) const {
		if (!scope.eof()) ++scope;

		if (!scope.eof() && debug) {
			for (auto& matcher : matchers) {
				if (std::holds_alternative<selector_action>(matcher)) continue;
				auto& step = std::get<walk::step>(matcher);
				for (size_t i = 0; i < depth; ++i)
					*debug << "    ";
				*debug << "!!! " << step.current << '\n';
			}
		}

		while (!scope.eof()) {
			auto& element = *scope;

			bool matched = false;
			for (auto& matcher : matchers) {
				if (std::holds_alternative<selector_action>(matcher)) continue;
				auto& step = std::get<walk::step>(matcher);

				if (debug && !step.current.name.empty() &&
				    element.name.is(step.current.name)) {
					for (size_t i = 0; i < depth; ++i)
						*debug << "    ";
					*debug << "??? " << element.name.value;
					for (auto& [name, val] : element.attrs) {
						*debug << ' ' << name << "=\"" << val.value << '"';
					}
					*debug << '\n';
				}

				if (!step.current.matches(element)) continue;

				if (debug) {
					for (size_t i = 0; i < depth; ++i)
						*debug << "    ";
					*debug << ">>> " << step.current << " -> "
					       << selector::matching(element) << '\n';
				}

				auto fragment = scope.forCurrentElement();
				if (!visit(fragment, step.submatchers, debug, depth + 1))
					return false;
				matched = true;
				break;
			}

			if (!matched) ++scope;
		}

		return true;
	}
}  // namespace tangle::browser::walk
