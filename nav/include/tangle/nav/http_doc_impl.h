// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Navigation HTTP document backend.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/cache/impl/loader_impl.h>
#include <tangle/cookie/item.h>
#include <tangle/msg/http_parser.h>
#include <tangle/nav/jar.h>
#include <tangle/nav/navigator.h>
#include <tangle/uri.h>

namespace tangle::nav {
	struct http_doc_impl : cache::doc_impl {
		static inline bool is_redirect(int status) {
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
			return status / 100 == 3
			           ? (BIT_POS <= max_redirect ? (BIT & codes) == BIT
			                                      : false)
			           : false;
		}

		http_doc_impl(uri const& location, navigator* nav)
		    : location_{location}, nav_{nav} {}

		void on_error(CURLcode code) {
			status_ = 1000 + code;
			status_text_ = curl_easy_strerror(code);
		}

		size_t on_data(const void* data, size_t count) {
			text_.append(static_cast<char const*>(data), count);
			return count;
		}

		void on_final_location(const std::string& location) {
			location_ = location;
		}

		void on_headers(std::string const& status_text,
		                int status,
		                msg::http_response::dict_t const& headers) {
			status_ = status;
			status_text_ = status_text;
			auto it = headers.find("set-cookie");
			if (it != headers.end()) {
				auto& jar = nav_->cookies();
				auto created = cookie::chrono::clock::now();
				for (auto& header : it->second) {
					jar.append(location_, header, created);
				}
			}

			size_t count = 0;
			for (auto const& pair : headers)
				count += pair.second.size();

			headers_.clear();
			headers_.reserve(count);

			for (auto const& [hdr, values] : headers) {
				std::string header = hdr.str();
				for (auto const& value : values) {
					headers_.emplace_back(header, value);
				}
			}
		}

		cache::document open(uri const& loc) override {
			if (!exists()) return {};
			return nav_->open(nav::request{loc}.referrer(location_));
		}

		uri const& location() const noexcept override { return location_; }
		std::string const& text() const noexcept override { return text_; }
		std::string&& moveable_text() noexcept override {
			return std::move(text_);
		}
		int status() const noexcept override { return status_; }
		std::string const& status_text() const noexcept override {
			return status_text_;
		}

		bool exists() const noexcept override { return (status_ / 100) == 2; }
		bool is_link() const noexcept override { return is_redirect(status_); }
		std::vector<std::pair<std::string, std::string>> const& headers()
		    const noexcept override {
			return headers_;
		}

	private:
		uri location_{};
		std::string text_{};
		int status_{0};
		std::string status_text_{};
		nav::navigator* nav_{};
		std::vector<std::pair<std::string, std::string>> headers_{};
	};
}  // namespace tangle::nav
