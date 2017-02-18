/*
 * Copyright (C) 2017 midnightBITS
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
#include <gmock/gmock.h>
#include <tangle/nav/navigator.h>
#include <tangle/nav/jar.h>
#include <tangle/nav/cache.h>
#include <tangle/nav/protocol.h>

namespace tangle { namespace nav { namespace testing {
	class navigator_base : public ::testing::Test {
	};

	static bool langs_equal(const std::vector<std::string>& left, const std::vector<std::string>& right) {
		if (left.size() != right.size())
			return false;
		auto lit = left.cbegin();
		for (const auto& r : right) {
			if (*lit != r)
				return false;
			++lit;
		}
		return true;
	}

	TEST(navigator_base, create_empty) {
		navigator nav{  };
		ASSERT_TRUE(nav.user_agent().empty()) << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty()) << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_FALSE(nav.cache().storage_backed());
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_ua) {
		config cfg{
			"user-agent-name"
		};
		navigator nav{ cfg };
		ASSERT_EQ(nav.user_agent(), cfg.app_version);
		ASSERT_TRUE(nav.cookies().path().empty()) << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_FALSE(nav.cache().storage_backed());
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_cookies) {
		config cfg{
			{},
			"cookies.dat"
		};
		navigator nav{ cfg };
		ASSERT_TRUE(nav.user_agent().empty()) << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_EQ(nav.cookies().path(), cfg.jar_file);
		ASSERT_FALSE(nav.cache().storage_backed());
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_cache) {
		config cfg{
			{},
			{},
			"tangle.cache"
		};
		navigator nav{ cfg };
		ASSERT_TRUE(nav.user_agent().empty()) << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty()) << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_TRUE(nav.cache().storage_backed());
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_languages) {
		config cfg{
			{},
			{},
			{},
			{
				"first",
				"second",
				"third",
			}
		};
		navigator nav{ cfg };
		ASSERT_TRUE(nav.user_agent().empty()) << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty()) << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_EQ(nav.languages().size(), cfg.languages.size());
		ASSERT_FALSE(nav.cache().storage_backed());
		ASSERT_TRUE(langs_equal(nav.languages(), cfg.languages));
	}

	struct ProtocolMock : public protocol {
		MOCK_METHOD2(open, loader(const request&, navigator&));
	};

	TEST(navigator_base, create_proto) {
		using ::testing::_;
		using ::testing::Return;
		navigator nav{};
		auto proto = std::make_shared<ProtocolMock>();
		EXPECT_CALL(*proto, open(_, _))
			.Times(2)
			.WillRepeatedly(Return(loader{}));
		nav.reg_proto("bar", proto);
		nav.open(request{ "foo://example.com/" }, true);
		nav.open(request{ "bar://example.com/" }, true);
		nav.open(request{ "bar://example.com/" }, true);
		nav.open(request{ "baz://example.com/" }, true);
	}
}}}
