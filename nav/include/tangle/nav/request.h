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
			: m_address(normalized(address))
		{
		}

		request(nav::method mth, const uri& address)
			: m_address(normalized(address))
			, m_method(mth)
		{
		}

		using header_list = std::vector<std::pair<std::string, std::string>>;

		request& method(nav::method value) { m_method = value; return *this; }
		request& address(const uri& value) { m_address = normalized(value); return *this; }
		request& headers(header_list const& hdrs) { m_headers = hdrs; return *this; }
		request& headers(header_list&& hdrs) { m_headers = std::move(hdrs); return *this; }
		request& follow_redirects(bool value) { m_follow_redirects = value; return *this; }
		request& max_redir(int value) { m_max_redir = value; return *this; }
		request& referrer(const uri& value) { m_referrer = normalized(value); m_address = normalized(m_address, m_referrer); return *this; }
		request& custom_agent(std::string value) { m_custom_agent = std::move(value); return *this; }
		request& content_type(std::string value) { m_content_type = std::move(value); return *this; }
		request& content(std::string value) { m_content = std::move(value); return *this; }
		request& form_fields(std::string value) { m_form_fields = std::move(value); return *this; }

		const uri& address() const noexcept { return m_address; }
		header_list const& headers() const noexcept { return m_headers; }
		nav::method method() const noexcept { return m_method; }
		bool follow_redirects() const noexcept { return m_follow_redirects; }
		int max_redir() const noexcept { return m_max_redir; }
		const uri& referrer() const noexcept { return m_referrer; }
		const std::string& custom_agent() const noexcept { return m_custom_agent; }
		const std::string& content_type() const noexcept { return m_content_type; }
		const std::string& content() const noexcept { return m_content; }
		const std::string& form_fields() const noexcept { return m_form_fields; }
	private:
		uri normalized(uri const& input, uri const& doc) {
			return uri::canonical(input, uri::make_base(doc), uri::with_pass);
			return uri::normal(input, uri::with_pass);
		}
		uri normalized(uri const& input) {
			return uri::normal(input, uri::with_pass);
		}
		uri m_address;
		nav::method m_method = nav::method::get;
		bool m_follow_redirects = true;
		int m_max_redir = 10;
		uri m_referrer;
		std::string m_custom_agent;
		std::string m_content_type;
		std::string m_content;
		std::string m_form_fields;
		header_list m_headers;
	};
}}
