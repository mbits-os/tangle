// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/uri.hpp>

using namespace std::literals;

namespace tangle::testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct uri_parts_info {
		std::string_view uri;
		struct exp {
			std::string_view scheme;
			std::string_view authority;
			std::string_view path;
			std::string_view query;
			std::string_view fragment;
		} expected;
	};

	inline std::string to_string(std::string_view sv) {
		return {sv.data(), sv.length()};
	}
}  // namespace tangle::testing

std::ostream& operator<<(std::ostream& o,
                         const tangle::testing::uri_parts_info& param) {
	o << "\"" << param.uri << "\" -> [" << param.expected.scheme << "]:";
	if (!param.expected.authority.empty())
		o << "//[" << param.expected.authority << "]";
	o << "[" << param.expected.path << "]";

	if (param.expected.query.empty())
		o << "?<empty>";
	else
		o << "?[" << param.expected.query.substr(1) << "]";

	if (param.expected.fragment.empty())
		o << "#<empty>";
	else
		o << "#[" << param.expected.fragment.substr(1) << "]";

	return o;
}

namespace tangle::testing {
	class uri_parts : public TestWithParam<uri_parts_info> {};

	TEST_P(uri_parts, breakup) {
		auto param = GetParam();
		auto& expected = param.expected;
		uri address{param.uri};
		ASSERT_EQ(expected.scheme, address.scheme());
		ASSERT_EQ(expected.authority, address.authority());
		ASSERT_EQ(expected.path, address.path());
		ASSERT_EQ(expected.query, address.query());
		ASSERT_EQ(expected.fragment, address.fragment());
	}

	static const uri_parts_info parts[] = {
	    {
	        "",
	        {},
	    },
	    {
	        // test from https://tools.ietf.org/html/rfc3986#section-3
	        "foo://example.com:8042/over/there?name=ferret#nose",
	        {"foo", "example.com:8042", "/over/there", "?name=ferret", "#nose"},
	    },
	    {
	        // test from https://tools.ietf.org/html/rfc3986#section-3
	        "urn:example:animal:ferret:nose",
	        {"urn", {}, "example:animal:ferret:nose"},
	    },
	    {
	        "http:relative?query",
	        {"http", {}, "relative", "?query"},
	    },
	};

	INSTANTIATE_TEST_CASE_P(samples, uri_parts, ValuesIn(parts));
}  // namespace tangle::testing
