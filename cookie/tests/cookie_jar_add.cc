// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include "testing_io.hpp"

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	struct session_info {
		const jar before;
		const item cookie;
		bool from_http;
		const jar after;

		io::data_span span;
	};

	std::ostream& operator<<(std::ostream& o, const session_info& nfo) {
		return o << nfo.cookie.server_string();
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

	class cookie_jar_add : public ::testing::TestWithParam<session_info> {};

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

	TEST_P(cookie_jar_add, modify) {
		auto& par = GetParam();
		auto actual = par.before;
		auto& expected = par.after;

		actual.add(par.cookie, par.from_http);
		auto moveable_copy = par.cookie;
		actual.add(std::move(moveable_copy), par.from_http);
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

	TEST_P(cookie_jar_add, store) {
		auto& par = GetParam();
		auto& jar = par.after;
		std::vector<uint8_t> actual;

		ASSERT_TRUE(jar.store_raw(actual, now()));
		ASSERT_EQ(par.span.length, actual.size());
		auto eptr = par.span.bytes;
		for (auto achar : actual) {
			auto echar = *eptr;
			ASSERT_EQ(echar, achar)
			    << "Offset: " << (eptr - par.span.bytes) / 32 << ':'
			    << (eptr - par.span.bytes) % 32 << "\nExpected hex:\n"
			    << printable{par.span} << "\nActual hex:\n"
			    << printable{actual};
			++eptr;
		}
	}

	TEST_P(cookie_jar_add, load) {
		auto& par = GetParam();
		auto& jar = par.after;
		io::data data{par.span};
		tangle::cookie::jar actual;

		ASSERT_TRUE(actual.load_raw(&data, now()));

		size_t expected_length = 0;
		for (auto& ex : jar) {
			if ((ex.flags() & flags::persistent) == flags::persistent &&
			    (ex.expires() > now()))
				++expected_length;
		}

		ASSERT_EQ(expected_length, actual.size());

		auto pact = std::begin(actual);
		for (auto& ex : jar) {
			if ((ex.flags() & flags::persistent) != flags::persistent ||
			    (ex.expires() <= now()))
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
	    FILE_HEADER(1, 0), V1_HEADER(0, io::sizeof_item, io::data_offset, 0),

	    U32(0x01),
	    // 0: ""
	    0x00};

	static constexpr uint8_t name2first[] = {
	    FILE_HEADER(1, 0), V1_HEADER(1, io::sizeof_item, io::data_offset, 0),

	    V1_ITEM_STR(1, 7, 13, 25), V1_ITEM_FLAGS(0x09),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    U32(27),
	    // 0: ""
	    0x00,
	    // 1: "name"
	    0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
	    // 7: "value"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x00,
	    // 13: "example.com"
	    0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x00,
	    // 25: "/"
	    0x2f, 0x00};

	static constexpr uint8_t both_name2[] = {
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
	    // 1: "name"
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

	static const session_info session[] = {
	    {
	        {},
	        {"name", "value", {"example.com", "/"}, flags::host_only},
	        true,
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	        },
	        empty,
	    },
	    {
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"expired",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(1601, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        {"name2",
	         "value",
	         {"example.com", "/"},
	         flags::http_only | flags::persistent,
	         make_time(2100, 1, 1, 12, 30, 00),
	         make_time(2016, 5, 29, 12, 30, 00)},
	        true,
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"expired",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(1601, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	            {"name2",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        name2first,
	    },
	    {
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"name2",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        {"name2", "value2", {"example.com", "/"}},
	        false,
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"name2",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        name2first,
	    },
	    {
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"name2", "value", {"example.com", "/"}, flags::http_only},
	        },
	        {"name", "value2", {"example.com", "/"}},
	        true,
	        {
	            {"name", "value2", {"example.com", "/"}},
	            {"name2", "value", {"example.com", "/"}, flags::http_only},
	        },
	        empty,
	    },
	    {
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"name2",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        {"name2",
	         "value2",
	         {"www.example.com", "/"},
	         flags::persistent,
	         make_time(2100, 1, 1, 12, 30, 00),
	         make_time(2016, 5, 29, 12, 30, 00)},
	        true,
	        {
	            {"name", "value", {"example.com", "/"}, flags::host_only},
	            {"name2",
	             "value",
	             {"example.com", "/"},
	             flags::http_only | flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	            {"name2",
	             "value2",
	             {"www.example.com", "/"},
	             flags::persistent,
	             make_time(2100, 1, 1, 12, 30, 00),
	             make_time(2016, 5, 29, 12, 30, 00)},
	        },
	        both_name2,
	    },
	};

	INSTANTIATE_TEST_CASE_P(session,
	                        cookie_jar_add,
	                        ::testing::ValuesIn(session));
}  // namespace
