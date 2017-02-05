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
	struct cookie_info {
		const char* header;
		std::initializer_list<std::pair<const char*, const char*>> cookies;
	};

	std::ostream& operator << (std::ostream& o, const cookie_info& nfo)
	{
		return o << '`' << nfo.header << '`';
	}

	class cookie_header : public ::testing::TestWithParam<cookie_info> {
	};

	TEST_P(cookie_header, serialize)
	{
		auto& par = GetParam();
		auto expected = par.header;
		auto& cookies = par.cookies;

		std::string actual;
		for (auto& pair : cookies) {
			if (!actual.empty())
				actual.append("; ");
			actual.append(pair.first);
			actual.push_back('=');
			actual.append(pair.second);
		}

		ASSERT_EQ(expected, actual);
	}

	static cookie_info client[] = {
		{ "=", { { "", "" } } },
		{ "", { } },
		{ "name=value", { { "name", "value" } } },
		{ "name=value; name=value2; name2=value3; name3=; =value4", { { "name", "value" }, { "name", "value2" }, { "name2", "value3" }, { "name3", "" }, { "", "value4" }, } },
		{ "name=value; =; name3== value4", { { "name", "value" }, { "", "" }, { "name3", "= value4" }, } },
	};

	INSTANTIATE_TEST_CASE_P(client, cookie_header, ::testing::ValuesIn(client));
}