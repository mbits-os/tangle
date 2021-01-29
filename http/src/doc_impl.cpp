// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/http/doc_impl.hpp>

namespace tangle::http {
	doc_impl::doc_impl(uri const& location, nav::navigator* nav)
	    : location_{location}, nav_{nav} {}

	void doc_impl::on_library_error(int code, char const* msg) {
		conn_status_ = code;
		conn_status_text_ = msg;
	}

	size_t doc_impl::on_data(const void* data, size_t count) {
		text_.append(static_cast<char const*>(data), count);
		return count;
	}

	void doc_impl::on_final_location(const std::string& location) {
		location_ = location;
	}

	void doc_impl::on_headers(std::string const& status_text,
	                          int status,
	                          msg::http_response::dict_t const& headers) {
		status_ = status;
		status_text_ = status_text;
		auto it = headers.find("set-cookie");
		if (it != headers.end()) {
			auto& jar = nav_->cookies();
			auto created = cookie::chrono::clock::now();
			for (auto& header : it->second) {
				jar.append(location_, header, created);
			}
		}

		headers_.clear();

		for (auto const& [hdr, values] : headers) {
			auto header = nav::header_key::make(hdr.str());
			for (auto const& value : values) {
				headers_.add(header, value);
			}
		}
	}

	nav::document doc_impl::open(nav::request const& req) {
		if (!exists()) return {};
		if (req.referrer().empty()) {
			auto copy = req;
			copy.referrer(location_);
			return nav_->open(copy);
		}
		return nav_->open(req);
	}

	uri const& doc_impl::location() const noexcept { return location_; }

	std::string const& doc_impl::text() const noexcept { return text_; }

	std::string&& doc_impl::moveable_text() noexcept {
		return std::move(text_);
	}

	int doc_impl::status() const noexcept { return status_; }

	std::string const& doc_impl::status_text() const noexcept {
		return status_text_;
	}

	int doc_impl::conn_status() const noexcept { return conn_status_; }

	std::string const& doc_impl::conn_status_text() const noexcept {
		return conn_status_text_;
	}

	bool doc_impl::exists() const noexcept { return (status_ / 100) == 2; }

	bool doc_impl::is_link() const noexcept { return is_redirect(status_); }

	nav::headers const& doc_impl::headers() const noexcept { return headers_; }
}  // namespace tangle::http
