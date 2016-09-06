/*
 * Copyright (C) 2016 midnightBITS
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

#include <tangle/cookie/scope.h>
#include <cctype>
#include <ctime>

namespace net { namespace cookie {
	scope_type::scope_type() = default;
	scope_type::scope_type(const scope_type&) = default;
	scope_type::scope_type(scope_type&&) = default;
	scope_type& scope_type::operator=(const scope_type&) = default;
	scope_type& scope_type::operator=(scope_type&&) = default;

	scope_type::scope_type(const std::string& domain, const std::string& path)
		: m_domain(domain)
		, m_path(path)
	{
	}

	bool scope_type::matches_domain(const scope_type& upstream, bool host_only) const
	{
		if (upstream.m_domain.empty())
			return false;

		auto ulen = upstream.m_domain.length();
		auto len = m_domain.length();
		if (ulen < len)
			return false;

		if (ulen == len)
			return upstream.m_domain == m_domain;

		if (host_only)
			return false;

		auto lower = ulen - len;
		if (m_domain.compare(upstream.m_domain.c_str() + lower) != 0)
			return false;

		return upstream.m_domain[lower - 1] == '.';
	}

	bool scope_type::matches_path(const scope_type& upstream) const
	{
		if (m_path.empty() || m_path == "/")
			return true;

		auto ulen = upstream.m_path.length();
		auto len = m_path.length();
		if (ulen < len)
			return false;

		if (ulen < len)
			return false;

		if (ulen == len)
			return upstream.m_path == m_path;

		if (m_path.compare(0, len, upstream.m_path.c_str(), len) != 0)
			return false;
		return m_path[len - 1] == '/' || upstream.m_path[len] == '/';
	}
}}
