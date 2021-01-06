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
#include "testing_io.h"

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	struct session_info {
		const jar before;
		const item cookie;
		bool from_http;
		const jar after;

		size_t length;
		const uint8_t* content;
	};

	std::ostream& operator << (std::ostream& o, const session_info& nfo)
	{
		return o << nfo.cookie.server_string();
	}

	class cookie_jar_add : public ::testing::TestWithParam<session_info> {
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

	TEST_P(cookie_jar_add, modify)
	{
		auto& par = GetParam();
		auto actual = par.before;
		auto& expected = par.after;

		actual.add(par.cookie, par.from_http);
		ASSERT_EQ(expected.size(), actual.size());
		auto pex = std::begin(expected);
		for (auto& act : actual) {
			auto& ex = *pex;
			++pex;
			ASSERT_EQ(ex.name(), act.name());
			ASSERT_EQ(ex.value(), act.value());
			ASSERT_EQ(ex.scope().domain, act.scope().domain);
			ASSERT_EQ(ex.scope().path, act.scope().path);
			ASSERT_EQ(ex.flags(), act.flags());
			ASSERT_EQ(ex.expires(), act.expires());
			ASSERT_EQ(ex.creation_time(), act.creation_time());
			ASSERT_EQ(ex.last_access_time(), act.last_access_time());
		}
	}

	TEST_P(cookie_jar_add, store)
	{
		auto& par = GetParam();
		auto& jar = par.after;
		std::vector<uint8_t> actual;

		ASSERT_TRUE(jar.store_raw(actual, now()));
		ASSERT_EQ(par.length, actual.size());
		auto eptr = par.content;
		for (auto achar : actual) {
			auto echar = *eptr;
			ASSERT_EQ(echar, achar) << "Offset: " << (eptr - par.content);
			++eptr;
		}
	}

	TEST_P(cookie_jar_add, load)
	{
		auto& par = GetParam();
		auto& jar = par.after;
		io::data data { par.length, 0, par.content };
		tangle::cookie::jar actual;

		ASSERT_TRUE(actual.load_raw(&data, now()));

		size_t expected_length = 0;
		for (auto& ex : jar) {
			if ((ex.flags() & flags::persistent) == flags::persistent && (ex.expires() > now()))
				++expected_length;
		}

		ASSERT_EQ(expected_length, actual.size());

		auto pact = std::begin(actual);
		for (auto& ex : jar) {
			if ((ex.flags() & flags::persistent) != flags::persistent || (ex.expires() <= now()))
				continue;

			auto& act = *pact;
			++pact;

			ASSERT_EQ(ex.name(), act.name());
			ASSERT_EQ(ex.value(), act.value());
			ASSERT_EQ(ex.scope().domain, act.scope().domain);
			ASSERT_EQ(ex.scope().path, act.scope().path);
			ASSERT_EQ(ex.flags(), act.flags());
			ASSERT_EQ(ex.expires(), act.expires());
			ASSERT_EQ(ex.creation_time(), act.creation_time());
			ASSERT_EQ(ex.last_access_time(), act.last_access_time());
		}
	}

	static constexpr uint8_t empty[] = {
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x2c, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x00
	};

	static constexpr uint8_t name2first[] = {
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x01, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0x2c, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,
		0x3c, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x07, 0x00, 0x00, 0x00,
		0x0d, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x09, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x1b, 0x00, 0x00, 0x00,
		0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
		0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00,
	};

	static constexpr uint8_t both_name2[] = {
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x01, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x2c, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,
		0x68, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x07, 0x00, 0x00, 0x00,
		0x0d, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x09, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x1b, 0x00, 0x00, 0x00,
		0x21, 0x00, 0x00, 0x00,
		0x28, 0x00, 0x00, 0x00,
		0x38, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x3a, 0x00, 0x00, 0x00,
		0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
		0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
		0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00
	};

	static const session_info session[] = {
		{
			{},
			{ "name", "value", { "example.com", "/" }, flags::host_only }, true,
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only }
			},
			sizeof(empty), empty
		},
		{
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "expired", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(1601, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }, true,
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "expired", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(1601, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) },
				{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			sizeof(name2first), name2first
		},
		{
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			{ "name2", "value2", { "example.com", "/" } }, false,
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			sizeof(name2first), name2first
		},
		{
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "name2", "value", { "example.com", "/" }, flags::http_only }
			},
			{ "name", "value2", { "example.com", "/" } }, true,
			{
				{ "name", "value2", { "example.com", "/" } },
				{ "name2", "value", { "example.com", "/" }, flags::http_only }
			},
			sizeof(empty), empty
		},
		{
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			{ "name2", "value2", { "www.example.com", "/" }, flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }, true,
			{
				{ "name", "value", { "example.com", "/" }, flags::host_only },
				{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) },
				{ "name2", "value2", { "www.example.com", "/" }, flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
			},
			sizeof(both_name2), both_name2
		},
	};

	INSTANTIATE_TEST_CASE_P(session, cookie_jar_add, ::testing::ValuesIn(session));
}
