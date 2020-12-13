/*
 * Copyright (C) 2020 midnightBITS
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
Navigation HTTP document backend.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/cache/impl/loader_impl.h>
#include <tangle/uri.h>
#include <tangle/nav/navigator.h>
#include <tangle/msg/http_parser.h>
#include <tangle/cookie/item.h>

namespace tangle { namespace nav {
	struct http_doc_impl : cache::doc_impl
	{
		static inline bool is_redirect(int status)
		{
			/*
			 * 01
			 * 02 301 Moved Permanently
			 * 04 302 Found
			 * 08 303 See Other
			 * 10
			 * 20
			 * 40
			 * 80 307 Temporary Redirect
			 */
			static int codes = 0x8E;
			static int max_redirect = 7;

#define BIT_POS (status % 100)
#define BIT (1 << BIT_POS)
			return status / 100 == 3 ? (BIT_POS <= max_redirect ? (BIT & codes) == BIT : false) : false;
		}

		http_doc_impl(uri const& location, navigator* nav) : location_{ location }, nav_{ nav } {}

		void on_error() { status_ = 999; }

		size_t on_data(const void* data, size_t count) {
			text_.append(static_cast<char const*>(data), count);
			return count;
		}

		void on_final_location(const std::string& location) { location_ = location; }
		void on_headers(std::string const&, int status, msg::http_response::dict_t const& headers) {
			status_ = status;
			auto it = headers.find("set-cookie");
			if (it != headers.end()) {
				auto& jar = nav_->cookies();
				auto created = cookie::chrono::clock::now();
				for (auto& header : it->second) {
					jar.append(location_, header, created);
				}
			}
		}

		cache::document open(uri const& loc) override {
			if (!exists())
				return {};
			return nav_->open(nav::request{ loc }.referrer(location_));
		}

		/*cache::document post_form(uri const& loc, std::vector<std::pair<std::string, std::string>> const& fields) override {
			if (!exists())
				return {};
			auto addr = uri::canonical(loc, location_, uri::with_pass);

			uri::query_builder query{};
			for (auto& [name, value] : fields)
				query.add(name, value);

			return nav_->open(
				nav::request{ nav::method::post, addr }
				.referrer(location_)
				.content_type("application/x-www-form-urlencoded")
				.content(query.string(uri::form_urlencoded))
			);
		}*/

		uri const& location() const override { return location_; }
		std::string const& text() const override { return text_; }
		int status() const override { return status_; }

		bool exists() const override {
			return (status_ / 100) == 2;
		}
		bool is_link() const override { return is_redirect(status_); }
	private:
		uri location_;
		std::string text_;
		int status_{ 0 };
		nav::navigator* nav_;
	};
}}
