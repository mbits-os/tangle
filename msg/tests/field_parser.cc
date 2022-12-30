// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/msg/field_parser.hpp>
#include <cctype>
#include <gtest/gtest.h>

namespace tangle::msg {
	static std::ostream& operator<<(std::ostream& o, parsing p) {
		switch (p) {
			case parsing::reading:
				return o << "parsing::reading";
			case parsing::separator:
				return o << "parsing::separator";
			case parsing::error:
				return o << "parsing::error";
		}
		return o << "<" << (int)p << ">";
	};
}  // namespace tangle::msg

namespace tangle::msg::testing {
	struct header_info {
		const char* test_name;
		std::initializer_list<const char*> stream;
		std::unordered_map<std::string_view, std::vector<const char*>> headers;
		parsing expected_result{parsing::separator};
	};

	static std::ostream& operator<<(std::ostream& o, const header_info& nfo) {
		o << nfo.test_name << ": {";
		bool first = true;
		for (auto chunk : nfo.stream) {
			if (first)
				first = false;
			else
				o << ',';
			o << ' ' << '"';
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
		return o << ' ' << '}';
	}

	class field_parser : public ::testing::TestWithParam<header_info> {};

	TEST_P(field_parser, append) {
		auto& par = GetParam();

		using tangle::msg::parsing;

		tangle::msg::field_parser field_parser;

		size_t read = 0;
		parsing result = parsing::error;
		for (auto chunk : par.stream) {
			size_t length = strlen(chunk);
			std::tie(read, result) = field_parser.append(chunk, length);
			if (par.expected_result == parsing::separator)
				ASSERT_EQ(parsing::reading, result);
			else if (result != parsing::reading)
				break;
			ASSERT_EQ(length, read);
		}
		if (par.expected_result == parsing::separator) {
			std::tie(read, result) = field_parser.append("\r\n\r\n", 4);
			ASSERT_EQ(2, read);
		}
		ASSERT_EQ(par.expected_result, result);

		auto headers = field_parser.dict();

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
	}

	static header_info samples[] = {
	    {
	        "empty",
	        {""},
	        {},
	    },
	    {
	        "newline-borders",
	        {"field: value\r\nField2 :value2\r\n",
	         "Field-Three: continued\r\n value\r\n", "Field2 :value3\r\n"},
	        {
	            {"field", {"value"}},
	            {"field2", {"value2", "value3"}},
	            {"field-three", {"continued value"}},
	        },
	    },
	    {
	        "midline-borders",
	        {"fiel", "d: value\r\nField2 :value2\r\n", "Field-Three: contin",
	         "ued\r\n value \r\n", "Field2 :value3\r\n",
	         "Accept-Language :\r\n   pl-PL, \r\n \r\n en-US\r\n"},
	        {{"field", {"value"}},
	         {"field2", {"value2", "value3"}},
	         {"field-three", {"continued value"}},
	         {"accept-language", {"pl-PL, en-US"}}},
	    },
	};

	static constexpr auto error = parsing::error;

	static header_info errors[] = {
	    {
	        "mid-line \\r",
	        {"field: value\rField2 :value2\r\n",
	         "Field-Three: continued\r\n value\r\n", "Field2 :value3\r\n"},
	        {},
	        error,
	    },
	    {
	        // checks, if the resulting dictionary is actually empty
	        "mid-line \\r, deeper",
	        {"field: value\r\nField2 :value2\r\n",
	         "Field-Three: continued\r value\r\n", "Field2 :value3\r\n"},
	        {},
	        error,
	    },
	    {
	        "start with continued value",
	        {"   continued\r\nfield: value is\r\n"},
	        {},
	        error,
	    },
	    {
	        "not an actual field",
	        {"field = value\r\n"},
	        {},
	        error,
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples,
	                         field_parser,
	                         ::testing::ValuesIn(samples));
	INSTANTIATE_TEST_SUITE_P(errors, field_parser, ::testing::ValuesIn(errors));
}  // namespace tangle::msg::testing
