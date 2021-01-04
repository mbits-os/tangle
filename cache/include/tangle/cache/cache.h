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
#include <tangle/cache/loader.h>
#include <tangle/cookie/chrono.h>
#include <tangle/uri.h>
#include <memory>

namespace tangle::cache {
	/**
	Navigation cache.

	Cache contains the data already downloaded or currently being downloaded.
	*/
	class cache {
	public:
		class file {
		public:
			file();
			bool is_active() const;
			bool is_fresh(cookie::time_point when = cookie::clock::now()) const;
			loader get_loader();
			std::string meta(const std::string& key);
		};

		virtual ~cache() {}
		virtual bool storage_backed() const noexcept = 0;
		virtual std::shared_ptr<file> get(const uri& address) = 0;
		virtual std::shared_ptr<file> create(const uri& address) = 0;
	};

}  // namespace tangle::cache
