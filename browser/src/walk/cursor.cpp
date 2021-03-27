// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/html_dtd.hpp>
#include <tangle/browser/walk/cursor.hpp>
#include <tangle/str.hpp>
#include "../tests/cxx_string.hh"
#include <iostream>
#include <stack>

namespace tangle::browser::walk {
	namespace {
		struct name_status {
			std::string end_name;
			std::string start_name{build_start_name(end_name)};
			element_info const& info{get_info(start_name)};

			explicit name_status(std::string_view name)
			    : end_name{build_end_name(name)} {};

		private:
			static std::string build_start_name(std::string_view end_name) {
				if (!end_name.empty() && end_name.front() == '/')
					end_name = end_name.substr(1);
				return {end_name.data(), end_name.size()};
			}
			static std::string build_end_name(std::string_view name) {
				std::string result{};
				result.reserve(name.length() + 1);
				if (!name.empty() && name.front() != '/') result.push_back('/');
				result.append(name);
				tolower_inplace(result);
				return result;
			}
		};

		struct name_stack : std::stack<name_status> {
			using std::stack<name_status>::stack;

			friend std::ostream& operator<<(std::ostream& out,
			                                name_stack const& stack) {
				if (stack.empty()) return out << "<empty>";
				out << '"';
				bool first = true;
				for (auto& item : stack.c) {
					if (first)
						first = false;
					else
						out << '/';
					out << item.start_name;
				}
				return out << '"';
			}
		};

		bool is_inline(elem_flag flags) {
			static element_info const inline_elems{
			    elem_flag::none,
			    {elem_flag::phrasing},
			    {elem_flag::flow_only},
			};
			return inline_elems.allows(flags, {});
		}

		struct print_on_exit {
			name_stack& stack;
			~print_on_exit() { std::cerr << stack << '\n'; }
		};

		bool remove_top(name_stack& stack, std::string_view name) {
			name_status curr{name};
			std::stack<name_status> inlines;
			// std::cerr << "--- " << stack << ", \"" << name << "\" => ";
			// print_on_exit value_of{stack};

			bool result = false;
			while (!stack.empty()) {
				auto& top_r = stack.top();
				if (top_r.start_name == curr.start_name) {
					if (!is_inline(top_r.info.flags) && !inlines.empty()) {
						std::stack<name_status> empty;
						inlines.swap(empty);
					}
					stack.pop();
					result = true;
					break;
				}
				auto top = top_r;
				stack.pop();
				if (is_inline(top.info.flags)) {
					// <b><i></b></i> is valid
					inlines.push(std::move(top));
				} else if (!inlines.empty()) {
					// <p><b><i></p> is treated as if closing both b and
					// i...
					std::stack<name_status> empty;
					inlines.swap(empty);
				}
			}

			if (stack.empty()) {
				return result;
			}
			while (!inlines.empty()) {
				stack.push(std::move(inlines.top()));
				inlines.pop();
			}
			return result;
		}

		void add_child(name_stack& stack,
		               std::string_view name,
		               bool autoclose) {
			// std::cerr << "+++ " << stack << ", \"" << name << "\" => ";
			// print_on_exit value_of{stack};

			name_status next{name};
			while (!stack.empty() &&
			       !stack.top().info.allows(next.info.flags, next.start_name)) {
				stack.pop();
			}

			if (stack.empty() || autoclose) return;
			stack.push(std::move(next));
		}

		enum class ending { unmatched, matched, unbalanced };

		ending find_closing_tag(cursor::iterator& it, cursor::iterator end) {
			if (it->name.autoclose) return ending::matched;

			name_stack stack;

			stack.push(name_status{it->name.value});
			// std::cout << "<<< " << stack << '\n';

			bool end_tag_matches = false;
			for (++it; it != end && !stack.empty(); ++it) {
				auto& element = *it;
				auto& name = element.name.value;
				if (name.empty()) continue;

				bool matching = false;
				if (name.front() == '/')
					matching = remove_top(stack, name);
				else
					add_child(stack, name, element.name.autoclose);

				if (stack.empty() && matching) end_tag_matches = true;
			}

			/*
			std::cout << "END " << stack << ' '
			          << (stack.empty() ? end_tag_matches ? "ending::matched"
			                                              : "ending::unmatched"
			                            : "ending::unbalanced")
			          << '\n';
			*/
			return stack.empty()
			           ? end_tag_matches ? ending::matched : ending::unmatched
			           : ending::unbalanced;
		}
	}  // namespace

	struct string_builder {
		std::string result{};
		bool in_spaces = false;

		void append(std::string_view part) {
			for (auto c : part) {
				auto const is_space =
				    !!std::isspace(static_cast<unsigned char>(c));
				if (is_space && (in_spaces || result.empty())) continue;
				in_spaces = is_space;
				result.push_back(is_space ? ' ' : c);
			}
		}
	};

