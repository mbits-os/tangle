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
	using namespace net::cookie;

	struct load_info {
		size_t length;
		const uint8_t* content;
		bool valid;
		const net::cookie::jar* expected;
	};

	class cookie_jar_load : public ::testing::TestWithParam<load_info> {
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

	TEST_P(cookie_jar_load, parse)
	{
		auto& par = GetParam();
		auto& expected = *par.expected;
		io::data data { par.length, 0, par.content };
		net::cookie::jar actual;

		ASSERT_EQ(par.valid, actual.load_raw(&data, now()));
		if (!par.valid)
			return;

		ASSERT_EQ(expected.size(), actual.size());

		auto pact = std::begin(actual);
		for (auto& ex : expected) {
			auto& act = *pact;
			++pact;

			ASSERT_EQ(ex.name(), act.name());
			ASSERT_EQ(ex.value(), act.value());
			ASSERT_EQ(ex.scope().domain(), act.scope().domain());
			ASSERT_EQ(ex.scope().path(), act.scope().path());
			ASSERT_EQ(ex.flags(), act.flags());
			ASSERT_EQ(ex.expires(), act.expires());
			ASSERT_EQ(ex.creation_time(), act.creation_time());
			ASSERT_EQ(ex.last_access_time(), act.last_access_time());
		}
	}

	static constexpr uint8_t HELLO[] = { 0x48, 0x45, 0x4c, 0x4f };

	static constexpr uint8_t CJAR_2_0[] =
	{
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x02, 0x00,
	};

	static constexpr uint8_t CJAR_1_0_loose[] =
	{
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x01, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x30, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, // PAD

		0x01, 0x00, 0x00, 0x00,
		0x07, 0x00, 0x00, 0x00,
		0x0d, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x09, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, // PAD

		0x1b, 0x00, 0x00, 0x00,
		0x21, 0x00, 0x00, 0x00,
		0x28, 0x00, 0x00, 0x00,
		0x38, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, // PAD

		0x00, 0x00, 0x00, 0x00, // PAD
		0x00, 0x00, 0x00, 0x00, // PAD
		0x00, 0x00, 0x00, 0x00, // PAD

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

	static constexpr uint8_t CJAR_1_0_compress[] = {
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

		0x01, 0x00, 0x00, 0x00,
		0x1b, 0x00, 0x00, 0x00,
		0x22, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x32, 0x00, 0x00, 0x00,
		0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
		0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
		0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00
	};

	static constexpr uint8_t CJAR_1_1_minor[] = {
		0x43, 0x4a, 0x41, 0x52,
		0x01, 0x00, 0x01, 0x00,
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

		0x01, 0x00, 0x00, 0x00,
		0x1b, 0x00, 0x00, 0x00,
		0x22, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x32, 0x00, 0x00, 0x00,
		0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
		0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
		0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00
	};

	static constexpr uint8_t CJAR_1_0_expired[] = {
		0x43, 0x4a, 0x41, 0x52,
		0x00, 0x00, 0x01, 0x00,
		0x03, 0x00, 0x00, 0x00,
		0x2c, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,
		0x94, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x07, 0x00, 0x00, 0x00,
		0x0d, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x09, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x1b, 0x00, 0x00, 0x00,
		0x22, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xc8, 0x97, 0x97, 0xaa, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x01, 0x00, 0x00, 0x00,
		0x1b, 0x00, 0x00, 0x00,
		0x22, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,
		0xc8, 0x71, 0x5b, 0x0d, 0x03, 0x00, 0x00, 0x00,

		0x32, 0x00, 0x00, 0x00,
		0x00,
		0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
		0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
		0x2f, 0x00,
		0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
		0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00
	};

	using net::cookie::flags;
	static const jar tested {
		{ "name2", "value", { "example.com", "/" }, flags::http_only | flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) },
		{ "name2", "value2", { "www.example.com", "/" }, flags::persistent, make_time(2100, 1, 1, 12, 30, 00), make_time(2016, 5, 29, 12, 30, 00) }
	};

	static const load_info experiments[] = {
		{ 0, { }, false },
		{ sizeof(HELLO), HELLO, false },
		{ sizeof(CJAR_2_0), CJAR_2_0, false },
		{ sizeof(CJAR_1_0_loose), CJAR_1_0_loose, true, &tested },
		{ sizeof(CJAR_1_1_minor), CJAR_1_1_minor, true, &tested },
		{ sizeof(CJAR_1_0_expired), CJAR_1_0_expired, true, &tested },
	};

	INSTANTIATE_TEST_CASE_P(experiments, cookie_jar_load, ::testing::ValuesIn(experiments));
}
