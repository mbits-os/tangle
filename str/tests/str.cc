// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/str.hpp>
#include <gtest/gtest.h>

namespace tangle::testing {
	template <typename String>
	struct basic_split_args {
		using string_type = String;
		std::string_view sep;
		String data;
		size_t max = std::string::npos;
	};

	template <typename String>
	std::ostream& operator<<(std::ostream& out,
	                         basic_split_args<String> const& data) {
		out << "{\"" << data.sep << "\", \"" << data.data << "\"";
		if (data.max != std::string::npos) {
			out << ", " << data.max;
		}
		return out << '}';
	}

	template <typename String>
	struct basic_split_test_data {
		basic_split_args<String> args;
		std::initializer_list<std::string_view> expected;
	};

	template <typename String>
	std::ostream& operator<<(std::ostream& out,
	                         basic_split_test_data<String> const& data) {
		out << "{" << data.args << ", {";
		bool first = true;
		for (auto str : data.expected) {
			if (first)
				first = false;
			else
				out << ", ";
			out << '"' << str << '"';
		}
		return out << '}';
	}

	using string_split_test_data = basic_split_test_data<std::string>;
	using string_view_split_test_data = basic_split_test_data<std::string_view>;

	struct strip_test_data {
		std::string_view argument;
		std::string_view expected;
		friend std::ostream& operator<<(std::ostream& out,
		                                strip_test_data const& data) {
			return out << "\"" << data.argument << "\" -> \"" << data.expected
			           << "\"";
		}
	};

	template <typename String>
	struct basic_join_args {
		std::string_view sep;
		std::initializer_list<String> items;
		auto call() const { return tangle::join(sep, items); }
	};

	template <typename String>
	struct basic_join_test_data {
		basic_join_args<String> args;
		std::string_view expected;
	};

	using string_join_test_data = basic_join_test_data<std::string>;
	using string_view_join_test_data = basic_join_test_data<std::string_view>;

	struct split_s_helper {
		static auto call(std::string_view sep,
		                 std::string_view data,
		                 size_t max) {
			return split_s(sep, data, max);
		}
	};

	struct split_sv_helper {
		static auto call(std::string_view sep, std::string& data, size_t max) {
			return split_sv(sep, data, max);
		}
		static auto call(std::string_view sep,
		                 std::string_view data,
		                 size_t max) {
			return split_sv(sep, data, max);
		}
	};

	template <typename String>
	class basic_split_test
	    : public ::testing::TestWithParam<basic_split_test_data<String>> {
	protected:
		template <typename Helper>
		void test() {
			auto& param = this->GetParam();
			String arg{param.args.data};
			auto actual = Helper::call(param.args.sep, arg, param.args.max);
			ASSERT_EQ(param.expected.size(), actual.size());
			auto it = actual.begin();
			for (auto const& exp : param.expected) {
				auto const& act = *it++;
				EXPECT_EQ(exp, act);
			}
		}
		void test_sv() { test<split_sv_helper>(); }
		void test_s() { test<split_s_helper>(); }
	};
#define SPLIT(STRING)                                               \
	class split_##STRING : public basic_split_test<std::STRING> {}; \
	TEST_P(split_##STRING, split_sv) { test_sv(); }                 \
	TEST_P(split_##STRING, split_s) { test_s(); }

	SPLIT(string)
	SPLIT(string_view)

	class strip_test : public ::testing::TestWithParam<strip_test_data> {
	protected:
		template <typename Pred>
		void test(Pred pred, char const* type_name) {
			auto& param = this->GetParam();
			auto actual = pred(param.argument);
			ASSERT_EQ(param.expected, actual)
			    << "Result type is: " << type_name;
		}
	};
#define STRIP(NAME, PRED)                      \
	class strip_##NAME : public strip_test {}; \
	TEST_P(strip_##NAME, compare) {            \
		test(PRED##_sv, "string_view");        \
		test(PRED##_s, "string");              \
	}

	STRIP(both, strip)
	STRIP(left, lstrip)
	STRIP(right, rstrip)

	template <typename String>
	class basic_join_test
	    : public ::testing::TestWithParam<basic_join_test_data<String>> {
	protected:
		void test() {
			auto& param = this->GetParam();
			auto actual = param.args.call();
			ASSERT_EQ(param.expected, actual);
		}
	};
#define JOIN(STRING)                                              \
	class join_##STRING : public basic_join_test<std::STRING> {}; \
	TEST_P(join_##STRING, join) { test(); }

	JOIN(string)
	JOIN(string_view)

	// The #includes to be sure both S and SV variants get exactly the same data
	static string_split_test_data const empty_sep_s[] =
#include "str.cc.empty_sep.inc"
	    ;
	static string_view_split_test_data empty_sep_sv[] =
#include "str.cc.empty_sep.inc"
	    ;

	static string_split_test_data const splits_s[] =
#include "str.cc.splits.inc"
	    ;
	static string_view_split_test_data splits_sv[] =
#include "str.cc.splits.inc"
	    ;

	static constexpr strip_test_data left_strip[] = {
	    {"    "sv, ""sv},
	    {"   \t"sv, ""sv},
	    {"    ***"sv, "***"sv},
	    {"   \t***"sv, "***"sv},
	    {" \v   ***  \f  "sv, "***  \f  "sv},
	    {"    \t***    "sv, "***    "sv},
	    {"    **     **    "sv, "**     **    "sv},
	};

	static constexpr strip_test_data right_strip[] = {
	    {"    "sv, ""sv},
	    {"   \t"sv, ""sv},
	    {"***    "sv, "***"sv},
	    {"***\t    "sv, "***"sv},
	    {"    ***    "sv, "    ***"sv},
	    {"    ***\t\r\n    "sv, "    ***"sv},
	    {"    **     **    "sv, "    **     **"sv},
	};

	static constexpr strip_test_data full_strip[] = {
	    {"    "sv, ""sv},
	    {"   \t"sv, ""sv},
	    {"***    "sv, "***"sv},
	    {"***\t    "sv, "***"sv},
	    {"    ***    "sv, "***"sv},
	    {"    ***\t\r\n    "sv, "***"sv},
	    {"    **     **    "sv, "**     **"sv},
	};

	static string_join_test_data const joins_s[] =
#include "str.cc.joins.inc"
	    ;
	static string_view_join_test_data joins_sv[] =
#include "str.cc.joins.inc"
	    ;

	using ::testing::ValuesIn;
	INSTANTIATE_TEST_SUITE_P(empty_sep, split_string, ValuesIn(empty_sep_s));
	INSTANTIATE_TEST_SUITE_P(empty_sep,
	                         split_string_view,
	                         ValuesIn(empty_sep_sv));

	INSTANTIATE_TEST_SUITE_P(splits, split_string, ValuesIn(splits_s));
	INSTANTIATE_TEST_SUITE_P(splits, split_string_view, ValuesIn(splits_sv));

	INSTANTIATE_TEST_SUITE_P(left, strip_left, ValuesIn(left_strip));
	INSTANTIATE_TEST_SUITE_P(right, strip_right, ValuesIn(right_strip));
	INSTANTIATE_TEST_SUITE_P(both, strip_both, ValuesIn(full_strip));

	INSTANTIATE_TEST_SUITE_P(joins, join_string, ValuesIn(joins_s));
	INSTANTIATE_TEST_SUITE_P(joins, join_string_view, ValuesIn(joins_sv));
}  // namespace tangle::testing
