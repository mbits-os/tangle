/*
 * Copyright (C) 2016 midnightBITS
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

#include <tangle/cookie/chrono.h>
#include <cctype>
#include <ctime>

namespace tangle { namespace cookie { namespace chrono {

	// gmtime/timegm doesn't work nice with the 1601-1970 range, required by RFC
	// most of this is taken from boost/chrono/io/time_point_io.hpp
	namespace {
		constexpr inline int days_from_0(int year)
		{
			return 365 * (year - 1) + ((year - 1) / 400) - ((year - 1) / 100) + ((year - 1) / 4);
		}

		constexpr inline int days_from_epoch(int year)
		{
			return days_from_0(year) - days_from_0(1601);
		}

		inline int leap_ndx(int year)
		{
			if (year % 400 == 0)
				return 1;
			if (year % 100 == 0)
				return 0;
			if (year % 4 == 0)
				return 1;
			return 0;
		}

		inline int days_from_1jan(int year, int month, int day)
		{
			static const int days[2][12] =
			{
				{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
				{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 }
			};
			return days[leap_ndx(year)][month - 1] + day - 1;
		}

		template <typename Int>
		inline int month_from_days_in_year(int year, Int& count)
		{
			static const int days[2][13] =
			{
				{ -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364 },
				{ -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 }
			};
			auto& ldays = days[leap_ndx(year)];
			int month = 0;
			while (month < 14 && ldays[month] < count)
				++month;
			--month;
			if (month == 13) {
				count = 1;
				return 1;
			}
			count -= ldays[month];
			return month + 1;
		}
	}

	clock::time_point clock::now()
	{
		using namespace std::chrono;
		static constexpr duration::rep days_from_epoch = chrono::days_from_epoch(1970);
		static constexpr auto from_epoch = seconds(days_from_epoch * 3600 * 24);
		// that breaks the "always in GMT" rule
		auto since_1970 = duration_cast<seconds>(system_clock::now().time_since_epoch());
		return time_point { since_1970 + from_epoch };
	}

	clock::time_point clock::from_date(const datetime& time)
	{
		if (time.mday < 1 || time.mday > 31 || time.month < 1 || time.month > 12 ||
			time.year < 1601 || time.hour > 23 || time.minute > 59 || time.second > 59)
			return clock::max_epoch();

		const auto day_of_year = days_from_1jan(time.year, time.month, time.mday);
		duration::rep result = days_from_epoch(time.year) + day_of_year;
		result *= 3600 * 24;
		result += 3600 * time.hour;
		result += 60 * time.minute;
		result += time.second;

		return time_point { duration{ result } };
	}

	datetime clock::to_date(time_point time)
	{
		datetime result;
		auto count = time.time_since_epoch().count();
		result.second = int(count % 60); count /= 60;
		result.minute = int(count % 60); count /= 60;
		result.hour = int(count % 24); count /= 24;
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

	inline int gauss_wkday(int year, int month, int mday)
	{
		if (month < 3) --year;
		--month;
		month += 10;
		month %= 12;
		++month; // march -> 1, feb -> 12
		return (mday + (month * 26 - 2) / 10 + 5 * (year % 4) + 4 * (year % 100) + 6 * (year % 400)) % 7;
	}

	std::string clock::to_string(time_point point)
	{
		static const char* wkdays[] = {
			"Sun", "Mon", "Tue", "Wed",
			"Thu", "Fri", "Sat"
		};
		static const char* mnths[] = {
			"Jan", "Feb", "Mar", "Apr",
			"May", "Jun", "Jul", "Aug",
			"Sep", "Oct", "Nov", "Dec",
			"?"
		};

		auto time = to_date(point);
		char buffer[100];
#ifdef _WIN32
		sprintf_s(buffer,
#else
		sprintf(buffer,
#endif
			"%s, %02d %s %04d %02d:%02d:%02d GMT",
			wkdays[gauss_wkday(time.year, time.month, time.mday)],
			time.mday,
			mnths[time.month < 13 && time.month > 0 ? time.month - 1 : 13],
			time.year,
			time.hour,
			time.minute,
			time.second);

		return buffer;
	}
}}}