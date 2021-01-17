// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <cctype>
#include <initializer_list>
#include <tangle/msg/http_parser.hpp>
#include <type_traits>

static std::ostream& operator<<(std::ostream& o, tangle::msg::parsing p) {
	switch (p) {
		case tangle::msg::parsing::reading:
			return o << "parsing::reading";
		case tangle::msg::parsing::separator:
			return o << "parsing::separator";
		case tangle::msg::parsing::error:
			return o << "parsing::error";
	}
	return o << "<" << (int)p << ">";
};

namespace tangle::msg::testing {
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

	void print_chunk(std::ostream& o, char const* chunk) {
		o << '"';
		for (; *chunk; ++chunk) {
			switch (*chunk) {
				case '"':
					o << "\\\"";
					break;
				case '\r':
					o << "\\r";
					break;
				case '\n':
					o << "\\n";
					break;
				case '\t':
					o << "\\t";
					break;
				case '\a':
					o << "\\a";
					break;
				case '\b':
					o << "\\b";
					break;
				case '\v':
					o << "\\v";
					break;
				case '\\':
					o << "\\\\";
					break;
				default:
					if (std::isprint((uint8_t)*chunk))
						o << *chunk;
					else
						o << "0x" << std::hex << std::setw(2)
						  << std::setfill('0') << (int)(uint8_t)*chunk
						  << std::dec;
			}
		}
		o << '"';
	}

	struct chunko {
		char const* chunk;
		friend std::ostream& operator<<(std::ostream& o,
		                                const chunko& wrapper) {
			print_chunk(o, wrapper.chunk);
			return o;
		}
	};

