// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/nav/jar.h>

namespace tangle::nav {
	void jar::path(const std::string& path) {
		m_path = path;
		m_cookies.load(path);
	}

	void jar::store(cookie::time_point when) const {
		if (m_path.empty()) return;

		// TODO: log if failed
		m_cookies.store(m_path, when);
	}

	bool jar::append(const uri& address,
	                 std::string_view header,
	                 cookie::time_point created) {
		auto auth = address.parsed_authority();
		if (auth.host.empty()) return false;
		auto item = tangle::cookie::from_server(auth.host, header, created);
		if (item.scope().domain.empty()) return false;
		m_cookies.add(item);
		if ((item.flags() & cookie::flags::persistent) ==
		    cookie::flags::persistent) {
			// TODO: schedule the store instead of storing immediately
			store(created);
		}
		return true;
	}

	std::string jar::get(const uri& address,
	                     bool https,
	                     cookie::time_point when) {
		auto auth = address.parsed_authority();
		if (auth.host.empty()) return {};

		auto options = cookie::match::http;
		if (https) options |= cookie::match::secure;
		auto path = address.path();
		return m_cookies.str({auth.host, {path.data(), path.length()}}, options,
		                     when);
	}
}  // namespace tangle::nav
