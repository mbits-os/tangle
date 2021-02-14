// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "testing_io.hpp"
#include <gtest/gtest.h>

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	struct store_info {
		io::data_span expected;
		const jar* cookies;
	};

	class cookie_jar_store : public ::testing::TestWithParam<store_info> {};

	// a time point which is common for every test in this file
	auto now() {
		static auto when = clock::now();
		return when;
	}

	auto make_time(int year,
	               int month,
	               int day,
	               int hour,
	               int minute,
	               int second) {
		return clock::from_date({year, month, day, hour, minute, second});
	}

	static void dbghex(const char* prefix,
	                   const void* str,
	                   size_t count,
	                   std::ostream& out) {
		constexpr size_t length = 32;
		static constexpr char alphabet[] = "0123456789abcdef";
		char line[length * 4 + 2];
		memset(line, ' ', sizeof(line));
		size_t ch = 0;
		auto chars = static_cast<char const*>(str);
		for (size_t j = 0; j < count; ++j) {
			auto c = *chars++;
			line[ch * 3] = alphabet[(c >> 4) & 0xF];
			line[ch * 3 + 1] = alphabet[(c)&0xF];
			line[ch * 3 + 2] = ' ';
			line[length * 3 + ch] =
			    std::isprint(static_cast<unsigned char>(c)) ? c : '.';

			++ch;
			if (ch == length) {
				ch = 0;
				line[length * 4] = '\n';
				line[length * 4 + 1] = 0;
				out << prefix << line;
			}
		}

		if (ch != 0) {
			for (size_t i = ch; i < length; ++i) {
				line[i * 3] = line[i * 3 + 1] = line[i * 3 + 2] =
				    line[length * 3 + i] = ' ';
			}

			line[length * 4] = '\n';
			line[length * 4 + 1] = 0;
			out << prefix << line;
		}
	}

	struct printable {
		io::data_span expected;
		friend std::ostream& operator<<(std::ostream& out,
		                                printable const& prn) {
			dbghex("  ", prn.expected.bytes, prn.expected.length, out);
			return out;
		}
	};

	TEST_P(cookie_jar_store, partial_store) {
		auto const then = clock::time_point{clock::duration::zero()};
		auto& par = GetParam();
		auto& expected = par.expected;
		if (!expected.length) return;
		auto const top_length = expected.length - 1;
		for (size_t length = 0; length < top_length; length += 4) {
			io::limited_vector actual{length};
			ASSERT_FALSE(par.cookies->store_raw(actual, then));
			ASSERT_EQ(expected.subspan(0, length), actual.buffer)
			    << "Expected length is: " << length << '/' << expected.length
			    << "\nActual length is: " << actual.buffer.size()
			    << "\nExpected hex:\n"
			    << printable{expected.subspan(0, length)} << "\nActual hex:\n"
			    << printable{actual.buffer};
		}
		std::vector<uint8_t> actual{};
		ASSERT_TRUE(par.cookies->store_raw(actual, then));
		ASSERT_EQ(expected, actual);
	}

	static constexpr uint8_t CJAR_1_0[] = {
	    FILE_HEADER(1, 0), V1_HEADER(2, io::sizeof_item, io::data_offset, 0),

	    V1_ITEM_STR(1, 7, 13, 25), V1_ITEM_FLAGS(0x09),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    V1_ITEM_STR(1, 27, 34, 25), V1_ITEM_FLAGS(0x01),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    U32(50),
	    // 0: ""
	    0x00,
	    // 1: "name2"
	    0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
	    // 7: "value"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
	    // 13: "example.com"
	    0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
	    // 25: "/"
	    0x2f, 0x00,
	    // 27: "value2"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
	    // 34: "www.example.com"
	    0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e,
	    0x63, 0x6f, 0x6d, 0x00};

	static constexpr uint8_t CJAR_1_0_expired[] = {
	    FILE_HEADER(1, 0), V1_HEADER(3, io::sizeof_item, io::data_offset, 0),

	    V1_ITEM_STR(1, 7, 13, 25), V1_ITEM_FLAGS(0x09),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    V1_ITEM_STR(1, 27, 34, 25), V1_ITEM_FLAGS(0x01),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    V1_ITEM_STR(1, 27, 34, 25), V1_ITEM_FLAGS(0x01),
	    V1_ITEM_TIMESTAMP(0x00000000, 0x00000010),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    U32(50),
	    // 0: ""
	    0x00,
	    // 1: "name2"
	    0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
	    // 7: "value"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
	    // 13: "example.com"
	    0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
	    // 25: "/"
	    0x2f, 0x00,
	    // 27: "value2"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
	    // 34: "www.example.com"
	    0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e,
	    0x63, 0x6f, 0x6d, 0x00};

	item make_item(const std::string& name,
	               const std::string& value,
	               const scope_type& scope,
	               flags flags,
	               time_point expires,
	               time_point created) {
		item result{name, value, scope, flags, expires, created};
		result.last_access_time(created);
		return result;
	}

	static const jar cookies{make_item("name2",
	                                   "value",
	                                   {"example.com", "/"},
	                                   flags::http_only | flags::persistent,
	                                   make_time(2100, 1, 1, 12, 30, 00),
	                                   make_time(2016, 5, 29, 12, 30, 00)),
	                         make_item("name2",
	                                   "value2",
	                                   {"www.example.com", "/"},
	                                   flags::persistent,
	                                   make_time(2100, 1, 1, 12, 30, 00),
	                                   make_time(2016, 5, 29, 12, 30, 00))};

	static const jar expired{
	    make_item("name2",
	              "value",
	              {"example.com", "/"},
	              flags::http_only | flags::persistent,
	              make_time(2100, 1, 1, 12, 30, 00),
	              make_time(2016, 5, 29, 12, 30, 00)),
	    make_item("name2",
	              "value2",
	              {"www.example.com", "/"},
	              flags::persistent,
	              make_time(2100, 1, 1, 12, 30, 00),
	              make_time(2016, 5, 29, 12, 30, 00)),
	    make_item("name2",
	              "value2",
	              {"www.example.com", "/"},
	              flags::persistent,
	              clock::time_point{std::chrono::seconds{16}},
	              make_time(2016, 5, 29, 12, 30, 00))};

	static const store_info experiments[] = {
	    {CJAR_1_0, &cookies},
	    {CJAR_1_0_expired, &expired},
	};

	INSTANTIATE_TEST_SUITE_P(experiments,
	                         cookie_jar_store,
	                         ::testing::ValuesIn(experiments));
}  // namespace
