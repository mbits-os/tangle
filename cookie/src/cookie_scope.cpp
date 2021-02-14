// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/scope.hpp>
#include <cctype>
#include <ctime>

namespace tangle::cookie {
	scope_type::scope_type() = default;
	scope_type::scope_type(const scope_type&) = default;
	scope_type::scope_type(scope_type&&) = default;
	scope_type& scope_type::operator=(const scope_type&) = default;
	scope_type& scope_type::operator=(scope_type&&) = default;

	scope_type::scope_type(const std::string& domain, const std::string& path)
	    : domain(domain), path(path) {}

	bool scope_type::matches_domain(const scope_type& upstream,
	                                bool host_only) const {
		if (upstream.domain.empty()) return false;

		auto ulen = upstream.domain.length();
		auto len = domain.length();

		if (ulen < len) return false;
		if (ulen == len) return upstream.domain == domain;
		if (host_only) return false;

		auto lower = ulen - len;

		if (domain.compare(upstream.domain.c_str() + lower) != 0) return false;
		return upstream.domain[lower - 1] == '.';
	}

	bool scope_type::matches_path(const scope_type& upstream) const {
		if (path.empty() || path == "/") return true;

		auto ulen = upstream.path.length();
		auto len = path.length();

		if (ulen < len) return false;
		if (ulen == len) return upstream.path == path;
		if (path.compare(0, len, upstream.path.c_str(), len) != 0) return false;
		return path[len - 1] == '/' || upstream.path[len] == '/';
	}
}  // namespace tangle::cookie
