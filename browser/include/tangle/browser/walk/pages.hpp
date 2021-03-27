#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include <tangle/browser/walk/cursor.hpp>
#include <tangle/browser/walk/selector.hpp>
#include <tangle/browser/walk/selector_action.hpp>

namespace tangle::browser::walk {
	struct matcher;

	struct step {
		selector current;
		std::vector<matcher> submatchers;
	};

	struct matcher : std::variant<step, selector_action> {
		using base_type = std::variant<step, selector_action>;

		using base_type::base_type;

		matcher(selector const& current,
		        std::initializer_list<matcher> const& subs)
		    : base_type{step{current, subs}} {};

		base_type& base() & { return *this; }
		base_type const& base() const& { return *this; }
		base_type&& base() && { return std::move(*this); }
	};

	struct paths {
		std::vector<matcher> tracks;
		static paths parse_paths(std::string_view text);
		paths() = default;
		paths(std::initializer_list<matcher> const& matchers)
		    : tracks{matchers} {}

		bool visit(cursor& root,
		           action_callbacks const& handlers,
		           action_callback* on_purecall = nullptr,
		           std::ostream* debug = nullptr) const;
	};

	struct paths_print {
		std::vector<matcher> const& ref;
		bool multiline{false};
		friend std::ostream& operator<<(std::ostream& out,
		                                paths_print const& descr);
	};

	struct pages {
		using map = std::unordered_map<std::string, paths>;

		map defs;
		static pages parse_definition(std::string_view text);

		bool visit(cursor& root,
		           std::string const& page_id,
		           action_callbacks const& handlers,
		           action_callback* on_purecall = nullptr,
		           std::ostream* debug = nullptr) const;
	};

}  // namespace tangle::browser::walk
