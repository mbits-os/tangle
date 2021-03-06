// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Navigation cache.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

Cache contains both the data already downloaded and still fresh
and the data to be downloaded. Any clases wishing to get any data
over any wire, should do so over the cache.
*/

#pragma once
#include <tangle/cookie/chrono.hpp>
#include <tangle/nav/document.hpp>
#include <tangle/nav/request.hpp>
#include <memory>

namespace tangle::nav {
	struct protocol;

	struct config {
		std::string app_version{};
		std::string jar_file{};
		std::vector<std::string> languages{};
	};

	class jar;
	struct navigator {
		struct backend;

		navigator(const config& cfg);
		navigator();
		~navigator();
		navigator(const navigator&);
		navigator& operator=(const navigator&);
		navigator(navigator&&) noexcept;
		navigator& operator=(navigator&&) noexcept;

		void reg_proto(const std::string& scheme,
		               const std::shared_ptr<protocol>& proto);

		const std::string& user_agent() const noexcept;
		jar& cookies() noexcept;
		const std::vector<std::string>& languages() const noexcept;

		document open(const request& req,
		              cookie::time_point when = cookie::clock::now());

	private:
		std::shared_ptr<backend> m_impl;
	};
}  // namespace tangle::nav
