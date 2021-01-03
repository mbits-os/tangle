#include <gtest/gtest.h>
#include <tangle/uri.h>

namespace tangle { namespace testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct uri_auth_info {
		std::string_view auth;
		struct exp {
			std::string_view user;
			std::string_view pass;
			std::string_view host;
			std::string_view port;
		} expected;
		std::string_view merged;
	};

	inline std::string to_string(std::string_view sv) {
		return { sv.data(), sv.length() };
	}
}}

std::ostream& operator<<(std::ostream& o, const tangle::testing::uri_auth_info& param)
{
	o << "\"" << param.auth << "\" -> ";
	if (!param.expected.user.empty()) {
		o << "[" << param.expected.user << "]";
		if (!param.expected.pass.empty())
			o << ":[" << param.expected.pass << "]";
		o << "@";
	}
	o << "[" << param.expected.host << "]";
	if (!param.expected.port.empty())
		o << ":[" << param.expected.port << "]";

	return o;
}

namespace tangle { namespace testing {
	class uri_auth : public TestWithParam<uri_auth_info> { };

	TEST_P(uri_auth, breakup)
	{
		auto param = GetParam();
		auto& expected = param.expected;
		auto auth = uri::auth_parts::parse(param.auth);
		ASSERT_EQ(expected.user, auth.user);
		ASSERT_EQ(expected.pass, auth.password);
		ASSERT_EQ(expected.host, auth.host);
		ASSERT_EQ(expected.port, auth.port);
	}

	TEST_P(uri_auth, breakup_2)
	{
		auto param = GetParam();
		auto& expected = param.expected;
		auto auth = uri{"dummy://" + param.auth + "/"}.parsed_authority();
		ASSERT_EQ(expected.user, auth.user);
		ASSERT_EQ(expected.pass, auth.password);
		ASSERT_EQ(expected.host, auth.host);
		ASSERT_EQ(expected.port, auth.port);
	}

	TEST_P(uri_auth, merge)
	{
		auto param = GetParam();
		auto& data = param.expected;
		auto& expected = param.merged.empty() ? param.auth : param.merged;
		uri::auth_parts auth {
			to_string(data.user),
			to_string(data.pass),
			to_string(data.host),
			to_string(data.port)
		};
		ASSERT_EQ(expected, auth.string(uri::with_pass));
	}

	static const uri_auth_info parts[] = {
		{
			"",
			{ }
		},
		{
			"example.com:8042",
			{ { }, { }, "example.com", "8042" }
		},
		{
			"user@example.com:8042",
			{ "user", { }, "example.com", "8042" }
		},
		{
			"user:pass@example.com:8042",
			{ "user", "pass", "example.com", "8042" }
		},
		{
			"user:pass:word@example.com:8042",
			{ "user", "pass:word", "example.com", "8042" }
		},
		{
			"user:pass:word@example.com",
			{ "user", "pass:word", "example.com" }
		},
		{
			"user%3Apass:word@example.com",
			{ "user:pass", "word", "example.com" }
		},
		{
			"user%40mail@example.com",
			{ "user@mail", { }, "example.com" }
		},
		{ // overall nightmare of an example, required "merged" field for a more proper host
			"user@mail@example.com",
			{ "user", { }, "mail@example.com" },
			"user@mail%40example.com"
		},
	};

	INSTANTIATE_TEST_CASE_P(samples, uri_auth, ValuesIn(parts));
}}
