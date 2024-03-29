// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cache content.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/nav/request.hpp>
#include <tangle/uri.hpp>
#include <functional>
#include <memory>

namespace tangle::nav {
	struct doc_backend;
	struct document {
		document();
		document(const document&);
		document& operator=(const document&);
		document(document&&);
		document& operator=(document&&);

		document open(uri const& loc,
		              uri::server_quirks quirks = uri::no_quirks);
		document open(request const& req);
		uri const& location() const noexcept;
		std::string const& text() const noexcept;
		std::string&& moveable_text() noexcept;
		int status() const noexcept;
		std::string const& status_text() const noexcept;
		int conn_status() const noexcept;
		std::string const& conn_status_text() const noexcept;
		// false, when file couldn't be opened, e.g. 4xx and 5xx on HTTP
		bool exists() const noexcept;
		// unsatisfied link, e.g. 3xx on HTTP
		bool is_link() const noexcept;
		nav::headers const& headers() const noexcept;

		static document wrap(std::shared_ptr<doc_backend> impl) {
			return document{std::move(impl)};
		}

	private:
		std::shared_ptr<doc_backend> m_impl;

		explicit document(std::shared_ptr<doc_backend> impl);
	};

	struct doc_backend {
		virtual ~doc_backend();
		virtual document open(uri const& loc, uri::server_quirks quirks);
		virtual document open(request const& req) = 0;
		virtual uri const& location() const noexcept = 0;
		virtual std::string const& text() const noexcept = 0;
		virtual std::string&& moveable_text() noexcept = 0;
		virtual int status() const noexcept = 0;
		virtual std::string const& status_text() const noexcept = 0;
		virtual int conn_status() const noexcept = 0;
		virtual std::string const& conn_status_text() const noexcept = 0;
		virtual bool exists() const noexcept = 0;
		virtual bool is_link() const noexcept = 0;
		virtual nav::headers const& headers() const noexcept = 0;
	};

}  // namespace tangle::nav
