// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/nav/headers.hpp>
#include <tangle/nav/navigator.hpp>

namespace tangle::http {
	class message {
	public:
		nav::headers const& headers() const noexcept { return headers_; }

	protected:
		message() = default;
		message(nav::headers&& hdrs) : headers_{std::move(hdrs)} {}
		message(nav::headers const& hdrs) : headers_{hdrs} {}

		nav::headers headers_;
	};
}  // namespace tangle::http

namespace tangle::http::client {
	class request : public message {
	public:
		using header_key = nav::header_key;

		request() = default;
		request(uri const& address) : address_{address} {}

		uri const& address() const noexcept { return address_; }

		request& add(header_key const& header, std::string const& value) {
			headers_.add(header, value);
			return *this;
		}

		request& add(header_key const& header, std::string&& value) {
			headers_.add(header, std::move(value));
			return *this;
		}

		request& set(header_key const& header, std::string const& value) {
			headers_.set(header, value);
			return *this;
		}

		request& set(header_key const& header, std::string&& value) {
			headers_.set(header, std::move(value));
			return *this;
		}

		request& set_basic_auth(std::string const& username,
		                        std::string const& secret);

		void erase(header_key const& key) noexcept { headers_.erase(key); }

	private:
		uri address_{};
	};

	class response : public message {
	public:
		response() = default;
		response(int status,
		         uri const& location,
		         nav::headers&& hdrs,
		         std::string&& text)
		    : message{std::move(hdrs)}
		    , status_{status}
		    , location_{location}
		    , text_{std::move(text)} {}

		int status() const noexcept { return status_; }
		uri const& location() const noexcept { return location_; }
		std::string const& text() const noexcept { return text_; }

	private:
		int status_{};
		uri location_{};
		std::string text_{};
	};
}  // namespace tangle::http::client