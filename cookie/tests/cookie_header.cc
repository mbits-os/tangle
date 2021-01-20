// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/cookie/item.hpp>

namespace {
	struct cookie_info {
		const char* header;
		std::initializer_list<std::pair<const char*, const char*>> cookies;
	};

	std::ostream& operator<<(std::ostream& o, const cookie_info& nfo) {
		return o << '`' << nfo.header << '`';
	}

	class cookie_header : public ::testing::TestWithParam<cookie_info> {};

	TEST_P(cookie_header, serialize) {
		auto& par = GetParam();
		auto expected = par.header;
		auto& cookies = par.cookies;

		std::string actual;
		for (auto& pair : cookies) {
			if (!actual.empty()) actual.append("; ");
			actual.append(pair.first);
			actual.push_back('=');
			actual.append(pair.second);
		}

		ASSERT_EQ(expected, actual);
	}

	static cookie_info client[] = {
	    {"=", {{"", ""}}},
	    {"", {}},
	    {"name=value", {{"name", "value"}}},
	    {"name=value; name=value2; name2=value3; name3=; =value4",
	     {
	         {"name", "value"},
	         {"name", "value2"},
	         {"name2", "value3"},
	         {"name3", ""},
	         {"", "value4"},
	     }},
	    {"name=value; =; name3== value4",
	     {
	         {"name", "value"},
	         {"", ""},
	         {"name3", "= value4"},
	     }},
	};

	INSTANTIATE_TEST_SUITE_P(client,
	                         cookie_header,
	                         ::testing::ValuesIn(client));
}  // namespace
