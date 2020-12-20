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

#include <functional>
#include <memory>
#include <tangle/uri.h>

namespace tangle { namespace cache {
	struct loader;
	struct loader_impl {
		virtual ~loader_impl() {}
		virtual void on_opened(const std::function<bool(loader&)>&) = 0;
		virtual void on_data(const std::function<void(loader&, const void*, size_t)>&) = 0;

		virtual bool exists() const = 0;
		virtual bool is_link() const = 0;
	};

	struct document;
	struct doc_impl {
		virtual ~doc_impl() {}
		virtual document open(uri const& loc) = 0;
		virtual uri const& location() const noexcept = 0;
		virtual std::string const& text() const noexcept = 0;
		virtual std::string&& moveable_text() noexcept = 0;
		virtual int status() const noexcept = 0;
		virtual std::string const& status_text() const noexcept = 0;
		virtual bool exists() const noexcept = 0;
		virtual bool is_link() const noexcept = 0;
		virtual std::vector<std::pair<std::string, std::string>> const& headers()
			const noexcept = 0;
	};

}}
