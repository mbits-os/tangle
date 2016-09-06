/*
 * Copyright (C) 2016 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>
#include <tangle/cookie/item.h>

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;
	struct cookie_info {
		cstring header;
		item cookie;
	};

	std::ostream& operator << (std::ostream& o, const cookie_info& nfo)
	{
		return o << '`' << nfo.header << '`';
	}

	std::ostream& operator << (std::ostream& o, const time_point& pt)
	{
		return o << pt.time_since_epoch().count();
	}

	class server_cookies : public ::testing::TestWithParam<cookie_info> {
	};

	// a time point which is common for every test in this file
	auto now()
	{
		static auto when = clock::now();
		return when;
	}

	auto make_time(int year, int month, int day, int hour, int minute, int second)
	{
		return clock::from_date({ year, month, day, hour, minute, second });
	}

	TEST_P(server_cookies, parse)
	{
		auto& par = GetParam();
		auto header = par.header;
		auto& expected = par.cookie;

		auto actual = from_server("example.com", header, now());
		ASSERT_EQ(expected.name(), actual.name());
		if (actual.name().empty())
			return;

		ASSERT_EQ(expected.value(), actual.value());
		ASSERT_EQ(expected.scope().domain(), actual.scope().domain());
		ASSERT_EQ(expected.scope().path(), actual.scope().path());
		ASSERT_EQ(expected.flags(), actual.flags());
		if ((actual.flags() & flags::persistent) == flags::persistent)
			ASSERT_EQ(expected.expires(), actual.expires());
	}

	constexpr cstring operator "" _cs(const char* ptr, size_t length)
	{
		return { ptr, length };
	}

	static cookie_info flag_tests[] = {
		{ "name=value"_cs, { "name", "value", { "example.com", "/" }, flags::host_only } },
		{ "name=value; seCure"_cs, { "name", "value", { "example.com", "/" }, flags::host_only | flags::secure } },
		{ "name=value; Secure"_cs, { "name", "value", { "example.com", "/" }, flags::host_only | flags::secure } },
		{ "name=value; HttpOnly"_cs, { "name", "value", { "example.com", "/" }, flags::host_only | flags::http_only } },
		{ "name=value; Secure; HttpOnly"_cs, { "name", "value", { "example.com", "/" }, flags::host_only | flags::http_only | flags::secure } },
		{ "name=value; HttpOnly; Secure"_cs, { "name", "value", { "example.com", "/" }, flags::host_only | flags::http_only | flags::secure } },
	};

	static cookie_info scope[] = {
		{ "name=value; Path=/"_cs, { "name", "value", { "example.com", "/" }, flags::host_only } },
		{ "name=value; Path=/; Domain=example.com"_cs, { "name", "value", { "example.com", "/" } } },
		{ "name=value; Path=/; Domain=.example.com"_cs, { "name", "value", { "example.com", "/" } } },
		{ "name=value; Path=/; Domain=www.example.com"_cs, { "name", "value", { "www.example.com", "/" } } },
		{ "name=value; Domain=www.example.com"_cs, { "name", "value", { "www.example.com", "/" } } },
		{ "name=value; Path=/res/ource; Domain=example.com"_cs, { "name", "value", { "example.com", "/res/ource" } } }
	};

	static cookie_info expires[] = {
		{
			"name=value; Expires = Sat, 08 May 2021 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(2021, 5, 8, 22, 23, 1) }
		},
		{
			"name=value; Expires = Sat, 08 May 21 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(2021, 5, 8, 22, 23, 1) }
		},
		{
			"name=value; Expires = Mon, 08 May 00 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(2000, 5, 8, 22, 23, 1) }
		},
		{
			"name=value; Expires = Wed, 08 May 69 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(2069, 5, 8, 22, 23, 1) }
		},
		{
			"name=value; Expires = Thu, 08 May 80 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(1980, 5, 8, 22, 23, 1) }
		},
		{
			"name=value; Expires = Wed, 08 May 1602 22:23:01; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, make_time(1602, 5, 8, 22, 23, 1) }
		},
	};

	static cookie_info max_age[] = {
		{
			"name=value; Max-Age = 200; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, now() + 200s }
		},
		{
			"name=value; Max-Age = 0; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, now() }
		},
		{
			"name=value; Max-Age = -5; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" }, flags::persistent, now() - 5s }
		},
		{
			"name=value; Max-Age = 3x3; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" } }
		},
		{
			"name=value; Max-Age = bleh; Domain = example.com"_cs,
			{ "name", "value", { "example.com", "/" } }
		},
	};

	INSTANTIATE_TEST_CASE_P(flags, server_cookies, ::testing::ValuesIn(flag_tests));
	INSTANTIATE_TEST_CASE_P(scope, server_cookies, ::testing::ValuesIn(scope));
	INSTANTIATE_TEST_CASE_P(expires, server_cookies, ::testing::ValuesIn(expires));
	INSTANTIATE_TEST_CASE_P(max_age, server_cookies, ::testing::ValuesIn(max_age));
}
