// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/nav/request.hpp>

using namespace std::literals;

namespace tangle::nav::testing {
	class req_tests : public ::testing::Test {};

	TEST(req_tests, empty) {
		request req{};

		ASSERT_TRUE(req.address().string().empty());
		ASSERT_TRUE(req.meta().empty());
		ASSERT_EQ(req.method(), method::get);
		ASSERT_TRUE(req.follow_redirects());
		ASSERT_EQ(req.max_redir(), 10);
		ASSERT_TRUE(req.referrer().string().empty());
		ASSERT_TRUE(req.custom_agent().empty());
		ASSERT_TRUE(req.content_type().empty());
		ASSERT_TRUE(req.content().empty());
		ASSERT_TRUE(req.form_fields().empty());
	}

	TEST(req_test, method) {
		request req{method::post};
		ASSERT_EQ(req.method(), method::post);
        req.method(method::get);
		ASSERT_EQ(req.method(), method::get);
	}

	TEST(req_test, referrer) {
		tangle::uri const dir_file{"dir/file.ext"sv};
		request req{dir_file};
		ASSERT_TRUE(req.referrer().string().empty());
		ASSERT_EQ(req.address().string(), dir_file.string());

		req.referrer("scheme://host/app/source.ext"sv);
		ASSERT_EQ(req.address().string(), "scheme://host/app/dir/file.ext"sv);
	}

	TEST(req_test, referrer2) {
		request req{method::get, "dir/file.ext"sv};
		ASSERT_TRUE(req.referrer().string().empty());
        req.address("dir/file2.ext"sv);
		ASSERT_EQ(req.address().string(), "dir/file2.ext"sv);

		req.referrer("scheme://host/app/source.ext"sv);
		ASSERT_EQ(req.address().string(), "scheme://host/app/dir/file2.ext"sv);

        req.address("../app2/file2.ext"sv);
		ASSERT_EQ(req.address().string(), "scheme://host/app2/file2.ext"sv);
	}

    TEST(req_test, redir) {
        request req{};

        ASSERT_TRUE(req.follow_redirects());
        ASSERT_EQ(req.max_redir(), 10);

        req.max_redir(-1);
        ASSERT_FALSE(req.follow_redirects());
        ASSERT_EQ(req.max_redir(), -1);

        req.max_redir(30);
        ASSERT_TRUE(req.follow_redirects());
        ASSERT_EQ(req.max_redir(), 30);

        req.max_redir(0);
        ASSERT_FALSE(req.follow_redirects());
        ASSERT_EQ(req.max_redir(), 0);
    }
}  // namespace tangle::nav::testing
