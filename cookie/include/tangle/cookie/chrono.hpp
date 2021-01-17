// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cookie clock code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

Defines extensions to std::chrono for cookie-compatible time point.
*/

#pragma once

#include <chrono>
#include <string>

/**
Chrono - extensions to std::chrono for cookie-compatible time point.

The cookie date should got back to 1601. The epoch for POSIX time (and, by
extension, std::chrono::system_clock) is 1970, or almost four centuries to
little. This namespace defines a clock whose epoch is January 1st 1601. Clock's
duration has 1 second interval - enough for clock for cookie expiration. In
addition to the clock the namespace defines a chrono::datetime structure,
helping with conversion between strings and time points.
*/
namespace tangle::cookie::chrono {

	/**
	Helper class for extracting date/time components from time_point.
	\see clock::to_date
	\see clock::from_date
	*/
	struct datetime {
		int year = 1601;  ///< The full year represented by the date.
		                  ///< The smallest possible value is 1601.
		int month = 1;    ///< The month represented by the date. Must be
		                  ///< in range 1 to 12, inclusive.
		int mday = 1;     ///< The day of the month represented by the
		                  ///< date. Must be in range 1 to 31, inclusive.
		int hour = 0;     ///< The hour represented by the date. Must be in
		                  ///< range 0 to 23, inclusive.
		int minute = 0;   ///< The minute represented by the date. Must
		                  ///< be in range 0 to 59, inclusive.
		int second = 0;   ///< The second represented by the date. Must
		                  ///< be in range 0 to 59, inclusive.

		/**
		Constructs a date representing January 1st, 1601, midnight
		*/
		constexpr datetime() noexcept = default;

		/**
		Constructs a date from given arguments
		\param year The year this datetime should be set to
		\param month The month this datetime should be set to
		\param mday The day of the month this datetime should be set to
		\param hour The hour this datetime should be set to
		\param minute The minute this datetime should be set to
		\param second The second this datetime should be set to
		*/
		constexpr datetime(int year,
		                   int month,
		                   int mday,
		                   int hour,
		                   int minute,
		                   int second) noexcept
		    : year{year}
		    , month{month}
		    , mday{mday}
		    , hour{hour}
		    , minute{minute}
		    , second{second} {}
	};

	/**
	Clock representing dates with a 1 second interval, counting from midnight,
	Jan 1st 1601. Provides standard type aliases, clock::duration and
	clock::time_point, as long as typical clock::now for getting current time
	point. However, this time_point, while connected to system_clock::now, will
	always be in GMT timezone.

	\see
	[std::chrono::system_clock](http://en.cppreference.com/w/cpp/chrono/system_clock)
	\see
	[std::chrono::seconds](http://en.cppreference.com/w/cpp/chrono/duration)
	*/
	struct clock {
		/**
		Clock's interval - one second.
		*/
		using duration = std::chrono::seconds;

		/**
		Clock's time point - standard time_point for this clock.
		\see
		[std::chrono::time_point](http://en.cppreference.com/w/cpp/chrono/time_point)
		*/
		using time_point = std::chrono::time_point<clock>;

		/**
		Returns a time point representing the current point in time.
		*/
		static time_point now();

		/**
		Returns a time point representing maximum possible point in
		time.
		*/
		static time_point max_epoch() {
			return time_point{time_point::duration::max()};
		}

		/**
		Creates a time point representing given datetime. If the date is
		invalid, max_epoch() is returned. \param time The date object to convert
		to time point \returns time_point representing the `time` parameter
		*/
		static time_point from_date(const datetime& time);

		/**
		Creates a date/time representing given time point.
		\param tp The time point to convert to date/time
		\returns datetime representing the `tp` parameter
		*/
		static datetime to_date(time_point tp);

		/**
		Creates a string representing given time point.
		The format is `"Wkd, DD Mth YYYY hh:mm:ss GMT"`.
		\param tp The time point to convert to string
		\returns a string representing the `tp` parameter
		*/
		static std::string to_string(time_point tp);
	};
}  // namespace tangle::cookie::chrono

namespace tangle::cookie {
	using clock = chrono::clock;  ///< The clock type moved directly into
	                              ///< cookie namespace
	using time_point = clock::time_point;  ///< Clock's time_point type moved
	                                       ///< directly into cookie namespace
}  // namespace tangle::cookie