	template <typename Info>
	static std::enable_if_t<std::is_same<Info, http_request_info>::value ||
	                            std::is_same<Info, http_response_info>::value,
	                        std::ostream&>
	operator<<(std::ostream& o, const Info& nfo) {
		o << '{';
		bool first = true;
		for (auto chunk : nfo.stream) {
			if (first)
				first = false;
			else
				o << ',';
			o << ' ' << chunko{chunk};
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
	inline void http_parser_base<Final, Info>::append_test() {
		auto& par = ::testing::TestWithParam<Info>::GetParam();

		parser_type parser;

		size_t read = 0;
		parsing result = parsing::error;
		for (auto chunk : par.stream) {
			size_t length = strlen(chunk);
			std::tie(read, result) = parser.append(chunk, length);
			ASSERT_NE(parsing::error, result);
			ASSERT_EQ(length, read);
		}
		std::tie(read, result) = parser.append("\r\n", 2);
		ASSERT_EQ(parsing::separator, result);
		ASSERT_EQ(2, read);

		auto headers = parser.dict();

		for (auto& header : headers) {
			auto it = par.headers.find(header.first);
			auto present = par.headers.end() != it;
			ASSERT_TRUE(present)
			    << "unexpected key in parsed headers: " << header.first.view();
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
			ASSERT_TRUE(present)
			    << "missing key in parsed headers: " << header.first;
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
	class http_parser_error
	    : public ::testing::TestWithParam<std::initializer_list<char const*>> {
	public:
		using parser_type = Parser;

		void error_test();
	};

	template <typename Parser>
	inline void http_parser_error<Parser>::error_test() {
		auto chunks = GetParam();

		parser_type parser;

		auto const top = chunks.size() ? chunks.size() - 1 : 0;
		size_t index = 0;

		for (auto chunk : chunks) {
			auto const expected =
			    index == top ? parsing::error : parsing::reading;
			++index;
			size_t read = 0;
			parsing result = expected;
			size_t length = strlen(chunk);
			std::tie(read, result) = parser.append(chunk, length);
			ASSERT_EQ(expected, result) << "Chunk: " << chunko{chunk};
		}
	}

	class request_parser
	    : public http_parser_base<request_parser, http_request_info> {
	public:
		void verify_fields(parser_type& parser) {
			auto& par = GetParam();
			ASSERT_EQ(par.method, parser.method());
			ASSERT_EQ(par.request, parser.resource());
		}
	};

	class response_parser
	    : public http_parser_base<response_parser, http_response_info> {
	public:
		void verify_fields(parser_type& parser) {
			auto& par = GetParam();
			ASSERT_EQ(par.status, parser.status());
			ASSERT_EQ(par.reason, parser.reason());
		}
	};

	class request_error : public http_parser_error<http_request> {};

	class response_error : public http_parser_error<http_response> {};

	TEST_P(request_parser, append) { append_test(); }
	TEST_P(response_parser, append) { append_test(); }
	TEST_P(request_error, test) { error_test(); }
	TEST_P(response_error, test) { error_test(); }

	http_request_info requests[] = {
	    {
	        {
	            "GET / HTTP/1.1\r\nField:value\r\nField2:value\r\n",
	        },
	        "GET",
	        "/",
	        1,
	        1,
	        {
	            {"field", {"value"}},
	            {"field2", {"value"}},
	        },
	    },
	    {
	        {
	            "GET / HTTP/1.1\r\nField:value\r\nField2:value\r\n\r\n",
	            "GET /res HTTP/1.1\r\nField3:value\r\nField4:value\r\n\r\n",
	            "GET /ource HTTP/1.1\r\nField5:value\r\nField6:value\r\n\r\n",
	            "GET /res/ource HTTP/1.0\r\nField7:value\r\nField8:value\r\n",
	        },
	        "GET",
	        "/res/ource",
	        1,
	        0,
	        {
	            {"field7", {"value"}},
	            {"field8", {"value"}},
	        },
	    },
	    {
	        {"POST /res/ource HTTP/0.99\r", "", "\n"},
	        "POST",
	        "/res/ource",
	        0,
	        99,
	        {},
	    },
	    {
	        {"POST /res/ource             HTTP/0.99\r\n"},
	        "POST",
	        "/res/ource",
	        0,
	        99,
	        {},
	    },
	    {
	        {"OPTI", "ONS * HTTP/", "1.1\r\nFie",
	         "ld:value\r\nField2:value\r\n"},
	        "OPTIONS",
	        "*",
	        1,
	        1,
	        {
	            {"field", {"value"}},
	            {"field2", {"value"}},
	        },
	    },
	};

	http_response_info responses[] = {
	    {
	        {"HTTP/1.1 200 OK\r\nField:value\r\nField2:value\r\n"},
	        1,
	        1,
	        200,
	        "OK",
	        {
	            {"field", {"value"}},
	            {"field2", {"value"}},
	        },
	    },
	    {
	        {"HTTP/1.1 200 OK\r", "", "\nField:value\r\nField2:value\r\n"},
	        1,
	        1,
	        200,
	        "OK",
	        {
	            {"field", {"value"}},
	            {"field2", {"value"}},
	        },
	    },
	    {
	        {"HTTP/314.99 500 I don't know\r\n"},
	        314,
	        99,
	        500,
	        "I don't know",
	        {},
	    },
	    {
	        {"HT", "TP/1.1 20", "0 OK\r\nField:va", "lue\r\nField2:value\r\n"},
	        1,
	        1,
	        200,
	        "OK",
	        {
	            {"field", {"value"}},
	            {"field2", {"value"}},
	        },
	    },
	    {
	        {"HT", "TP/1.1 20", "0 OK\r\nField:va", "lue\r\nField2:value\r\n",
	         "\r\n", "HTTP/314.99 500 I don't know\r\n"},
	        314,
	        99,
	        500,
	        "I don't know",
	        {},
	    },
	};

	std::initializer_list<char const*> error_requests[] = {
	    {"\r\n"},
	    {"SINGLE-WORD\r\n"},
	    {"METHOD RESOURCE\r\n"},
	    {"METHOD         HTTP/1.1\r\n"},
	    {"METHOD    *     HTTP/1.\r\n"},
	    {"GET\t/\tHTTP/1.0\r\n"},
	    {"GET / HTTP/1.0\r", "<error-here>"},
	};

	std::initializer_list<char const*> error_responses[] = {
	    {"\r\n"},
	    {"SINGLE-WORD\r\n"},
	    {"HTTP 200 OK\r\n"},
	    {"Http/0.1 200 OK\r\n"},
	    {"HTTP?1.1 200 OK\r\n"},
	    {"HTTP/ 200 OK\r\n"},
	    {"HTTP/.2 200 OK\r\n"},
	    {"HTTP/2,2 200 OK\r\n"},
	    {"HTTP/2.2a 200 OK\r\n"},
	    {"HTTP/2.2 0 OK\r\n"},
	    {"HTTP/1.0 200 OK\r", "<error-here>"},
	};

	INSTANTIATE_TEST_CASE_P(requests,
	                        request_parser,
	                        ::testing::ValuesIn(requests));
	INSTANTIATE_TEST_CASE_P(responses,
	                        response_parser,
	                        ::testing::ValuesIn(responses));

	INSTANTIATE_TEST_CASE_P(first_line,
	                        request_error,
	                        ::testing::ValuesIn(error_requests));
	INSTANTIATE_TEST_CASE_P(first_line,
	                        response_error,
	                        ::testing::ValuesIn(error_responses));
}  // namespace tangle::msg::testing
