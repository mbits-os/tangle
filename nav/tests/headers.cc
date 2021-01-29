// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/nav/headers.hpp>

using namespace std::literals;

namespace tangle::nav::testing {
	struct key_info {
		std::string_view name;
		bool user_defined{false};
	};

	class headers_tests : public ::testing::TestWithParam<key_info> {};

	TEST(headers_tests, make) {
		auto const key = header_key{header::Link};
		ASSERT_EQ(key.value(), header::Link);
		ASSERT_TRUE(key.user_defined().empty());
		ASSERT_FALSE(key.empty());

		auto const lower_case = header_key::make("link");
		auto const upper_case = header_key::make("LINK");
		auto const eXistenZ_case = header_key::make("lInk");
		auto const Normal_case = header_key::make("Link");

		ASSERT_EQ(key, lower_case);
		ASSERT_EQ(key, upper_case);
		ASSERT_EQ(key, eXistenZ_case);
		ASSERT_EQ(key, Normal_case);

		ASSERT_FALSE(key < lower_case);
		ASSERT_FALSE(key != lower_case);
	}

	TEST(headers_tests, user_defined) {
		auto const key = header_key::make("X-Header-Name");
		ASSERT_EQ(key.value(), header::user_defined_header);
		ASSERT_EQ(key.user_defined(), "x-header-name");
		ASSERT_FALSE(key.empty());

		auto const lower_case = header_key::make("x-header-name");
		auto const upper_case = header_key::make("X-HEADER-NAME");
		auto const eXistenZ_case = header_key::make("X-headeR-nAme");

		ASSERT_EQ(key, lower_case);
		ASSERT_EQ(key, upper_case);
		ASSERT_EQ(key, eXistenZ_case);
		ASSERT_NE(key, header_key::make("X-Header-Name-2"));
	}

	TEST(headers_tests, extension_hash) {
		headers hdrs;
		auto key = header_key::make("X-Header-Name");
		std::string value = "X-Header-Value";
		std::string value2 = "X-Header-Value-2";
		hdrs.set(key, value);
		hdrs.add(key, value2);
		hdrs.add(header_key::make("x-header-name"), "X-Header-Value-3");
		hdrs.add(header::Content_Type, "text/plain");

		ASSERT_EQ(hdrs.size(), 2);
		ASSERT_TRUE(hdrs.has(key));
		ASSERT_TRUE(hdrs.has(nav::header::Content_Type));

		hdrs.erase(header::Content_Type);

		ASSERT_EQ(hdrs.size(), 1);
		ASSERT_TRUE(hdrs.has(key));
		ASSERT_FALSE(hdrs.has(nav::header::Content_Type));

		auto x_hdr_value = hdrs.find_front(key);
		auto content_type = hdrs.find_front(nav::header::Content_Type);

		ASSERT_NE(x_hdr_value, nullptr);
		ASSERT_EQ(content_type, nullptr);

		ASSERT_EQ(*x_hdr_value, "X-Header-Value");

		auto it = hdrs.find(key);
		ASSERT_NE(it, hdrs.end());
		ASSERT_EQ(it->second.size(), 3);
		ASSERT_EQ(it->second[0], "X-Header-Value");
		ASSERT_EQ(it->second[1], "X-Header-Value-2");
		ASSERT_EQ(it->second[2], "X-Header-Value-3");

		ASSERT_NE(hdrs.begin(), hdrs.end());
		hdrs.clear();
		ASSERT_EQ(hdrs.begin(), hdrs.end());
	}

	TEST(headers_tests, empty) {
		auto const key = header_key{};
		ASSERT_EQ(key.value(), header::empty);
		ASSERT_EQ(key.name(), nullptr);
		ASSERT_TRUE(key.user_defined().empty());
		ASSERT_TRUE(key.empty());

		ASSERT_EQ(header_key::name(header::empty), nullptr);
		ASSERT_EQ(header_key::name(header::user_defined_header), nullptr);
	}

	TEST_P(headers_tests, keys) {
		auto const& param = GetParam();

		auto key = header_key::make({param.name.data(), param.name.length()});

		EXPECT_EQ(key.name(), param.name);

		if (param.user_defined) {
			EXPECT_EQ(key.value(), header::user_defined_header);
			EXPECT_FALSE(key.user_defined().empty());
			EXPECT_TRUE(key.user_defined_header());
			EXPECT_EQ(key.user_defined(), key.name());
		} else {
			EXPECT_NE(key.value(), header::user_defined_header);
			EXPECT_TRUE(key.user_defined().empty());
			EXPECT_FALSE(key.user_defined_header());
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
	                                    {"Sender"sv},
	                                    {"To"sv},
	                                    {"Resent-To"sv},
	                                    {"cc"sv},
	                                    {"Resent-cc"sv},
	                                    {"bcc"sv},
	                                    {"Resent-bcc"sv},
	                                    {"Message-ID"sv},
	                                    {"Resent-Message-ID"sv},
	                                    {"In-Reply-To"sv},
	                                    {"References"sv},
	                                    {"Keywords"sv},
	                                    {"Subject"sv},
	                                    {"Comments"sv},
	                                    {"Encrypted"sv},
	                                    {"Received"sv},
	                                    {"Resent-Reply-To"sv},
	                                    {"Resent-From"sv},
	                                    {"Resent-Sender"sv},
	                                    {"Resent-Date"sv},
	                                    {"Return-Path"sv},
	                                    {"x-header-name"sv, true}};
	INSTANTIATE_TEST_SUITE_P(keys, headers_tests, ::testing::ValuesIn(keys));

}  // namespace tangle::nav::testing
