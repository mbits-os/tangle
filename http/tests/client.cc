// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/http/client.hpp>

using namespace std::literals;

namespace tangle::http::client::testing {
	TEST(request, empty) {
		request req{};
		EXPECT_TRUE(req.address().empty());
		EXPECT_TRUE(req.headers().empty());
	}

	TEST(request, basic_auth) {
		request req{};
		req.set_basic_auth("user", "password");
		auto authorization = req.headers().find_front(header::Authorization);
		ASSERT_NE(authorization, nullptr);
		ASSERT_EQ(*authorization, "Basic dXNlcjpwYXNzd29yZA==");
	}
}  // namespace tangle::http::client::testing
