// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cache content.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <functional>
#include <memory>
#include <tangle/uri.hpp>

namespace tangle::cache {
	struct loader;
	struct loader_impl {
		virtual ~loader_impl() {}
		virtual void on_opened(const std::function<bool(loader&)>&) = 0;
		virtual void on_data(
		    const std::function<void(loader&, const void*, size_t)>&) = 0;

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
		virtual std::vector<std::pair<std::string, std::string>> const&
		headers() const noexcept = 0;
	};

}  // namespace tangle::cache
