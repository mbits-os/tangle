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
#include <tangle/msg/base_parser.h>
#include <cctype>

static std::ostream& operator<<(std::ostream& o, tangle::msg::parsing p)
{
	switch (p) {
	case tangle::msg::parsing::reading: return o << "parsing::reading";
	case tangle::msg::parsing::separator: return o << "parsing::separator";
	case tangle::msg::parsing::error: return o << "parsing::error";
	}
	return o << "<" << (int)p << ">";
};

namespace tangle { namespace msg { namespace testing {
	struct header_info {
		std::initializer_list<const char*> stream;
		std::unordered_map<std::string_view, std::vector<const char*>> headers;
	};

	static std::ostream& operator<<(std::ostream& o, const header_info& nfo)
	{
		o << '{';
		bool first = true;
		for (auto chunk : nfo.stream) {
			if (first) first = false;
			else o << ',';
			o << ' ' << '"';
			for (; *chunk; ++chunk) {
				switch (*chunk) {
				case '"': o << "\\\""; break;
				case '\r': o << "\\r"; break;
				case '\n': o << "\\n"; break;
				case '\t': o << "\\t"; break;
				case '\a': o << "\\a"; break;
				case '\b': o << "\\b"; break;
				case '\v': o << "\\v"; break;
				case '\\': o << "\\\\"; break;
				default:
					if (std::isprint((uint8_t)*chunk))
						o << *chunk;
					else
						o << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)(uint8_t)*chunk << std::dec;
				}
			}
			o << '"';
		}
		return o << ' ' << '}';
	}

	class base_parser : public ::testing::TestWithParam<header_info> {
	};

	TEST_P(base_parser, append)
	{
		auto& par = GetParam();

		using tangle::msg::parsing;

		tangle::msg::base_parser base_parser;

		size_t read = 0;
		parsing result = parsing::error;
		for (auto chunk : par.stream) {
			size_t length = strlen(chunk);
			std::tie(read, result) = base_parser.append(chunk, length);
			ASSERT_EQ(parsing::reading, result);
			ASSERT_EQ(length, read);
		}
		std::tie(read, result) = base_parser.append("\r\n\r\n", 4);
		ASSERT_EQ(parsing::separator, result);
		ASSERT_EQ(0, read);

		auto headers = base_parser.dict();

		for (auto& header : headers) {
			auto it = par.headers.find(header.first);
			auto present = par.headers.end() != it;
			ASSERT_TRUE(present) << "unexpected key in parsed headers: " << header.first;
			ASSERT_EQ(header.second.size(), it->second.size());
			auto val = std::begin(header.second);
			for (auto& lhs : it->second) {
				auto& rhs = *val++;
				ASSERT_EQ(lhs, rhs);
			}
		}

		for (auto& header : par.headers) {
			auto it = headers.find(header.first);
			auto present = headers.end() != it;
			ASSERT_TRUE(present) << "missing key in parsed headers: " << header.first;
			ASSERT_EQ(header.second.size(), it->second.size());
			auto val = std::begin(header.second);
			for (auto& lhs : it->second) {
				auto& rhs = *val++;
				ASSERT_EQ(lhs, rhs);
			}
		}
	}

	static header_info samples[] = {
		{ { "" }, { } },
		{
			{ "field: value\r\nField2 :value2\r\n", "Field-Three: continued\r\n value\r\n", "Field2 :value3\r\n" },
			{
				{ "field", { "value" } },
				{ "field2", { "value2", "value3" } },
				{ "field-three", { "continued value" } },
			}
		},
		{
			{ "fiel", "d: value\r\nField2 :value2\r\n", "Field-Three: contin", "ued\r\n value \r\n", "Field2 :value3\r\n", "Accept-Language :\r\n   pl-PL, \r\n \r\n en-US\r\n" },
			{
				{ "field", { "value" } },
				{ "field2", { "value2", "value3" } },
				{ "field-three", { "continued value" } },
				{ "accept-language", { "pl-PL, en-US" } }
			}
		},
	};

	INSTANTIATE_TEST_CASE_P(samples, base_parser, ::testing::ValuesIn(samples));
}}}
