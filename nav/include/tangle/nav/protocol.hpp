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
#include <tangle/nav/document.hpp>
#include <tangle/nav/jar.hpp>
#include <tangle/nav/request.hpp>
#include <memory>
#include <unordered_map>

namespace tangle::nav {
	struct navigator;
	struct protocol {
		virtual ~protocol() {}
		virtual document open(const request& req, navigator& nav) = 0;
	};
}  // namespace tangle::nav
