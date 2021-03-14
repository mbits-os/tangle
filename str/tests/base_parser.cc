// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/base_parser.hpp>
#include <tangle/str.hpp>
#include <string>
#include <unordered_map>
#include <gtest/gtest.h>

namespace tangle::testing {
	using ini_section = std::unordered_map<std::string, std::string>;
	using ini = std::unordered_map<std::string, ini_section>;

	struct ini_parser : tangle::base_parser {
		using base_parser::base_parser;

		std::string current_section;
		ini result;

		bool process() {
			skip_ws();
			while (!eof()) {
				if (peek('[')) {
					get('[');
					auto name = text_until<']'>();
					if (!get(']')) return false;
					current_section.assign(strip_sv(name));
					result[current_section];
					skip_ws();

					continue;
				}

				auto name = rstrip_s(text_until<'='>());
				if (name.empty()) return false;
				if (!get('=')) return false;
				skip_ws();

				auto value = rstrip_s(text_until<'\r', '\n'>());
				result[current_section][std::move(name)] = std::move(value);
				skip_ws();
			}

			return true;
		}
	};

	struct testcase {
		std::string_view text;
		ini expected;
		bool succeeded{true};
	};

	class base_parser : public ::testing::TestWithParam<testcase> {};

	TEST_P(base_parser, parse_ini) {
		auto& param = GetParam();
		ini_parser parser{param.text};
		bool success = parser.process();
		ASSERT_EQ(param.succeeded, success);

		if (!success) return;

		for (auto const& [section_name, section] : parser.result) {
			auto it = param.expected.find(section_name);

			ASSERT_NE(param.expected.end(), it)
			    << "Missing in expected: " << section_name;

			auto const& expected = it->second;
			for (auto const& [name, value] : section) {
				auto sec_it = expected.find(name);

				ASSERT_NE(expected.end(), sec_it)
				    << "Missing in expected: " << section_name << '/' << name;

				auto const& exp_value = sec_it->second;

				ASSERT_EQ(exp_value, value);
			}

			for (auto const& [name, _] : expected) {
				auto sec_it = section.find(name);

				ASSERT_NE(section.end(), sec_it)
				    << "Missing in actual: " << section_name << '/' << name;
			}
		}

		for (auto const& [section_name, _] : param.expected) {
			auto it = parser.result.find(section_name);

			ASSERT_NE(parser.result.end(), it)
			    << "Missing in actual: " << section_name;
		}
	}

	static testcase const tests[] = {
	    {},
	    {
	        "[unclosed-section"sv,
	        {},
	        false,
	    },
	    {
	        R"([noname]
=  value
name2=value2   )"sv,
	        {},
	        false,
	    },
	    {
	        R"([noname]
name   )"sv,
	        {},
	        false,
	    },
	    {
	        "[empty-section]"sv,
	        {
	            {"empty-section"s, {}},
	        },
	    },
	    {
	        "[\t\n\r   empty-section    \t\n\r]"sv,
	        {
	            {"empty-section"s, {}},
	        },
	    },
	    {
	        R"([section]
name =  value
name2=value2   )"sv,
	        {
	            {
	                "section"s,
	                {
	                    {"name"s, "value"s},
	                    {"name2"s, "value2"s},
	                },
	            },
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(tests, base_parser, ::testing::ValuesIn(tests));
}  // namespace tangle::testing