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

/**
Navigation cache.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

Cache contains both the data already downloaded and still fresh
and the data to be downloaded. Any clases wishing to get any data
over any wire, should do so over the cache.
*/

#pragma once
#include <memory>
#include <tangle/uri.h>

namespace tangle { namespace nav {
	enum class method {
		get,
		post
	};

	class request {
	public:
		request() = default;

		explicit request(nav::method mth)
			: m_method(mth)
		{
		}

		explicit request(const uri& address)
			: m_address(address)
		{
		}

		explicit request(uri&& address)
			: m_address(std::move(address))
		{
		}

		request(nav::method mth, const uri& address)
			: m_address(address)
			, m_method(mth)
		{
		}

		request(nav::method mth, const std::string& address)
			: m_address(address)
			, m_method(mth)
		{
		}

		request(nav::method mth, std::string_view address)
			: m_address(address)
			, m_method(mth)
		{
		}

		request(nav::method mth, uri&& address)
			: m_address(std::move(address))
			, m_method(mth)
		{
		}

		request(nav::method mth, std::string&& address)
			: m_address(std::move(address))
			, m_method(mth)
		{
		}

		request& method(nav::method value) { m_method = value; return *this; }
		request& address(const uri& value) { m_address = value; return *this; }
		request& address(uri&& value) { m_address = std::move(value); return *this; }
		request& follow_redirects(bool value) { m_follow_redirects = value; return *this; }
		request& max_redir(int value) { m_max_redir = value; return *this; }
		request& referrer(const std::string& value) { m_referrer = value; return *this; }
		request& referrer(std::string&& value) { m_referrer = std::move(value); return *this; }
		request& custom_agent(const std::string& value) { m_custom_agent = value; return *this; }
		request& custom_agent(std::string&& value) { m_custom_agent = std::move(value); return *this; }

		const uri& address() const { return m_address; }
		nav::method method() const { return m_method; }
		bool follow_redirects() const { return m_follow_redirects; }
		int max_redir() const { return m_max_redir; }
		const std::string& referrer() const { return m_referrer; }
		const std::string& custom_agent() const { return m_custom_agent; }
	private:
		uri m_address;
		nav::method m_method = nav::method::get;
		bool m_follow_redirects = true;
		int m_max_redir = 10;
		std::string m_referrer;
		std::string m_custom_agent;
	};
}}
