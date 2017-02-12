/*
 * Copyright (C) 2017 midnightBITS
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
Navigation cache.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

Cache contains both the data already downloaded and still fresh
and the data to be downloaded. Any clases wishing to get any data
over any wire, should do so over the cache.
*/

#pragma once
#include <memory>
#include <tangle/uri.h>
#include <tangle/nav/loader.h>
#include <tangle/cookie/chrono.h>

namespace tangle { namespace nav {
	/**
	Navigation cache.

	Cache contains the data already downloaded ar currently being downloaded.
	*/
	class cache {
	public:
		class file {
		public:
			file();
			bool is_active() const;
			bool is_fresh(cookie::time_point when = cookie::clock::now()) const;
			loader get_loader();
			std::string meta(const std::string& key);
		};

		virtual ~cache() {}
		virtual std::shared_ptr<file> get(const uri& address) = 0;
		virtual std::shared_ptr<file> create(const uri& address) = 0;
	};

}}
