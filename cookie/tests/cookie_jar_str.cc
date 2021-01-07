// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/cookie/jar.hpp>

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	struct jar_info {
		const jar cookies;
		const scope_type scope;
		match options;
		const char* header;
	};

	std::ostream& operator<<(std::ostream& o, const jar_info& nfo) {
		if ((nfo.options & match::http) == match::http)
			o << "[NET]";
		else
			o << "[JS]";

		o << " http";
		if ((nfo.options & match::secure) == match::secure) o << 's';

		return o << "://" << nfo.scope.domain << nfo.scope.path;
	}

	class cookie_jar_str : public ::testing::TestWithParam<jar_info> {};

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

	TEST_P(cookie_jar_str, serialize) {
		auto& par = GetParam();
		auto jar = par.cookies;
		auto expected = par.header;

		auto actual = jar.str(par.scope, par.options, now());
		ASSERT_EQ(expected, actual);
	}

	static jar_info empty[] = {
	    {
	        // empty jar
	        {},
	        {"example.com", "/"},
	        match::secure,
	        "",
	    },
	    {
	        // flags mismatch
	        {
	            {"https_only", "value", {"example.com", "/"}, flags::secure},
	            {"no_subdoms", "value", {"example.com", "/"}, flags::host_only},
	            {"no_js", "value", {"example.com", "/"}, flags::http_only},
	        },
	        {"www.example.com", "/"},
	        match::none,
	        "",
	    },
	    {
	        // expired
	        {
	            {"5_seconds_ago",
	             "value",
	             {"example.com", "/"},
	             flags::persistent,
	             now() - 5s},
	            {"begining_of_time",
	             "value",
	             {"example.com", "/"},
	             flags::persistent,
	             make_time(1601, 1, 1, 0, 0, 0)},
	        },
	        {"example.com", "/"},
	        match::none,
	        "",
	    },
	};

	static const jar jar_contents{
	    {"password", "123456", {"example.com", "/"}, flags::http_only},
	    {"settings", "some", {"example.com", "/"}, flags::none},
	    {"password", "password", {"example.net", "/"}, flags::http_only},
	    {
	        "weak",
	        "value",
	        {"example.net", "/"},
	        flags::http_only | flags::secure,
	    },
	    {
	        "weakNone",
	        "value",
	        {"example.net", "/sub"},
	        flags::http_only | flags::secure,
	    },
	    {
	        "main",
	        "value",
	        {"example.net", "/"},
	        flags::http_only | flags::host_only,
	    },
	    {
	        "www",
	        "value",
	        {"www.example.net", "/"},
	        flags::http_only | flags::host_only,
	    },
	    {"wwwSubDom", "value", {"www.example.net", "/"}, flags::http_only},
	    {"wwwSubPath", "value", {"www.example.net", "/sub"}, flags::http_only},
	};

	static jar_info browser[] = {
	    {jar_contents, {"example.com", "/"}, match::none, "settings=some"},
	    {jar_contents,
	     {"example.com", "/"},
	     match::http,
	     "password=123456; settings=some"},
	    {jar_contents,
	     {"example.net", "/"},
	     match::http,
	     "password=password; main=value"},
	    {jar_contents,
	     {"example.net", "/"},
	     match::http | match::secure,
	     "password=password; weak=value; main=value"},
	    {jar_contents, {"example.net", "/"}, match::secure, ""},
	    {jar_contents,
	     {"www.example.net", "/"},
	     match::http,
	     "password=password; www=value; wwwSubDom=value"},
	    {jar_contents,
	     {"www.example.net", "/"},
	     match::http | match::secure,
	     "password=password; weak=value; www=value; wwwSubDom=value"},
	    {jar_contents, {"www.example.net", "/"}, match::secure, ""},
	    {jar_contents,
	     {"www.example.net", "/sub/path"},
	     match::http | match::secure,
	     "password=password; weak=value; weakNone=value; www=value; "
	     "wwwSubDom=value; wwwSubPath=value"},
	    {jar_contents, {"ns.www.example.net", "/"}, match::secure, ""},
	    {jar_contents,
	     {"ns.www.example.net", "/sub/path"},
	     match::http | match::secure,
	     "password=password; weak=value; weakNone=value; wwwSubDom=value; "
	     "wwwSubPath=value"},
	};

	INSTANTIATE_TEST_CASE_P(empty, cookie_jar_str, ::testing::ValuesIn(empty));
	INSTANTIATE_TEST_CASE_P(browser,
	                        cookie_jar_str,
	                        ::testing::ValuesIn(browser));
}  // namespace
