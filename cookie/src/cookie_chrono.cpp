// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/chrono.hpp>
#include <cctype>
#include <ctime>

namespace tangle::cookie::chrono {

	// gmtime/timegm doesn't work nice with the 1601-1970 range, required by RFC
	// most of this is taken from boost/chrono/io/time_point_io.hpp; that file
	// is covered by Boost Software License, Version 1.0; a copy of that license
	// may be found either in BSL_1.0.txt in this directory or at
	// http://www.boost.org/LICENSE_1_0.txt
	namespace {
		constexpr inline int days_from_0(int year) {
			return 365 * (year - 1) + ((year - 1) / 400) - ((year - 1) / 100) +
			       ((year - 1) / 4);
		}

		constexpr inline int days_from_epoch(int year) {
			return days_from_0(year) - days_from_0(1601);
		}

		inline int leap_ndx(int year) {
			if (year % 400 == 0) return 1;
			if (year % 100 == 0) return 0;
			if (year % 4 == 0) return 1;
			return 0;
		}

		inline int days_from_1jan(int year, int month, int day) {
			static const int days[2][12] = {
			    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
			    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}};
			return days[leap_ndx(year)][month - 1] + day - 1;
		}

		template <typename Int>
		inline int month_from_days_in_year(int year, Int& doy) {
			static const unsigned char day_of_year_month[2][366] = {
			    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,
			     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
			     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,
			     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
			     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,
			     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
			     4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,
			     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			     5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			     6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
			     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
			     7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
			     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
			     8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
			     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
			     9,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
			     10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
			     11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
			     11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12,
			     12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
			     12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
			    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
			     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,
			     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
			     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
			     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
			     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,
			     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
			     4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,
			     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
			     5,  5,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,
			     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
			     6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
			     7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
			     7,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
			     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
			     8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
			     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
			     9,  9,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
			     10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
			     10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
			     11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
			     12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
			     12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}};
			static const int days[2][13] = {
			    {-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364},
			    {-1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}};
			auto const leap = leap_ndx(year);
			int month = day_of_year_month[leap][doy];
			doy -= days[leap][month - 1];
			return month;
		}
	}  // namespace

	clock::time_point clock::now() {
		using namespace std::chrono;
		static constexpr duration::rep days_from_epoch =
		    chrono::days_from_epoch(1970);
		static constexpr auto from_epoch = seconds(days_from_epoch * 3600 * 24);
		// that breaks the "always in GMT" rule
		auto since_1970 =
		    duration_cast<seconds>(system_clock::now().time_since_epoch());
		return time_point{since_1970 + from_epoch};
	}

	clock::time_point clock::from_date(const datetime& time) {
		if (time.mday < 1 || time.mday > 31 || time.month < 1 ||
		    time.month > 12 || time.year < 1601 || time.hour > 23 ||
		    time.minute > 59 || time.second > 59)
			return clock::max_epoch();

		const auto day_of_year =
		    days_from_1jan(time.year, time.month, time.mday);
		duration::rep result = days_from_epoch(time.year) + day_of_year;
		result *= 3600 * 24;
		result += 3600 * time.hour;
		result += 60 * time.minute;
		result += time.second;

		return time_point{duration{result}};
	}

	datetime clock::to_date(time_point time) {
		datetime result;
		auto count = time.time_since_epoch().count();
		result.second = int(count % 60);
		count /= 60;
		result.minute = int(count % 60);
		count /= 60;
		result.hour = int(count % 24);
		count /= 24;
		result.year = int(count / 365) + 1601;
		auto dys = days_from_epoch(result.year);
		while (dys > count) {
			--result.year;
			dys = days_from_epoch(result.year);
		}
		count -= dys;
		result.month = month_from_days_in_year(result.year, count);
		result.mday = int(count);
		return result;
	}

	inline int gauss_wkday(int year, int month, int mday) {
		if (month < 3) --year;
		--month;
		month += 10;
		month %= 12;
		++month;  // march -> 1, feb -> 12
		return (mday + (month * 26 - 2) / 10 + 5 * (year % 4) +
		        4 * (year % 100) + 6 * (year % 400)) %
		       7;
	}

	std::string clock::to_string(time_point point) {
		static const char* wkdays[] = {"Sun", "Mon", "Tue", "Wed",
		                               "Thu", "Fri", "Sat"};
		static const char* mnths[] = {"Jan", "Feb", "Mar", "Apr", "May",
		                              "Jun", "Jul", "Aug", "Sep", "Oct",
		                              "Nov", "Dec", "?"};

		auto time = to_date(point);
		char buffer[100];
#ifdef _WIN32
		sprintf_s(
		    buffer,
#else
		sprintf(
		    buffer,
#endif
		    "%s, %02d %s %04d %02d:%02d:%02d GMT",
		    wkdays[gauss_wkday(time.year, time.month, time.mday)], time.mday,
		    mnths[time.month < 13 && time.month > 0 ? time.month - 1 : 13],
		    time.year, time.hour, time.minute, time.second);

		return buffer;
	}
}  // namespace tangle::cookie::chrono
