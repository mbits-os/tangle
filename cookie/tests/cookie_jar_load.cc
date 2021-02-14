// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "cookie-dir.hh"
#include "testing_io.hpp"
#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	void write_jars();

	struct load_info {
		io::data_span span;
		bool valid;
		const jar* expected;
	};

	class cookie_jar_load : public ::testing::TestWithParam<load_info> {
	public:
		static void SetUpTestSuite() { write_jars(); }
	};

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

	TEST_P(cookie_jar_load, parse) {
		auto& par = GetParam();
		auto& expected = *par.expected;
		io::data data{par.span};
		jar actual;

		ASSERT_EQ(par.valid, actual.load_raw(&data, now()));
		if (!par.valid) return;

		ASSERT_EQ(expected.size(), actual.size());

		auto pact = std::begin(actual);
		for (auto& ex : expected) {
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

	TEST_P(cookie_jar_load, partial_parse) {
		auto& par = GetParam();
		if (!par.span.length) return;
		auto const top_length = par.span.length - 1;
		for (size_t length = 0; length < top_length; ++length) {
			io::data data{par.span.subspan(0, length)};
			jar actual;
			ASSERT_FALSE(actual.load_raw(&data, now()));
		}
	}

	static constexpr uint8_t HELLO[] = {0x48, 0x45, 0x4c, 0x4f};

	static constexpr uint8_t CJAR_2_0[] = {
	    FILE_HEADER(2, 0), V1_HEADER(2, io::sizeof_item, io::data_offset, 0),

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

	static constexpr uint8_t CJAR_1_0_loose[] = {
	    FILE_HEADER(1, 0),
	    V1_HEADER(2, io::sizeof_item + 8, io::data_offset + 12, 48),

	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD

	    V1_ITEM_STR(1, 7, 13, 25), V1_ITEM_FLAGS(0x09),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    //
	    NULL_BYTES, NULL_BYTES,  // PAD

	    V1_ITEM_STR(27, 33, 40, 56), V1_ITEM_FLAGS(0x01),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    //
	    NULL_BYTES, NULL_BYTES,  // PAD

	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD

	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD

	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD
	    NULL_BYTES,  // PAD

	    U32(58),
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
	    // 27: "name2"
	    0x6e, 0x61, 0x6d, 0x65, 0x32, 0x00,
	    // 33: "value"
	    0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x00,
	    // 40: "www.example.com"
	    0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e,
	    0x63, 0x6f, 0x6d, 0x00,
	    // 56: "/"
	    0x2f, 0x00};

	static constexpr uint8_t CJAR_1_0_compress[] = {
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

	static constexpr uint8_t CJAR_1_1_minor[] = {
	    FILE_HEADER(1, 1), V1_HEADER(2, io::sizeof_item, io::data_offset, 0),

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
	    V1_ITEM_TIMESTAMP(0x00000000, 0x00000000),
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

	static constexpr uint8_t CJAR_1_0_nopersist[] = {
	    FILE_HEADER(1, 0), V1_HEADER(2, io::sizeof_item, io::data_offset, 0),

	    V1_ITEM_STR(1, 7, 13, 25), V1_ITEM_FLAGS(0x08),
	    V1_ITEM_TIMESTAMP(0x00000003, 0xaa9797c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),
	    V1_ITEM_TIMESTAMP(0x00000003, 0x0d5b71c8),

	    V1_ITEM_STR(1, 27, 34, 25), V1_ITEM_FLAGS(0x00),
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

	static const jar tested{{"name2",
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
	                         make_time(2016, 5, 29, 12, 30, 00)}};

	static const load_info experiments[] = {{{}, false},
	                                        {HELLO, false},
	                                        {CJAR_2_0, false},
	                                        {CJAR_1_0_loose, true, &tested},
	                                        {CJAR_1_0_compress, true, &tested},
	                                        {CJAR_1_1_minor, true, &tested},
	                                        {CJAR_1_0_expired, true, &tested},
	                                        {CJAR_1_0_nopersist, false}};

	INSTANTIATE_TEST_SUITE_P(experiments,
	                         cookie_jar_load,
	                         ::testing::ValuesIn(experiments));

	void write_jars() {
		if constexpr (false) {
			fs::path dirname{io::DATA_DIR};

			struct file_info {
				io::data_span span;
				char const* filename;
			};

			struct file_closer {
				void operator()(FILE* f) { std::fclose(f); }
			};

			static constexpr file_info files[] = {
			    {HELLO, "hello.jar"},
			    {CJAR_2_0, "2.0.jar"},
			    {CJAR_1_0_loose, "1.0-loose.jar"},
			    {CJAR_1_0_compress, "1.0.jar"},
			    {CJAR_1_1_minor, "1.1.jar"},
			    {CJAR_1_0_expired, "1.0-expired.jar"},
			};

			for (auto const& [data, filename] : files) {
				auto path = dirname / filename;
				auto const path_str = path.make_preferred().string();

				std::unique_ptr<FILE, file_closer> file{
				    std::fopen(path_str.c_str(), "wb")};
				if (!file) continue;
				std::fwrite(data.bytes, 1, data.length, file.get());
			}
		}
	}

}  // namespace
