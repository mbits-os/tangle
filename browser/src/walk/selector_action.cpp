// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <iostream>

#include <tangle/browser/walk/selector_action.hpp>
#include <tangle/str.hpp>

#include "action_parser.hpp"

namespace tangle::browser::walk {
	struct action_parser : base_action_parser<action_parser> {
		selector_action& dst;

		action_parser(selector_action& dst, std::string_view text)
		    : base_action_parser<action_parser>{text}, dst{dst} {}

		bool set_action(std::string_view name_view,
		                std::vector<std::string>&& arguments) {
			std::string name{};
			name.assign(name_view);
			dst = selector_action{std::move(name), std::move(arguments)};
			return true;
		};

		bool parse() {
			if (!base_action_parser<action_parser>::parse_action())
				return false;
			if (eof() || is_space(static_cast<unsigned char>(*pos)))
				return true;
			return false;
		}
	};

	std::ostream& operator<<(std::ostream& out, selector_action const& action) {
		if (action.name.empty() && action.args.empty())
			return out << "<invalid-action>";

		out << '!' << action.name;
		if (!action.args.empty()) {
			out << "[";
			bool first = true;
			for (auto& arg : action.args) {
				if (first)
					first = false;
				else
					out << ' ';
				out << arg;
			}
			out << "]";
		}
		return out;
	}

	selector_action selector_action::parse(std::string_view text) {
		selector_action result;
		if (!action_parser{result, text}.parse()) result = {};
		return result;
	}

	action_callback::~action_callback() = default;
}  // namespace tangle::browser::walk