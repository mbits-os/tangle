// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Navigation HTTP document backend.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/cookie/item.hpp>
#include <tangle/msg/http_parser.hpp>
#include <tangle/nav/document.hpp>
#include <tangle/nav/jar.hpp>
#include <tangle/nav/navigator.hpp>
#include <tangle/uri.hpp>

namespace tangle::nav {
	struct doc_impl : doc_backend {
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

		doc_impl(uri const& location, navigator* nav);
		void on_library_error(int code, char const* msg);
		size_t on_data(const void* data, size_t count);
		void on_final_location(const std::string& location);
		void on_headers(std::string const& status_text,
		                int status,
		                msg::http_response::dict_t const& headers);
		using doc_backend::open;
		document open(request const& req) override;
		uri const& location() const noexcept override;
		std::string const& text() const noexcept override;
		std::string&& moveable_text() noexcept override;
		int status() const noexcept override;
		std::string const& status_text() const noexcept override;
		int conn_status() const noexcept override;
		std::string const& conn_status_text() const noexcept override;
		bool exists() const noexcept override;
		bool is_link() const noexcept override;
		nav::headers const& headers() const noexcept override;

	private:
		uri location_{};
		std::string text_{};
		int status_{0};
		int conn_status_{0};
		std::string status_text_{};
		std::string conn_status_text_{};
		navigator* nav_{};
		nav::headers headers_{};
	};
}  // namespace tangle::nav
