// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/jar.hpp>
#include <gtest/gtest.h>

namespace {
	using namespace std::literals;
	using namespace tangle::cookie;

	struct session_info {
		const jar before;
		const clock::time_point when;
		const jar after;
	};

	class cookie_jar_prune : public ::testing::TestWithParam<session_info> {};

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

	TEST_P(cookie_jar_prune, modify) {
		auto& par = GetParam();
		auto actual = par.before;
		auto& expected = par.after;

		actual.prune(par.when);
		ASSERT_EQ(expected.size(), actual.size());
		auto pex = std::begin(expected);
		for (auto& act : actual) {
			auto& ex = *pex;
			++pex;
			ASSERT_EQ(ex.name(), act.name());
			ASSERT_EQ(ex.value(), act.value());
			ASSERT_EQ(ex.scope().domain, act.scope().domain);
			ASSERT_EQ(ex.scope().path, act.scope().path);
			ASSERT_EQ(ex.flags(), act.flags());
			ASSERT_EQ(ex.expires(), act.expires());
		}
	}

	static const session_info session[] = {
	    {{{"name", "value", {"example.com", "/"}, flags::host_only}},
	     now(),
	     {{"name", "value", {"example.com", "/"}, flags::host_only}}},
	    {{{"name", "value", {"example.com", "/"}, flags::host_only},
	      {"name2",
	       "value",
	       {"example.com", "/"},
	       flags::http_only | flags::persistent,
	       make_time(1601, 1, 1, 12, 30, 00)},
	      {"name2",
	       "value2",
	       {"www.example.com", "/"},
	       flags::persistent,
	       make_time(2100, 1, 1, 12, 30, 00)}},
	     make_time(2000, 1, 1, 12, 30, 00),
	     {{"name", "value", {"example.com", "/"}, flags::host_only},
	      {"name2",
	       "value2",
	       {"www.example.com", "/"},
	       flags::persistent,
	       make_time(2100, 1, 1, 12, 30, 00)}}},
	    {{{"name", "value", {"example.com", "/"}, flags::host_only},
	      {"name2",
	       "value",
	       {"example.com", "/"},
	       flags::http_only | flags::persistent,
	       make_time(1601, 1, 1, 12, 30, 00)},
	      {"name2",
	       "value2",
	       {"www.example.com", "/"},
	       flags::persistent,
	       make_time(2100, 1, 1, 12, 30, 00)}},
	     make_time(2100, 1, 1, 12, 30, 00),
	     {
	         {"name", "value", {"example.com", "/"}, flags::host_only},
	     }},
	};

	INSTANTIATE_TEST_SUITE_P(session,
	                         cookie_jar_prune,
	                         ::testing::ValuesIn(session));
}  // namespace
