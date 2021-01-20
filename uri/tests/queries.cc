// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/uri.hpp>

using namespace std::literals;

namespace tangle::testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct query_parser {
		std::string_view query;
		std::unordered_map<std::string_view, std::vector<std::string_view>>
		    expected;
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

	TEST_P(query, breakup) {
		auto param = GetParam();
		auto& expected = param.expected;
		auto const params = uri::params::parse(param.query);
		for (auto const& [name, values] : param.expected) {
			auto it = params.values().find({name.data(), name.size()});
			if (it == params.values().end()) {
				auto error = ::testing::AssertionFailure();
				error << "Cannot find " << name << " in parsed query";
				ASSERT(error);
				continue;
			}
			auto vals_it = it->second.begin();
			auto vals_end = it->second.end();
			auto exp_it = values.begin();
			auto exp_end = values.end();
			for (; vals_it != vals_end && exp_it != exp_end;
			     ++vals_it, ++exp_it) {
				auto const& actual = *vals_it;
				auto const& exp = *exp_it;
				EXPECT_EQ(actual, exp);
			}
			if (exp_it != exp_end) {
				auto error = ::testing::AssertionFailure();
				error << "Missing values for " << name;
				char sep = ':';
				for (; exp_it != exp_end; ++exp_it) {
					error << sep << ' ' << *exp_it;
					sep = ',';
				}
				ASSERT(error);
			}
			if (vals_it != vals_end) {
				auto error = ::testing::AssertionFailure();
				error << "Unexpected values for " << name;
				char sep = ':';
				for (; vals_it != vals_end; ++vals_it) {
					error << sep << ' ' << *vals_it;
					sep = ',';
				}
				ASSERT(error);
			}
		}
		for (auto const& [name, values] : params.values()) {
			auto it = param.expected.find({name.data(), name.size()});
			if (it == param.expected.end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name << " in parsed query");
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

		for (auto const& [name, values] : verbose.values()) {
			auto it = from_uri.values().find(name);
			if (it == from_uri.values().end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name << " in params::parse");
			}
		}

		for (auto const& [name, values] : from_uri.values()) {
			auto it = verbose.values().find(name);
			if (it == verbose.values().end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name
				       << " in uri::parsed_query");
			}
		}

		for (auto const& [name, values] : from_uri.values()) {
			auto verbose_it = verbose.values().find(name);
			if (verbose_it == verbose.values().end()) continue;

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
		auto& expected = param.expected;
		uri address{};
		address.query(uri::params::parse(param.query).string());
		std::cerr << "QUERY: " << address.query() << '\n';
		auto const params = address.parsed_query();
		for (auto const& [name, values] : param.expected) {
			auto it = params.values().find({name.data(), name.size()});
			if (it == params.values().end()) {
				auto error = ::testing::AssertionFailure();
				error << "Cannot find " << name << " in parsed query";
				ASSERT(error);
				continue;
			}
			auto vals_it = it->second.begin();
			auto vals_end = it->second.end();
			auto exp_it = values.begin();
			auto exp_end = values.end();
			for (; vals_it != vals_end && exp_it != exp_end;
			     ++vals_it, ++exp_it) {
				auto const& actual = *vals_it;
				auto const& exp = *exp_it;
				EXPECT_EQ(actual, exp);
			}
			if (exp_it != exp_end) {
				auto error = ::testing::AssertionFailure();
				error << "Missing values for " << name;
				char sep = ':';
				for (; exp_it != exp_end; ++exp_it) {
					error << sep << ' ' << *exp_it;
					sep = ',';
				}
				ASSERT(error);
			}
			if (vals_it != vals_end) {
				auto error = ::testing::AssertionFailure();
				error << "Unexpected values for " << name;
				char sep = ':';
				for (; vals_it != vals_end; ++vals_it) {
					error << sep << ' ' << *vals_it;
					sep = ',';
				}
				ASSERT(error);
			}
		}
		for (auto const& [name, values] : params.values()) {
			auto it = param.expected.find({name.data(), name.size()});
			if (it == param.expected.end()) {
				ASSERT(::testing::AssertionFailure()
				       << "Unexpected param " << name << " in parsed query");
			}
		}
	}

	TEST_P(query, list) {
		auto param = GetParam();
		auto& expected = param.expected;
		uri address{};
		address.query(param.query);
		EXPECT_EQ(param.query, address.query());
		auto const params = address.parsed_query();

		auto parsed_list = params.list();
		std::sort(parsed_list.begin(), parsed_list.end());

		auto test_list = uri::params::list_type{};
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
			EXPECT_EQ(left.second, right.second);
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
	        "?na%3dme=am%26persand",
	        {
	            {"na=me", {"am&persand"}},
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples, query, ValuesIn(tests));
}  // namespace tangle::testing
