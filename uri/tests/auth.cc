// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/uri.hpp>

namespace tangle::testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct uri_auth_info {
		std::string_view auth{};
		struct exp {
			std::string_view user{};
			std::string_view pass{};
			std::string_view host{};
			std::string_view port{};
		} expected{};
		std::string_view merged{};
		uri::auth_flag how_merged{uri::with_pass};
	};

	inline std::string to_string(std::string_view sv) {
		return {sv.data(), sv.length()};
	}
}  // namespace tangle::testing

std::ostream& operator<<(std::ostream& o,
                         const tangle::testing::uri_auth_info& param) {
	o << "\"" << param.auth << "\" -> ";
	if (!param.expected.user.empty()) {
		o << "[" << param.expected.user << "]";
		if (!param.expected.pass.empty())
			o << ":[" << param.expected.pass << "]";
		o << "@";
	}
	o << "[" << param.expected.host << "]";
	if (!param.expected.port.empty()) o << ":[" << param.expected.port << "]";

	o << " (";
	switch (param.how_merged) {
		case tangle::uri::ui_safe:
			o << "UI-safe";
			break;
		case tangle::uri::with_pass:
			o << "unsafe / with pass";
			break;
		case tangle::uri::no_userinfo:
			o << "host-only / no userinfo";
			break;
		default:
			o << '<' << (int)param.how_merged << '>';
			break;
	}
	o << ")";

	return o;
}

namespace tangle::testing {
	class uri_auth : public TestWithParam<uri_auth_info> {};

	TEST_P(uri_auth, breakup) {
		auto param = GetParam();
		auto& expected = param.expected;
		auto auth = uri::auth_parts::parse(param.auth);
		ASSERT_EQ(expected.user, auth.user);
		ASSERT_EQ(expected.pass, auth.password);
		ASSERT_EQ(expected.host, auth.host);
		ASSERT_EQ(expected.port, auth.port);
	}

	TEST_P(uri_auth, breakup_2) {
		auto param = GetParam();
		auto& expected = param.expected;
		auto auth = uri{"dummy://" +
		                std::string{param.auth.data(), param.auth.size()} + "/"}
		                .parsed_authority();
		ASSERT_EQ(expected.user, auth.user);
		ASSERT_EQ(expected.pass, auth.password);
		ASSERT_EQ(expected.host, auth.host);
		ASSERT_EQ(expected.port, auth.port);
	}

	TEST_P(uri_auth, merge) {
		auto param = GetParam();
		auto& data = param.expected;
		auto& expected = param.merged.empty() ? param.auth : param.merged;
		uri::auth_parts auth{to_string(data.user), to_string(data.pass),
		                     to_string(data.host), to_string(data.port)};
		ASSERT_EQ(expected, auth.string(param.how_merged));
	}

	static const uri_auth_info parts[] = {
	    {
	        "",
	        {},
	    },
	    {
	        "example.com:8042",
	        {{}, {}, "example.com", "8042"},
	    },
	    {
	        "user@example.com:8042",
	        {"user", {}, "example.com", "8042"},
	    },
	    {
	        "user:pass@example.com:8042",
	        {"user", "pass", "example.com", "8042"},
	    },
	    {
	        "user:pass:word@example.com:8042",
	        {"user", "pass:word", "example.com", "8042"},
	    },
	    {
	        "user:pass:word@example.com",
	        {"user", "pass:word", "example.com"},
	        "user@example.com",
	        uri::ui_safe,
	    },
	    {
	        "user:pass:word@example.com",
	        {"user", "pass:word", "example.com"},
	        {},
	        uri::with_pass,
	    },
	    {
	        "user:pass:word@example.com",
	        {"user", "pass:word", "example.com"},
	        "example.com",
	        uri::no_userinfo,
	    },
	    {
	        "user%3Apass:word@example.com",
	        {"user:pass", "word", "example.com"},
	    },
	    {
	        "user%40mail@example.com",
	        {"user@mail", {}, "example.com"},
	    },
	    {
	        // overall nightmare of an example, required "merged" field for
	        // a more proper host
	        "user@mail@example.com",
	        {"user", {}, "mail@example.com"},
	        "user@mail%40example.com",
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples, uri_auth, ValuesIn(parts));
}  // namespace tangle::testing
