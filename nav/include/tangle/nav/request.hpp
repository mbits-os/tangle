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
#include <memory>
#include <tangle/uri.hpp>

namespace tangle::nav {
	enum class method { get, post };

	class request {
	public:
		request() = default;

		explicit request(nav::method mth) : m_method(mth) {}

		explicit request(const uri& address) : m_address(normalized(address)) {}

		request(nav::method mth, const uri& address)
		    : m_address(normalized(address)), m_method(mth) {}

		using meta_list = std::vector<std::pair<std::string, std::string>>;

		request& method(nav::method value) {
			m_method = value;
			return *this;
		}
		request& address(const uri& value) {
			if (m_referrer.empty())
				m_address = normalized(value);
			else
				m_address = normalized(value, m_referrer);
			return *this;
		}
		request& meta(meta_list&& hdrs) {
			m_meta = std::move(hdrs);
			return *this;
		}
		request& max_redir(int value) {
			m_max_redir = value;
			return *this;
		}
		request& referrer(const uri& value) {
			m_referrer = normalized(value);
			m_address = normalized(m_address, m_referrer);
			return *this;
		}
		request& custom_agent(std::string value) {
			m_custom_agent = std::move(value);
			return *this;
		}
		request& content_type(std::string value) {
			m_content_type = std::move(value);
			return *this;
		}
		request& content(std::string value) {
			m_content = std::move(value);
			return *this;
		}
		request& form_fields(std::string value) {
			m_form_fields = std::move(value);
			return *this;
		}

		const uri& address() const noexcept { return m_address; }
		meta_list const& meta() const noexcept { return m_meta; }
		nav::method method() const noexcept { return m_method; }
		bool follow_redirects() const noexcept { return m_max_redir > 0; }
		int max_redir() const noexcept { return m_max_redir; }
		const uri& referrer() const noexcept { return m_referrer; }
		const std::string& custom_agent() const noexcept {
			return m_custom_agent;
		}
		const std::string& content_type() const noexcept {
			return m_content_type;
		}
		const std::string& content() const noexcept { return m_content; }
		const std::string& form_fields() const noexcept {
			return m_form_fields;
		}

	private:
		uri normalized(uri const& input, uri const& doc) {
			return uri::canonical(input, uri::make_base(doc), uri::with_pass);
		}
		uri normalized(uri const& input) {
			return uri::normal(input, uri::with_pass);
		}
		uri m_address;
		nav::method m_method = nav::method::get;
		int m_max_redir = 10;
		uri m_referrer;
		std::string m_custom_agent;
		std::string m_content_type;
		std::string m_content;
		std::string m_form_fields;
		meta_list m_meta;
	};
}  // namespace tangle::nav
