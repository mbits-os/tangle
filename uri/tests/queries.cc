#include <gtest/gtest.h>
#include <tangle/uri.h>

using namespace std::literals;

namespace tangle { namespace testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;


	struct query_parser {
		std::string_view query;
		std::unordered_map<std::string_view, std::vector<std::string_view>> expected;
	};

	inline std::string to_string(std::string_view sv) {
		return { sv.data(), sv.length() };
	}

	std::ostream& operator<<(std::ostream& o, const query_parser& param)
	{
		o << "\"" << param.query << "\" -> ";
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
		return o;
	}

	class query : public TestWithParam<query_parser> {};

#define ASSERT(result) GTEST_NONFATAL_FAILURE_((result).failure_message())

	TEST_P(query, breakup) {
		auto param = GetParam();
		auto& expected = param.expected;
		uri address{param.query};
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
		        {"A", {"1", "2", "3"}}
		    },
		},
		{
		    "?na%3dme=am%26persand",
		    {{"na=me", {"am&persand"}}},
		},
	};


	INSTANTIATE_TEST_CASE_P(samples, query, ValuesIn(tests));
}}
