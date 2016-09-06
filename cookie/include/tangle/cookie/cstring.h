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

/**
Non-owning string.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

This class implements the cookie::cstring type, an iterable non-mutable
string of characters.
*/

#pragma once

#include <iostream>
#include <string>

namespace net { namespace cookie {
	class cstring {
	public:
		using size_type = size_t;
		using reference = const char&;
		using pointer = const char*;
		using const_iterator = pointer;

		static const size_type dynamic_range = -1;

		constexpr cstring() noexcept
			: m_data(nullptr)
			, m_size(0)
		{
		}

		constexpr cstring(const char* d, size_type s) noexcept
			: m_data(d)
			, m_size(s)
		{
		}

		template <size_type s>
		constexpr cstring(const char (&d)[s]) noexcept
			: m_data(d)
			, m_size(s - 1)
		{
		}

		cstring(const std::string& s) noexcept
			: m_data(s.data())
			, m_size(s.length())
		{
		}

		constexpr cstring subspan(size_type offset, size_type count) const noexcept
		{
			return { data() + offset, count };
		}

		constexpr cstring subspan(size_type offset) const noexcept
		{
			return { data() + offset, length() - offset };
		}

		constexpr cstring first(size_type count) const noexcept { return subspan(0, count); }
		constexpr cstring last(size_type count) const noexcept { return subspan(length() - count, count); }

		constexpr bool empty() const noexcept { return !m_size; }
		constexpr const char* data() const noexcept { return m_data; }
		constexpr size_type size() const noexcept { return m_size; }
		constexpr size_type length() const noexcept { return m_size; }
		constexpr reference operator[](size_type ndx) const noexcept { return m_data[ndx]; }

		constexpr const_iterator begin() const noexcept { return m_data; }
		constexpr const_iterator end() const noexcept { return m_data + m_size; }
	private:
		const char* m_data;
		size_type m_size;
	};

	inline std::string to_string(const cstring& h)
	{
		return { h.data(), h.length() };
	}

	inline std::ostream& operator<<(std::ostream& o, const cstring& s)
	{
		std::ios_base::iostate state = std::ios_base::goodbit;

		const std::ostream::sentry ok(o);
		if (!ok) state |= std::ios_base::badbit;

		if (state == std::ios_base::goodbit &&
			o.rdbuf()->sputn(s.data(), s.length()) != s.length()) {
			state |= std::ios_base::badbit;
		};

		o.setstate(state);
		return o;
	}

}}
