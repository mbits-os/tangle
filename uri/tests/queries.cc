// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/uri.hpp>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct query_parser {
		std::string_view query;
		std::vector<std::pair<std::string_view, std::vector<std::string_view>>>
		    expected;
		std::string_view reversed{};
	};

	inline std::string to_string(std::string_view sv) {
		return {sv.data(), sv.length()};
	}

	std::ostream& operator<<(std::ostream& o, const query_parser& param) {
		o << "\"" << param.query << "\" -> (";
		for (auto& [name, values] : param.expected) {
			o << name << " -> ";
			if (values.size() == 1) {
				o << values.front() << "; ";
				continue;
			}
			char sep = '[';
			for (auto& value : values) {
				o << sep << ' ' << value;
				sep = ',';
			}
			o << "]; ";
		}
		return o << ")";
	}

	class query : public TestWithParam<query_parser> {};

#define ASSERT(result) GTEST_NONFATAL_FAILURE_((result).failure_message())

	TEST_F(query, remove) {
		uri::params params{};
		params.add("name", "value");
		params.add("remove", "yes");
		params.add("name2", "value");
		ASSERT_EQ("?name=value&remove=yes&name2=value", params.string());
		params.remove("nonexistent");
		ASSERT_EQ("?name=value&remove=yes&name2=value", params.string());
		params.remove("remove");
		// also, make sure indexes are still working
		params.add("name2", "value2");
		ASSERT_EQ("?name=value&name2=value&name2=value2", params.string());
	}

	TEST_P(query, breakup) {
		auto param = GetParam();
		auto& expected = param.expected;
		auto const params = uri::params::parse(param.query);

		ASSERT_EQ(params.size(), param.expected.size());
		auto it = params.begin();
		size_t index = 0;
		for (auto const& [exp_name, exp_values] : param.expected) {
			auto const& [act_name, act_values] = *it++;
			EXPECT_EQ(exp_name, act_name) << "Param at: " << index;
			ASSERT_EQ(exp_values.size(), act_values.size())
			    << "Param at: " << index;
			++index;

			auto val_it = act_values.begin();
			size_t val_index = 0;
			for (auto const& exp_value : exp_values) {
				auto const& act_value = *val_it++;
				ASSERT_EQ(exp_value, act_value)
				    << "Param at: " << index << "\nValue at: " << val_index;
				++val_index;
			}
		}
	}

	TEST_P(query, same) {
		auto param = GetParam();
		auto& expected = param.expected;
		uri address{};
		address.query(param.query);
		EXPECT_EQ(param.query, address.query());
		auto const verbose = uri::params::parse(address.query());
		auto const from_uri = address.parsed_query();

		for (auto const& [name, values] : verbose) {
			auto it = from_uri.find(name);
			if (it == from_uri.end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name << " in params::parse");
			}
		}

		for (auto const& [name, values] : from_uri) {
			auto it = verbose.find(name);
			if (it == verbose.end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name
				       << " in uri::parsed_query");
			}
		}

		for (auto const& [name, values] : from_uri) {
			auto verbose_it = verbose.find(name);
			if (verbose_it == verbose.end()) continue;

			auto v_it = verbose_it->second.begin();
			auto v_it_end = verbose_it->second.end();

			auto u_it = values.begin();
			auto u_it_end = values.end();

			for (; v_it != v_it_end && u_it != u_it_end; ++v_it, ++u_it) {
				auto const& left = *v_it;
				auto const& right = *u_it;
				EXPECT_EQ(left, right);
			}
		}
	}

	TEST_P(query, reverse) {
		auto param = GetParam();
		auto expected = param.reversed.empty() ? param.query : param.reversed;
		uri address{};
		auto actual = uri::params::parse(param.query).string();
		if (expected.empty() || expected.front() != '?')
			actual = actual.substr(1);
		ASSERT_EQ(expected, actual);
	}

	struct opt {
		std::optional<std::string_view> data;
		friend std::ostream& operator<<(std::ostream& out, opt const& value) {
			if (!value.data) return out << "std::nullopt";
			return out << '"' << *value.data << '"';
		}
	};

	TEST_P(query, list) {
		auto param = GetParam();
		auto& expected = param.expected;
		uri address{};
		address.query(param.query);
		EXPECT_EQ(param.query, address.query());
		auto const params = address.parsed_query();

		auto parsed_list = params.flat_list();
		std::sort(parsed_list.begin(), parsed_list.end());

		auto test_list = uri::params::flat_list_type{};
		for (auto const& [name, values] : expected) {
			auto const sname = std::string{name.data(), name.size()};
			if (values.empty()) {
				test_list.emplace_back(sname, std::nullopt);
				continue;
			}
			for (auto const& value : values) {
				auto const svalue = std::string{value.data(), value.size()};
				test_list.emplace_back(sname, svalue);
			}
		}
		std::sort(test_list.begin(), test_list.end());

		auto l_it = test_list.begin();
		auto l_it_end = test_list.end();

		auto r_it = parsed_list.begin();
		auto r_it_end = parsed_list.end();

		for (; l_it != l_it_end && r_it != r_it_end; ++l_it, ++r_it) {
			auto const& left = *l_it;
			auto const& right = *r_it;
			EXPECT_EQ(left.first, right.first);
			EXPECT_EQ(left.second, right.second)
			    << "Optionals:\n  expected:\n    " << opt{left.second}
			    << "\n  actual:\n    " << opt{right.second} << '\n';
		}
	}

	static const query_parser tests[]{
	    {
	        "A=a&b=B",
	        {
	            {"A", {"a"}},
	            {"b", {"B"}},
	        },
	    },
	    {
	        "?A=a&b=B",
	        {
	            {"A", {"a"}},
	            {"b", {"B"}},
	        },
	    },
	    {
	        "A=1&A=2&A=3&B=a&B=b",
	        {
	            {"A", {"1", "2", "3"}},
	            {"B", {"a", "b"}},
	        },
	    },
	    {
	        "?A=1&A=2&A=3",
	        {
	            {"A", {"1", "2", "3"}},
	        },
	    },
	    {
	        "?A=1&A=2&A=3&B",
	        {
	            {"A", {"1", "2", "3"}},
	            {"B", {}},
	        },
	    },
	    {
	        // params::set over pre-existing name
	        "?A=1&A=2&A=3&A",
	        {
	            {"A", {}},
	        },
	        "?A",
	    },
	    {
	        "?na%3dme=am%26persand",
	        {
	            {"na=me", {"am&persand"}},
	        },
	        "?na%3Dme=am%26persand",
	    },
	    {
	        "?name=value+with+spaces",
	        {
	            {"name", {"value with spaces"}},
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples, query, ValuesIn(tests));
}  // namespace tangle::testing
