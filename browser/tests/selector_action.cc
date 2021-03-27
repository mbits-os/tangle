// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/selector_action.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::testing {
	struct action_parse_test {
		std::string_view action{};
		selector_action expected{};
		std::string_view formatted{};

		friend std::ostream& operator<<(std::ostream& out,
		                                action_parse_test const& test) {
			return out << '"' << test.action << '"';
		}
	};

	class action_parse : public ::testing::TestWithParam<action_parse_test> {};

	TEST_P(action_parse, parse) {
		auto& param = GetParam();
		auto action = selector_action::parse(param.action);

		std::ostringstream oss;
		oss << action;
		auto actual = oss.str();

		if (param.formatted.empty()) {
			ASSERT_EQ(param.action, actual);
		} else {
			ASSERT_EQ(param.formatted, actual);
		}

		ASSERT_EQ(param.expected.name, action.name);
		ASSERT_EQ(param.expected.args.size(), action.args.size());

		auto it = action.args.begin();
		for (auto const& exp : param.expected.args) {
			auto const& act = *it++;
			ASSERT_EQ(exp, act);
		}
	}

	static action_parse_test const actions[]{
	    {
	        {},
	        {},
	        "<invalid-action>",
	    },
	    {
	        "action",
	        {},
	        "<invalid-action>",
	    },
	    {
	        "!action?trash",
	        {},
	        "<invalid-action>",
	    },
	    {
	        "!action[arg1 arg2?trash arg3]",
	        {},
	        "<invalid-action>",
	    },
	    {
	        "!action[]",
	        {"action"},
	        "!action",
	    },
	    {
	        "!action[arg]",
	        {"action", {"arg"}},
	    },
	    {
	        "!action[arg1 arg2 arg3]",
	        {"action", {"arg1", "arg2", "arg3"}},
	    },
	    {
	        "!action#name[.selector#id]",
	        {"action#name", {".selector#id"}},
	    },
	    {
	        "!action#name[arg-dash_under]",
	        {"action#name", {"arg-dash_under"}},
	    },
	    {
	        "!action#name[name/sub]",
	        {"action#name", {"name/sub"}},
	    },
	    {
	        "!action#name[pages:ref]",
	        {"action#name", {"pages:ref"}},
	    },
	    {
	        "!action#name[.selector#id arg-dash_under name/sub pages:ref]",
	        {
	            "action#name",
	            {
	                ".selector#id",
	                "arg-dash_under",
	                "name/sub",
	                "pages:ref",
	            },
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(actions,
	                         action_parse,
	                         ::testing::ValuesIn(actions));
}  // namespace tangle::browser::walk::testing
