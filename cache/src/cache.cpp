// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cache/cache.h>
#include <tangle/cache/loader.h>

namespace tangle::cache {
	cache::file::file() = default;
	bool cache::file::is_active() const { return false; }
	bool cache::file::is_fresh(cookie::time_point when) const { return false; }
	std::string cache::file::meta(const std::string& key) { return {}; }
	loader cache::file::get_loader() { return {}; }
}  // namespace tangle::cache
