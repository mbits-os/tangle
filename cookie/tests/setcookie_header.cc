// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/cookie/item.hpp>

namespace {
	using namespace std::literals;
	struct cookie_info {
		const char* header;
		tangle::cookie::item cookie;
		bool prefer_maxage{false};
	};

	std::ostream& operator<<(std::ostream& o, const cookie_info& nfo) {
		return o << '`' << nfo.header << '`';
	}

	std::ostream& operator<<(std::ostream& o,
	                         const tangle::cookie::time_point& pt) {
		return o << pt.time_since_epoch().count();
	}

	class SetCookie_header : public ::testing::TestWithParam<cookie_info> {};

	auto now() {
		static auto when = tangle::cookie::clock::now();
		return when;
	}

	auto make_time(int year,
	               int month,
	               int day,
	               int hour,
	               int minute,
	               int second) {
		return tangle::cookie::clock::from_date(
		    {year, month, day, hour, minute, second});
	}

	TEST_P(SetCookie_header, serialize) {
		auto& par = GetParam();
		auto expected = par.header;
		auto actual = par.cookie.server_string(par.prefer_maxage, now());
		ASSERT_EQ(expected, actual);
	}

	using tangle::cookie::flags;
	static cookie_info flag_tests[] = {
	    {
	        "name=value",
	        {"name", "value", {}, flags::host_only},
	    },
	    {
	        "name=value",
	        {"name", "value", {"example.com", {}}, flags::host_only},
	    },
	    {
	        "name=value; Secure",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::host_only | flags::secure},
	    },
	    {
	        "name=value; HttpOnly",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::host_only | flags::http_only},
	    },
	    {
	        "name=value; Secure; HttpOnly",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::host_only | flags::http_only | flags::secure},
	    },
	};

	static cookie_info scope[] = {
	    {
	        "name=value; Path=/",
	        {"name", "value", {"example.com", "/"}, flags::host_only},
	    },
	    {
	        "name=value; Domain=example.com; Path=/",
	        {"name", "value", {"example.com", "/"}},
	    },
	    {
	        "name=value; Domain=www.example.com; Path=/",
	        {"name", "value", {"www.example.com", "/"}},
	    },
	    {
	        "name=value; Domain=www.example.com",
	        {"name", "value", {"www.example.com", {}}},
	    },
	    {
	        "name=value; Domain=example.com; Path=/res/ource",
	        {"name", "value", {"example.com", "/res/ource"}},
	    },
	};

	static cookie_info expires[] = {
	    {
	        "name=value; Domain=example.com; Expires=Sat, 08 May 2021 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(2021, 5, 8, 22, 23, 1)},
	    },
	    {
	        "name=value; Domain=example.com; Expires=Mon, 08 May 2000 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(2000, 5, 8, 22, 23, 1)},
	    },
	    {
	        "name=value; Domain=example.com; Expires=Wed, 08 May 2069 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(2069, 5, 8, 22, 23, 1)},
	    },
	    {
	        "name=value; Domain=example.com; Expires=Thu, 08 May 1980 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(1980, 5, 8, 22, 23, 1)},
	    },
	    {
	        "name=value; Domain=example.com; Expires=Wed, 08 May 1602 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(1602, 5, 8, 22, 23, 1)},
	    },
	    {
	        "name=value; Domain=example.com; Expires=Mon, 01 Feb 2016 22:23:01 "
	        "GMT",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         make_time(2016, 2, 1, 22, 23, 1)},
	    },
	};

	static cookie_info max_age[] = {
	    {
	        "name=value; Domain=example.com; Max-Age=200",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         now() + 200s},
	        true,
	    },
	    {
	        "name=value; Domain=example.com; Max-Age=0",
	        {"name", "value", {"example.com", {}}, flags::persistent, now()},
	        true,
	    },
	    {
	        "name=value; Domain=example.com; Max-Age=-5",
	        {"name",
	         "value",
	         {"example.com", {}},
	         flags::persistent,
	         now() - 5s},
	        true,
	    },
	};

	INSTANTIATE_TEST_SUITE_P(flags,
	                         SetCookie_header,
	                         ::testing::ValuesIn(flag_tests));
	INSTANTIATE_TEST_SUITE_P(scope,
	                         SetCookie_header,
	                         ::testing::ValuesIn(scope));
	INSTANTIATE_TEST_SUITE_P(expires,
	                         SetCookie_header,
	                         ::testing::ValuesIn(expires));
	INSTANTIATE_TEST_SUITE_P(max_age,
	                         SetCookie_header,
	                         ::testing::ValuesIn(max_age));
}  // namespace
