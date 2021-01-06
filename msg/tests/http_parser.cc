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
#include <tangle/msg/http_parser.hpp>
#include <initializer_list>
#include <type_traits>
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
	struct http_request_info {
		std::initializer_list<const char*> stream;
		std::string method;
		std::string request;
		int maj, min;
		std::unordered_map<std::string_view, std::vector<const char*>> headers;

		using parser_type = http_request;
	};

	struct http_response_info {
		std::initializer_list<const char*> stream;
		int maj, min;
		int status;
		std::string reason;
		std::unordered_map<std::string_view, std::vector<const char*>> headers;

		using parser_type = http_response;
	};

	template <typename Info>
	static std::enable_if_t<
		std::is_same<Info, http_request_info>::value || std::is_same<Info, http_response_info>::value,
		std::ostream&> operator<<(std::ostream& o, const Info& nfo)
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

	template <typename Final, typename Info>
	class http_parser_base : public ::testing::TestWithParam<Info> {
	public:
		using parser_type = typename Info::parser_type;

		void append_test();
	};

	template <typename Final, typename Info>
	inline void http_parser_base<Final, Info>::append_test()
	{
		auto& par = ::testing::TestWithParam<Info>::GetParam();

		parser_type parser;

		size_t read = 0;
		parsing result = parsing::error;
		for (auto chunk : par.stream) {
			size_t length = strlen(chunk);
			std::tie(read, result) = parser.append(chunk, length);
			ASSERT_EQ(parsing::reading, result);
			ASSERT_EQ(length, read);
		}
		std::tie(read, result) = parser.append("\r\n\r\n", 4);
		ASSERT_EQ(parsing::separator, result);
		ASSERT_EQ(0, read);

		auto headers = parser.dict();

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

		ASSERT_EQ(par.maj, parser.proto().http_major);
		ASSERT_EQ(par.min, parser.proto().http_minor);

		static_cast<Final*>(this)->verify_fields(parser);
	}

	template <typename Parser>
	class http_parser_error : public ::testing::TestWithParam<const char*> {
	public:
		using parser_type = Parser;

		void error_test();
	};

	template <typename Parser>
	inline void http_parser_error<Parser>::error_test()
	{
		auto chunk = GetParam();

		parser_type parser;

		size_t read = 0;
		parsing result = parsing::error;
		size_t length = strlen(chunk);
		std::tie(read, result) = parser.append(chunk, length);
		ASSERT_EQ(parsing::error, result);
	}

	class request_parser : public http_parser_base<request_parser, http_request_info> {
	public:
		void verify_fields(parser_type& parser)
		{
			auto& par = GetParam();
			ASSERT_EQ(par.method, parser.method());
			ASSERT_EQ(par.request, parser.resource());
		}
	};

	class response_parser : public http_parser_base<response_parser, http_response_info> {
	public:
		void verify_fields(parser_type& parser)
		{
			auto& par = GetParam();
			ASSERT_EQ(par.status, parser.status());
			ASSERT_EQ(par.reason, parser.reason());
		}
	};

	class request_error : public http_parser_error<http_request> {
	};

	class response_error : public http_parser_error<http_response> {
	};

	TEST_P(request_parser, append) { append_test(); }
	TEST_P(response_parser, append) { append_test(); }
	TEST_P(request_error, test) { error_test(); }
	TEST_P(response_error, test) { error_test(); }

	http_request_info requests[] = {
		{
			{ "GET / HTTP/1.1\r\nField:value\r\nField2:value\r\n" },
			"GET", "/", 1, 1,
			{
				{ "field", { "value" } },
				{ "field2", { "value" } },
			}
		},
		{
			{ "POST /res/ource HTTP/0.99\r\n" },
			"POST", "/res/ource", 0, 99,
			{
			}
		},
		{
			{ "OPTI", "ONS * HTTP/", "1.1\r\nFie", "ld:value\r\nField2:value\r\n" },
			"OPTIONS", "*", 1, 1,
			{
				{ "field", { "value" } },
				{ "field2", { "value" } },
			}
		},
	};

	http_response_info responses[] = {
		{
			{ "HTTP/1.1 200 OK\r\nField:value\r\nField2:value\r\n" },
			1, 1, 200, "OK",
			{
				{ "field", { "value" } },
				{ "field2", { "value" } },
			}
		},
		{
			{ "HTTP/314.99 500 I don't know\r\n" },
			314, 99, 500, "I don't know",
			{
			}
		},
		{
			{ "HT", "TP/1.1 20", "0 OK\r\nField:va", "lue\r\nField2:value\r\n" },
			1, 1, 200, "OK",
			{
				{ "field", { "value" } },
				{ "field2", { "value" } },
			}
		},
	};

	const char* error_requests[] = {
		"\r\n",
		"SINGLE-WORD\r\n",
		"METHOD RESOURCE\r\n",
		"METHOD         HTTP/1.1\r\n",
		"METHOD    *     HTTP/1.\r\n",
	};

	const char* error_responses[] = {
		"\r\n",
		"SINGLE-WORD\r\n",
		"HTTP 200 OK\r\n",
		"Http/0.1 200 OK\r\n",
		"HTTP?1.1 200 OK\r\n",
		"HTTP/ 200 OK\r\n",
		"HTTP/.2 200 OK\r\n",
		"HTTP/2,2 200 OK\r\n",
		"HTTP/2.2a 200 OK\r\n",
		"HTTP/2.2 0 OK\r\n",
	};

	INSTANTIATE_TEST_CASE_P(requests, request_parser, ::testing::ValuesIn(requests));
	INSTANTIATE_TEST_CASE_P(responses, response_parser, ::testing::ValuesIn(responses));

	INSTANTIATE_TEST_CASE_P(first_line, request_error, ::testing::ValuesIn(error_requests));
	INSTANTIATE_TEST_CASE_P(first_line, response_error, ::testing::ValuesIn(error_responses));
}}}
