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
#include <tangle/nav/request.h>
#include <tangle/nav/cache.h>
#include <tangle/cookie/chrono.h>

namespace tangle { namespace nav {
	struct protocol;

	struct config {
		std::string app_version;
		std::string jar_file;
		std::string cache_dir;
		std::vector<std::string> languages;
	};

	class jar;
	struct navigator {
		struct backend;

		navigator(const config& cfg);
		navigator();
		~navigator();
		navigator(const navigator&);
		navigator& operator=(const navigator&);
		navigator(navigator&&);
		navigator& operator=(navigator&&);

		void reg_proto(const std::string& scheme,
			const std::shared_ptr<protocol>& proto);

		jar& cookies();
		const jar& cookies() const;
		nav::cache& cache();
		const nav::cache& cache() const;

		loader open(const request& req, bool refreshing,
			cookie::time_point when = cookie::clock::now());
	private:
		std::shared_ptr<backend> m_impl;
	};
}}
