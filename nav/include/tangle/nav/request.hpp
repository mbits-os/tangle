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
#include <tangle/nav/headers.hpp>
#include <tangle/uri.hpp>

namespace tangle::nav {
	enum class method { get, post };

	struct request_trace {
		virtual ~request_trace();
		virtual void message(std::string_view text) = 0;
		virtual void req_header(std::string_view text) = 0;
		virtual void resp_header(std::string_view text) = 0;
		virtual void req_data(std::string_view text) = 0;
		virtual void resp_data(std::string_view text) = 0;
		virtual void ssl_data_in(std::string_view text) = 0;
		virtual void ssl_data_out(std::string_view text) = 0;
	};

	class request {
	public:
		request() = default;

		explicit request(nav::method mth, nav::headers&& hdrs = {})
		    : m_method(mth), m_headers(std::move(hdrs)) {}

		explicit request(const uri& address, nav::headers&& hdrs = {})
		    : m_address(normalized(address)), m_headers(std::move(hdrs)) {}

		request(nav::method mth, const uri& address, nav::headers&& hdrs = {})
		    : m_address(normalized(address))
		    , m_method(mth)
		    , m_headers(std::move(hdrs)) {}

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
		request& add(header_key const& header, std::string const& value) {
			m_headers.add(header, value);
			return *this;
		}
		request& add(header_key const& header, std::string&& value) {
			m_headers.add(header, std::move(value));
			return *this;
		}
		request& set(header_key const& header, std::string const& value) {
			m_headers.set(header, value);
			return *this;
		}
		request& set(header_key const& header, std::string&& value) {
			m_headers.set(header, std::move(value));
			return *this;
		}
		void remove(header_key const& key) noexcept { m_headers.remove(key); }
		void clear_headers() noexcept { m_headers.clear(); }
		request& referrer(uri const& value);
		request& basic_auth(std::string const& username,
		                    std::string const& secret);
		request& custom_agent(std::string const& value) {
			return set(header::User_Agent, value);
		}
		request& content_type(std::string const& value) {
			return set(header::Content_Type, value);
		}
		request& content(std::string value) {
			m_content = std::move(value);
			return *this;
		}
		request& form_fields(std::string value) {
			m_form_fields = std::move(value);
			return *this;
		}
		request& max_redir(int value) {
			m_max_redir = value;
			return *this;
		}
		request& trace(std::shared_ptr<request_trace> callback) {
			m_trace = callback;
			return *this;
		}

		const uri& address() const noexcept { return m_address; }
		nav::headers const& headers() const noexcept { return m_headers; }
		nav::method method() const noexcept { return m_method; }
		bool follow_redirects() const noexcept { return m_max_redir > 0; }
		int max_redir() const noexcept { return m_max_redir; }
		const uri& referrer() const noexcept { return m_referrer; }
		const std::string& content() const noexcept { return m_content; }
		const std::string& form_fields() const noexcept {
			return m_form_fields;
		}
		std::shared_ptr<request_trace> trace() const { return m_trace; }

	private:
		uri normalized(uri const& input, uri const& doc) {
			return uri::canonical(input, uri::make_base(doc), uri::with_pass);
		}
		uri normalized(uri const& input) {
			return uri::normal(input, uri::with_pass);
		}
		uri m_address{};
		uri m_referrer{};
		nav::method m_method = nav::method::get;
		int m_max_redir = 50;
		std::string m_content{};
		std::string m_form_fields{};
		nav::headers m_headers;
		std::shared_ptr<request_trace> m_trace{};
	};
}  // namespace tangle::nav
