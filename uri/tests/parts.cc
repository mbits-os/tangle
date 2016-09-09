#include <gtest/gtest.h>
#include <tangle/uri.h>

using namespace std::literals;

namespace tangle { namespace testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct uri_parts_info {
		cstring uri;
		struct exp {
			cstring scheme;
			cstring authority;
			cstring path;
			cstring query;
			cstring fragment;
		} expected;
	};
}}

std::ostream& operator<<(std::ostream& o, const tangle::testing::uri_parts_info& param)
{
	o << "\"" << param.uri << "\" -> [" << param.expected.scheme << "]:";
	if (!param.expected.authority.empty())
		o << "//[" << param.expected.authority << "]";
	o << "[" << param.expected.path << "]";

	if (param.expected.query.empty())
		o << "?<empty>";
	else
		o << "?[" << param.expected.query.subspan(1) << "]";

	if (param.expected.fragment.empty())
		o << "#<empty>";
	else
		o << "#[" << param.expected.fragment.subspan(1) << "]";

	return o;
}

namespace tangle { namespace testing {
	class uri_parts  : public TestWithParam<uri_parts_info> { };

	TEST_P(uri_parts, breakup)
	{
		auto param = GetParam();
		auto& expected = param.expected;
		uri address { param.uri };
		ASSERT_EQ(expected.scheme, address.scheme());
		ASSERT_EQ(expected.authority, address.authority());
		ASSERT_EQ(expected.path, address.path());
		ASSERT_EQ(expected.query, address.query());
		ASSERT_EQ(expected.fragment, address.fragment());
	}

	static const uri_parts_info parts[] = {
		{
			"",
			{ }
		},
		{ // test from https://tools.ietf.org/html/rfc3986#section-3
			"foo://example.com:8042/over/there?name=ferret#nose",
			{ "foo", "example.com:8042", "/over/there", "?name=ferret", "#nose" }
		},
		{ // test from https://tools.ietf.org/html/rfc3986#section-3
			"urn:example:animal:ferret:nose",
			{ "urn", { }, "example:animal:ferret:nose" }
		},
		{
			"http:relative?query",
			{ "http", { }, "relative", "?query" }
		},
	};

	INSTANTIATE_TEST_CASE_P(samples, uri_parts, ValuesIn(parts));
}}