	std::string cursor::innerText() const {
		string_builder builder{};

		size_t prev = 0;
		for (auto cur = it; cur != end; ++cur) {
			auto const position = cur->name.start - offset;
			builder.append(attr_decode(text.substr(prev, position - prev)));
			prev = cur->name.stop - offset;
		}
		builder.append(attr_decode(text.substr(prev)));

		auto result = std::move(builder.result);
		if (!result.empty()) {
			if (result.back() == ' ')
				result = result.substr(0, result.size() - 1);
		}

		return result;
	}

	struct print {
		size_t offset;
		elem const& e;
		friend std::ostream& operator<<(std::ostream& out, print const& p) {
			return out << (p.e.name.start - p.offset) << ':'
			           << (p.e.name.stop - p.offset);
		}
	};

	std::string cursor::attr(std::string_view elem,
	                         std::string_view attr_name) const {
		for (auto cur = it; cur != end; ++cur) {
			if (!cur->name.is(elem)) continue;

			for (auto& [name, attr] : cur->attrs) {
				if (equal_ignore_case(name, attr_name)) {
					auto result = attr_decode(attr.value);
					auto stripped = tangle::strip_sv(result);
					if (stripped.length() != result.length())
						result.assign(stripped);
					if (!result.empty()) return result;
				}
			}
		}

		return {};
	}

	href_text cursor::aHrefText() const {
		href_text result{};

		for (auto cur = it; cur != end; ++cur) {
			if (!cur->name.is("a"sv)) continue;

			for (auto& [name, attr] : cur->attrs) {
				if (equal_ignore_case(name, "href"sv)) {
					result.href = attr_decode(attr.value);
					auto stripped = tangle::strip_sv(result.href);
					if (stripped.length() != result.href.length())
						result.href.assign(stripped);
				}
			}

			auto copy = *this;
			copy.it += cur - it;
			copy = copy.forCurrentElement();
			result.text = copy.innerText();

			return result;
		}

		return result;
	}

	cursor cursor::forCurrentElement() {
		if (it == end) return {};

		auto const new_offset = it->name.start;
		auto ptr = it;

		auto match = find_closing_tag(it, end);

		if (ptr == it && match == ending::matched) {
			++it;

			auto section =
			    text.substr(new_offset - offset, ptr->name.stop - new_offset);
			return {{ptr, it}, section, new_offset};
		}

#if 0
		for (auto cur = ptr; cur != it; ++cur) {
			std::cout << "::: " << cur->name.start << ' ' << cur->name.stop
			          << ' ' << cur->name.value << '\n';
		}
		if (it == end) {
			std::cout << "::> end()\n";
		} else {
			std::cout << "::> " << it->name.start << ' ' << it->name.stop << ' '
			          << it->name.value << '\n';
		}
#endif

		if (match != ending::matched && (it - ptr) > 1) {
			--it;
			// std::cout << "~~> " << it->name.start << ' ' << it->name.stop <<
			// ' ' << it->name.value << '\n';
		}

		auto text_end = it;
		if (match == ending::matched && text_end != ptr) --text_end;
		auto const text_end_stop =
		    text_end == end ? text.size() + offset : text_end->name.stop;
		auto const it_start = it == end ? text.size() + offset : it->name.start;
		auto const new_end =
		    match == ending::matched ? text_end_stop : it_start;

		auto const text_length = new_end - new_offset;
		auto section = text.substr(new_offset - offset, text_length);

		// if (match == ending::unbalanced)
		// 	std::cout << "    [" << testing::cxx_string{section, 120} << "]\n";

		return {{ptr, it}, section, new_offset};
	}

	cursor cursor::innerHTML() {
		if (elements.size() < 2) {
			if (elements.empty() || elements.front().name.autoclose)
				return {{}, {}};
			auto const text_start = elements.front().name.stop - offset;
			return {{}, text.substr(text_start)};
		}
		auto open = it;

		auto const match = find_closing_tag(it, end);

#if 0
		for (auto cur = open; cur != it; ++cur) {
			std::cout << "::: " << cur->name.start << ' ' << cur->name.stop
			          << ' ' << cur->name.value << '\n';
		}
		if (it == end) {
			std::cout << "::> end()\n";
		} else {
			std::cout << "::> " << it->name.start << ' ' << it->name.stop << ' '
			          << it->name.value << '\n';
		}
#endif

		auto close = it;
		if (match != ending::unbalanced && close != open) --close;

		auto const new_offset = open->name.stop;
		auto const end_offset =
		    close == end ? (text.size() + offset) : close->name.start;

		auto const text_start = new_offset - offset;
		auto const text_length = end_offset - new_offset;

		auto section = text.substr(text_start, text_length);

		// if (match == ending::unbalanced)
		// 	std::cout << "    {" << testing::cxx_string{section, 120} << "}\n";

		return {{std::next(open), close}, section, new_offset};
	}

	cursor cursor::copyFromHere() const {
		auto ptr = it;
		auto begin = elements.data();
		if (ptr != begin) --ptr;
		if (ptr == end) return {};

		auto const new_offset = ptr->name.stop;
		auto section = text.substr(new_offset - offset);

		return {{it, end}, section, new_offset};
	}
}  // namespace tangle::browser::walk