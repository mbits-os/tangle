// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/chrono.hpp>
#include <gtest/gtest.h>

namespace {
	using namespace tangle::cookie::chrono;

	using days = std::chrono::duration<int, std::ratio<86400>>;

	class cookies_chrono_dates : public ::testing::TestWithParam<datetime> {};

	TEST(cookies_chrono, year_too_far) {
		auto const input = clock::time_point{days{2190}};
		auto const date = clock::to_date(input);
		// 2190 / 4 + 1601 == 1607, but days{1607y} - days{1606y} == 2191
		EXPECT_EQ(date.year, 1606);
		EXPECT_EQ(date.month, 12);
		EXPECT_EQ(date.mday, 31);
		EXPECT_EQ(date.hour, 0);
		EXPECT_EQ(date.minute, 0);
		EXPECT_EQ(date.second, 0);
	}

	TEST_P(cookies_chrono_dates, bad) {
		auto const& date = GetParam();
		auto const actual = clock::from_date(date);
		EXPECT_EQ(clock::max_epoch(), actual);
	}

	static constexpr datetime bad_dates[] = {
	    {0, 1, 1, 0, 0, 0},     {1600, 1, 1, 0, 0, 0},  {1601, 0, 1, 0, 0, 0},
	    {1601, 13, 1, 0, 0, 0}, {1601, 1, 0, 0, 0, 0},  {1601, 1, 32, 0, 0, 0},
	    {1601, 1, 1, 25, 0, 0}, {1601, 1, 1, 0, 60, 0}, {1601, 1, 1, 0, 0, 60},
	};
	INSTANTIATE_TEST_SUITE_P(dates,
	                         cookies_chrono_dates,
	                         ::testing::ValuesIn(bad_dates));
}  // namespace