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
Navigation cookie jar.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once
#include <tangle/uri.h>
#include <tangle/cookie/jar.h>

namespace tangle { namespace nav {

	/**
	Navigation cookie jar.

	Helper class ensuring a jar is stored and loaded.
	*/
	class jar {
	public:
		void path(const std::string& path);
		const std::string& path() const { return m_path; }
		void store(cookie::time_point when = cookie::clock::now()) const;
		bool append(const uri& address, const cstring& header, cookie::time_point created = cookie::clock::now());
		std::string get(const uri& address, bool https, cookie::time_point when = cookie::clock::now());
	private:
		std::string m_path;
		cookie::jar m_cookies;
	};
}}
