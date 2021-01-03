/*
 * Copyright (C) 2017 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <tangle/nav/jar.h>

namespace tangle { namespace nav {
	void jar::path(const std::string& path)
	{
		m_path = path;
		m_cookies.load(path);
	}

	void jar::store(cookie::time_point when) const
	{
		if (m_path.empty())
			return;

		// TODO: log if failed
		m_cookies.store(m_path, when);
	}

	bool jar::append(const uri& address, std::string_view header, cookie::time_point created)
	{
		auto auth = address.parsed_authority();
		if (auth.host.empty())
			return false;
		auto item = tangle::cookie::from_server(auth.host, header, created);
		if (item.scope().domain().empty())
			return false;
		m_cookies.add(item);
		if ((item.flags() & cookie::flags::persistent) == cookie::flags::persistent) {
			//TODO: schedule the store instead of storing immediately
			store(created);
		}
		return true;
	}

	std::string jar::get(const uri& address, bool https, cookie::time_point when)
	{
		auto auth = address.parsed_authority();
		if (auth.host.empty())
			return { };

		auto options = cookie::match::http;
		if (https)
			options |= cookie::match::secure;
		auto path = address.path();
		return m_cookies.str({ auth.host, { path.data(), path.length() } }, options, when);
	}
}}
