// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/nav/jar.hpp>
#include <tangle/nav/navigator.hpp>
#include <tangle/nav/protocol.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::nav::testing {
	class navigator_base : public ::testing::Test {};

	static bool langs_equal(const std::vector<std::string>& left,
	                        const std::vector<std::string>& right) {
		if (left.size() != right.size()) return false;
		auto lit = left.cbegin();
		for (const auto& r : right) {
			if (*lit != r) return false;
			++lit;
		}
		return true;
	}

	TEST(navigator_base, create_empty) {
		navigator nav{};
		ASSERT_TRUE(nav.user_agent().empty())
		    << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty())
		    << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_ua) {
		config cfg{"user-agent-name"};
		navigator nav{cfg};
		ASSERT_EQ(nav.user_agent(), cfg.app_version);
		ASSERT_TRUE(nav.cookies().path().empty())
		    << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_cookies) {
		config cfg{{}, "cookies.dat"};
		navigator nav{cfg};
		ASSERT_TRUE(nav.user_agent().empty())
		    << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_EQ(nav.cookies().path(), cfg.jar_file);
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_cache) {
		config cfg{};
		navigator nav{cfg};
		ASSERT_TRUE(nav.user_agent().empty())
		    << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty())
		    << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_TRUE(nav.languages().empty());
	}

	TEST(navigator_base, create_languages) {
		config cfg{{},
		           {},
		           {
		               "first",
		               "second",
		               "third",
		           }};
		navigator nav{cfg};
		ASSERT_TRUE(nav.user_agent().empty())
		    << "user agent: \"" << nav.user_agent() << "\"";
		ASSERT_TRUE(nav.cookies().path().empty())
		    << "cookie path: \"" << nav.cookies().path() << "\"";
		ASSERT_EQ(nav.languages().size(), cfg.languages.size());
		ASSERT_TRUE(langs_equal(nav.languages(), cfg.languages));
	}

	struct ProtocolMock : public protocol {
		MOCK_METHOD2(open, tangle::nav::document(const request&, navigator&));
	};

	TEST(navigator_base, create_proto) {
		using ::testing::_;
		using ::testing::Return;
		navigator nav{};
		auto proto = std::make_shared<ProtocolMock>();
		EXPECT_CALL(*proto, open(_, _))
		    .Times(2)
		    .WillRepeatedly(Return(tangle::nav::document{}));
		nav.reg_proto("bar", proto);
		nav.open(request{"foo://example.com/"sv});
		nav.open(request{"bar://example.com/"sv});
		nav.open(request{"bar://example.com/"sv});
		nav.open(request{"baz://example.com/"sv});
		nav.reg_proto("bar", nullptr);
	}
}  // namespace tangle::nav::testing
