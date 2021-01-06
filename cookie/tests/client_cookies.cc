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

	class cookies : public ::testing::TestWithParam<cookie_info> {};

	TEST_P(cookies, parse) {
		auto& par = GetParam();
		auto header = par.header;
		auto& expected = par.cookies;

		auto actual = tangle::cookie::from_client("example.com", header);
		ASSERT_EQ(expected.size(), actual.size());
		auto cur = std::begin(actual);
		for (auto& ex : expected) {
			auto ex_name = ex.first;
			auto ex_value = ex.second;
			auto& act = *cur++;

			ASSERT_EQ(ex_name, act.name());
			ASSERT_EQ(ex_value, act.value());
		}
	}

	static cookie_info client[] = {
	    {"=", {{"", ""}}},
	    {"", {}},
	    {"name=value", {{"name", "value"}}},
	    {"  name  = value   ", {{"name", "value"}}},
	    {"name = value; name = value2; name2 = value3; name3 = ; = value4",
	     {
	         {"name", "value"},
	         {"name", "value2"},
	         {"name2", "value3"},
	         {"name3", ""},
	         {"", "value4"},
	     }},
	    {"name = value; = ; name2 and value3; name3 = = value4",
	     {
	         {"name", "value"},
	         {"", ""},
	         {"name3", "= value4"},
	     }},
	};

	INSTANTIATE_TEST_CASE_P(client, cookies, ::testing::ValuesIn(client));
}  // namespace
