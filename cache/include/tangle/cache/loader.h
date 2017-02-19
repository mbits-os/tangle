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
Cache content.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/uri.h>
#include <functional>
#include <memory>

namespace tangle { namespace cache {
	struct loader_impl;
	/**
	Content loader.

	Content interface for cache object clients.
	*/
	struct loader {
		loader();
		loader(const loader&);
		loader& operator=(const loader&);
		loader(loader&&);
		loader& operator=(loader&&);

		loader& on_opened(const std::function<bool(loader&)>&);
		loader& on_data(const std::function<void(loader&, const void*, size_t)>&);

		bool exists() const; // file couldn't be opened, e.g. 4xx and 5xx on HTTP
		bool is_link() const; // unsatisfied link, e.g. 3xx on HTTP

		static loader wrap(std::shared_ptr<loader_impl> impl) {
			return loader{ std::move(impl) };
		}

	private:
		std::shared_ptr<loader_impl> m_impl;

		explicit loader(std::shared_ptr<loader_impl> impl);
	};
}}
