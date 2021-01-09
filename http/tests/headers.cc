// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/http/headers.hpp>

using namespace std::literals;

namespace tangle::http::testing {
	struct key_info {
		std::string_view name;
		bool extension{false};
	};

	class headers_tests : public ::testing::TestWithParam<key_info> {};

	TEST(headers_tests, make) {
		auto const key = header_key{header::Link};
		ASSERT_EQ(key.value(), header::Link);
		ASSERT_TRUE(key.extension().empty());
		ASSERT_FALSE(key.empty());

		auto const lower_case = header_key::make("link");
		auto const upper_case = header_key::make("LINK");
		auto const eXistenZ_case = header_key::make("lInk");
		auto const Normal_case = header_key::make("Link");

		ASSERT_EQ(key, lower_case);
		ASSERT_EQ(key, upper_case);
		ASSERT_EQ(key, eXistenZ_case);
		ASSERT_EQ(key, Normal_case);
	}

	TEST(headers_tests, extension) {
		auto const key = header_key::make("X-Header-Name");
		ASSERT_EQ(key.value(), header::extension_header);
		ASSERT_EQ(key.extension(), "x-header-name");
		ASSERT_FALSE(key.empty());

		auto const lower_case = header_key::make("x-header-name");
		auto const upper_case = header_key::make("X-HEADER-NAME");
		auto const eXistenZ_case = header_key::make("X-headeR-nAme");

		ASSERT_EQ(key, lower_case);
		ASSERT_EQ(key, upper_case);
		ASSERT_EQ(key, eXistenZ_case);
	}

	TEST(headers_tests, empty) {
		auto const key = header_key{};
		ASSERT_EQ(key.value(), header::empty);
		ASSERT_EQ(key.name(), nullptr);
		ASSERT_TRUE(key.extension().empty());
		ASSERT_TRUE(key.empty());

		ASSERT_EQ(header_key::name(header::empty), nullptr);
		ASSERT_EQ(header_key::name(header::extension_header), nullptr);
	}

	TEST_P(headers_tests, keys) {
		auto const& param = GetParam();

		auto key = header_key::make({param.name.data(), param.name.length()});

		EXPECT_EQ(key.name(), param.name);

		if (param.extension) {
			EXPECT_EQ(key.value(), header::extension_header);
			EXPECT_FALSE(key.extension().empty());
			EXPECT_TRUE(key.extension_header());
		} else {
			EXPECT_NE(key.value(), header::extension_header);
			EXPECT_TRUE(key.extension().empty());
			EXPECT_FALSE(key.extension_header());
		}
	}

	constexpr static key_info keys[] = {{"Accept"sv},
	                                    {"Accept-Charset"sv},
	                                    {"Accept-Encoding"sv},
	                                    {"Accept-Language"sv},
	                                    {"Accept-Ranges"sv},
	                                    {"Age"sv},
	                                    {"Allow"sv},
	                                    {"Authorization"sv},
	                                    {"Cache-Control"sv},
	                                    {"Connection"sv},
	                                    {"Content-Encoding"sv},
	                                    {"Content-Language"sv},
	                                    {"Content-Length"sv},
	                                    {"Content-Location"sv},
	                                    {"Content-MD5"sv},
	                                    {"Content-Range"sv},
	                                    {"Content-Type"sv},
	                                    {"Cookie"sv},
	                                    {"Date"sv},
	                                    {"ETag"sv},
	                                    {"Expect"sv},
	                                    {"Expires"sv},
	                                    {"From"sv},
	                                    {"Host"sv},
	                                    {"If-Match"sv},
	                                    {"If-Modified-Since"sv},
	                                    {"If-None-Match"sv},
	                                    {"If-Range"sv},
	                                    {"If-Unmodified-Since"sv},
	                                    {"Last-Modified"sv},
	                                    {"Location"sv},
	                                    {"Max-Forwards"sv},
	                                    {"Pragma"sv},
	                                    {"Proxy-Authenticate"sv},
	                                    {"Proxy-Authorization"sv},
	                                    {"Range"sv},
	                                    {"Referer"sv},
	                                    {"Retry-After"sv},
	                                    {"Server"sv},
	                                    {"Set-Cookie"sv},
	                                    {"TE"sv},
	                                    {"Trailer"sv},
	                                    {"Transfer-Encoding"sv},
	                                    {"Upgrade"sv},
	                                    {"User-Agent"sv},
	                                    {"Vary"sv},
	                                    {"Via"sv},
	                                    {"Warning"sv},
	                                    {"WWW-Authenticate"sv},
	                                    {"Content-Disposition"sv},
	                                    {"Origin"sv},
	                                    {"Access-Control-Allow-Origin"sv},
	                                    {"Access-Control-Allow-Credentials"sv},
	                                    {"Access-Control-Allow-Methods"sv},
	                                    {"Access-Control-Allow-Headers"sv},
	                                    {"Access-Control-Max-Age"sv},
	                                    {"Access-Control-Expose-Headers"sv},
	                                    {"Link"sv},
	                                    {"x-header-name"sv, true}};
	INSTANTIATE_TEST_CASE_P(keys, headers_tests, ::testing::ValuesIn(keys));

}  // namespace tangle::http::testing