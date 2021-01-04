// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cache content.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once

#include <tangle/uri.h>
#include <functional>
#include <memory>

namespace tangle::cache {
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
		loader& on_data(
		    const std::function<void(loader&, const void*, size_t)>&);

		// false, when file couldn't be opened, e.g. 4xx and 5xx on HTTP
		bool exists() const;
		// unsatisfied link, e.g. 3xx on HTTP
		bool is_link() const;

		static loader wrap(std::shared_ptr<loader_impl> impl) {
			return loader{std::move(impl)};
		}

	private:
		std::shared_ptr<loader_impl> m_impl;

		explicit loader(std::shared_ptr<loader_impl> impl);
	};

	struct doc_impl;
	struct document {
		document();
		document(const document&);
		document& operator=(const document&);
		document(document&&);
		document& operator=(document&&);

		document open(uri const& loc);
		uri const& location() const noexcept;
		std::string const& text() const noexcept;
		std::string&& moveable_text() noexcept;
		int status() const noexcept;
		std::string const& status_text() const noexcept;
		// false, when file couldn't be opened, e.g. 4xx and 5xx on HTTP
		bool exists() const noexcept;
		// unsatisfied link, e.g. 3xx on HTTP
		bool is_link() const noexcept;
		std::vector<std::pair<std::string, std::string>> const& headers()
		    const noexcept;

		static document wrap(std::shared_ptr<doc_impl> impl) {
			return document{std::move(impl)};
		}

	private:
		std::shared_ptr<doc_impl> m_impl;

		explicit document(std::shared_ptr<doc_impl> impl);
	};
}  // namespace tangle::cache
