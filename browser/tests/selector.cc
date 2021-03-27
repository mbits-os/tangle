// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/selector.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::testing {
	struct selector_parse_test {
		std::string_view selector{};
		std::string_view expected{};

		friend std::ostream& operator<<(std::ostream& out,
		                                selector_parse_test const& test) {
			return out << test.selector << " -> " << test.expected;
		}
	};

	enum class should : bool { match = true, fail = false };

	struct selector_match_test {
		std::string_view selector{};
		std::string_view tag{};
		should should_match{should::fail};

		friend std::ostream& operator<<(std::ostream& out,
		                                selector_match_test const& test) {
			out << '"' << test.selector << '"';
			if (test.should_match == should::match)
				out << " == ";
			else
				out << " != ";
			out << '"' << test.tag << '"';
			return out;
		}
	};

	struct selector_matching_test {
		std::string_view tag{};
		std::string_view expected{};

		friend std::ostream& operator<<(std::ostream& out,
		                                selector_matching_test const& test) {
			return out << '"' << test.tag << "\" -> \"" << test.expected << '"';
		}
	};

	class selector_parse
	    : public ::testing::TestWithParam<selector_parse_test> {};

	TEST(selector_parse, print_bad_selector) {
		selector sel{
		    "name",
		    "id",
		    {{"class1", attr_match::exactly}},
		    {
		        {"name1", {}, attr_match::exists},
		        {"name2", {}, attr_match::unused},
		        {"name3", "value3", attr_match::exactly},
		        {"name4", {}, attr_match::ends_with},
		    },
		};

		std::ostringstream oss;
		oss << sel;
		auto actual = oss.str();

		ASSERT_EQ("name#id.class1[name1][name3=value3]", actual);
	}

	TEST_P(selector_parse, parse) {
		auto& param = GetParam();
		auto actual_sel = selector::parse(param.selector);

		std::ostringstream oss;
		oss << actual_sel;
		auto actual = oss.str();

		ASSERT_EQ(param.expected, actual);
	}

	static constexpr selector_parse_test selectors[]{
	    {""sv, ""sv},
	    {"elem"sv, "elem"sv},
	    {".class"sv, ".class"sv},
	    {"#identifier"sv, "#identifier"sv},
	    {"[class~=value]"sv, ".value"sv},
	    {"[class|=value]"sv, "[class|=value]"sv},
	    {"[attr~=value]"sv, "[attr~=value]"sv},
	    {"[attr|=value]"sv, "[attr|=value]"sv},
	    {"[attr^=value]"sv, "[attr^=value]"sv},
	    {"[attr$=value]"sv, "[attr$=value]"sv},
	    {"[attr^=value]"sv, "[attr^=value]"sv},
	    {"[attr*=value]"sv, "[attr*=value]"sv},
	    {
	        "elem[attr1][attr2=value].className2"sv,
	        "elem.classname2[attr1][attr2=value]"sv,
	    },
	    {"elem.class#ident"sv, "elem#ident.class"sv},
	};

	static constexpr selector_parse_test failed[]{
	    {"elem!error"sv}, {".(){}"sv},  {"#..."sv},    {"[]"sv},
	    {"[attr"sv},      {"[attr="sv}, {"[attr=]"sv}, {""sv}};

	static constexpr selector_parse_test enhancements[]{
	    {".|name"sv, ".|name"sv},
	    {".^name"sv, ".^name"sv},
	    {".$name"sv, ".$name"sv},
	    {".*name"sv, ".*name"sv},
	};

	INSTANTIATE_TEST_SUITE_P(selectors,
	                         selector_parse,
	                         ::testing::ValuesIn(selectors));

	INSTANTIATE_TEST_SUITE_P(failed,
	                         selector_parse,
	                         ::testing::ValuesIn(failed));

	INSTANTIATE_TEST_SUITE_P(enhancements,
	                         selector_parse,
	                         ::testing::ValuesIn(enhancements));

	class selector_match
	    : public ::testing::TestWithParam<selector_match_test> {};

	TEST(selector_match, class_unused) {
		auto sel = selector{
		    {},
		    {},
		    {{"class-name", attr_match::unused}},
		};

		auto tags = html_split("<e class=class-name>");
		ASSERT_EQ(1, tags.size());

		ASSERT_FALSE(sel.matches(tags.front()));
	}

	TEST(selector_match, class_exists) {
		auto sel = selector{
		    {},
		    {},
		    {{"class-name", attr_match::exists}},
		};

		auto tags = html_split("<e class=\"first class-name last\">");
		ASSERT_EQ(1, tags.size());

		ASSERT_TRUE(sel.matches(tags.front()));
	}

	TEST(selector_match, class_exactly) {
		auto sel = selector{
		    {},
		    {},
		    {{"class-name", attr_match::exactly}},
		};

		auto tags = html_split("<e class=\"first class-name last\">");
		ASSERT_EQ(1, tags.size());

		ASSERT_TRUE(sel.matches(tags.front()));
	}

	TEST(selector_match, class_contains_token) {
		auto sel = selector{
		    {},
		    {},
		    {{"class-name", attr_match::contains_token}},
		};

		auto tags = html_split("<e class=\"first class-name last\">");
		ASSERT_EQ(1, tags.size());

		ASSERT_TRUE(sel.matches(tags.front()));
	}

	TEST(selector_match, bad_attr) {
		auto sel = selector{
		    {},
		    {},
		    {},
		    {{"data", "class-name", attr_match::unused}},
		};

		auto tags = html_split("<e data=class-name>");
		ASSERT_EQ(1, tags.size());

		ASSERT_FALSE(sel.matches(tags.front()));
	}

	TEST_P(selector_match, parse) {
		auto& param = GetParam();
		auto sel = selector::parse(param.selector);

		auto should_match = param.should_match == should::match;

		auto tags = html_split(param.tag);
		ASSERT_EQ(1, tags.size());

		auto matched = sel.matches(tags.front());
		ASSERT_EQ(should_match, matched);
	}

	static constexpr selector_match_test matches[]{
	    {"img", "<img src='' class='name' />", should::match},
	    {"img.name", "<img src='' class='name' />", should::match},
	    {"img.name2", "<img src='' class='name' />", should::fail},

	    {"img", "<div src='' class='name' />", should::fail},

	    {"div#id1", "<div id='id1' />", should::match},
	    {"div#id1", "<div id='id2' />", should::fail},

	    {"[attr=value]", "<elem attr=value />", should::match},
	    {"[attr=value]", "<elem id=value />", should::fail},
	    {"[attr=value]", "<elem attr=value2 />", should::fail},

	    {"[attr]", "<elem attr=value />", should::match},
	    {"[attr]", "<elem id=value />", should::fail},

	    {"[attr~=value]", "<elem attr='value2 value value3' />", should::match},
	    {"[attr~=value]", "<elem attr='value1 value2 value3' />", should::fail},

	    {"[attr$=A]", "<elem attr='prefixA' />", should::match},
	    {"[attr$=A]", "<elem attr='Asuffix' />", should::fail},

	    {"[attr^=abc]", "<elem attr='ABCsuffix' />", should::match},
	    {"[attr^=ABC]", "<elem attr='ABCsuffix' />", should::match},
	    {"[attr^=ABC]", "<elem attr='prefixABC' />", should::fail},

	    {"[attr|=abc]", "<elem attr='abc' />", should::match},
	    {"[attr|=abc]", "<elem attr='abc-dash-suffix' />", should::match},
	    {"[attr|=abc]", "<elem attr='abcdash-suffix' />", should::fail},

	    {"[attr*=abc]", "<elem attr='abc' />", should::match},
	    {"[attr*=dash]", "<elem attr='abc-dash-suffix' />", should::match},
	    {"[attr*=dash]", "<elem attr='abcdash-suffix' />", should::match},
	    {"[attr*=value]", "<elem attr='value1 value2' />", should::match},
	    {"[attr*=value]", "<elem attr='attr1 attr2' />", should::fail},
	};

	static constexpr selector_match_test enh_matches[]{
	    {"e.|middle", "<e class='first middle last' />", should::match},
	    {"e.|middle", "<e class='first middle-suffix last' />", should::match},
	    {"e.|middle", "<e class='first middle_suffix last' />", should::fail},
	    {"e.|middle", "<e class='middle last' />", should::match},
	    {"e.|middle", "<e class='middle-suffix last' />", should::match},
	    {"e.|middle", "<e class='middle_suffix last' />", should::fail},
	    {"e.|middle", "<e class='first middle' />", should::match},
	    {"e.|middle", "<e class='first middle-suffix' />", should::match},
	    {"e.|middle", "<e class='first middle_suffix' />", should::fail},

	    {"e.^middle", "<e class='first middle last' />", should::match},
	    {"e.^middle", "<e class='first middle-suffix last' />", should::match},
	    {"e.^middle", "<e class='first middle_suffix last' />", should::match},
	    {"e.^middle", "<e class='first prefix_middle last' />", should::fail},
	    {"e.^middle", "<e class='middle last' />", should::match},
	    {"e.^middle", "<e class='middle-suffix last' />", should::match},
	    {"e.^middle", "<e class='middle_suffix last' />", should::match},
	    {"e.^middle", "<e class='prefix_middle last' />", should::fail},
	    {"e.^middle", "<e class='first middle' />", should::match},
	    {"e.^middle", "<e class='first middle-suffix' />", should::match},
	    {"e.^middle", "<e class='first middle_suffix' />", should::match},
	    {"e.^middle", "<e class='first prefix_middle' />", should::fail},

	    {"e.$middle", "<e class='first middle last' />", should::match},
	    {"e.$middle", "<e class='first prefix_middle last' />", should::match},
	    {"e.$middle", "<e class='first middle_suffix last' />", should::fail},
	    {"e.$middle", "<e class='middle last' />", should::match},
	    {"e.$middle", "<e class='prefix_middle last' />", should::match},
	    {"e.$middle", "<e class='middle_suffix last' />", should::fail},
	    {"e.$middle", "<e class='first middle' />", should::match},
	    {"e.$middle", "<e class='first prefix_middle' />", should::match},
	    {"e.$middle", "<e class='first middle_suffix' />", should::fail},

	    {"e.*middle", "<e class='first middle last' />", should::match},
	    {"e.*middle", "<e class='middle last' />", should::match},
	    {"e.*middle", "<e class='first middle' />", should::match},
	    {"e.*middle", "<e class='first last' />", should::fail},
	    {"e.*middle", "<e class='first prefix_middle last' />", should::match},
	    {"e.*middle", "<e class='first middle_suffix last' />", should::match},
	    {"e.*middle", "<e class='prefix_middle_suffix' />", should::match},
	    {"e.*middle", "<e class='MIDDLE' />", should::match},
	};

	INSTANTIATE_TEST_SUITE_P(matches,
	                         selector_match,
	                         ::testing::ValuesIn(matches));

	INSTANTIATE_TEST_SUITE_P(enhancements,
	                         selector_match,
	                         ::testing::ValuesIn(enh_matches));

	class selector_matching
	    : public ::testing::TestWithParam<selector_matching_test> {};

	TEST_P(selector_matching, elem) {
		auto& param = GetParam();

		auto tags = html_split(param.tag);
		ASSERT_EQ(1, tags.size());

		auto actual_sel = selector::matching(tags.front());

		std::ostringstream oss;
		oss << actual_sel;
		auto actual = oss.str();

		ASSERT_EQ(param.expected, actual);
	}

	static constexpr selector_matching_test elements[]{
	    {"<e>", "e"},
	    {"<e attr=value>", "e"},
	    {"<e id=name>", "e#name"},
	    {"<e id=name1 id=name2>", "e#name2"},
	    {"<e class=\"one three six\">", "e.one.three.six"},
	    {"<e class=\"one three six\" id=name>", "e#name.one.three.six"},
	};

	INSTANTIATE_TEST_SUITE_P(elements,
	                         selector_matching,
	                         ::testing::ValuesIn(elements));
}  // namespace tangle::browser::walk::testing
