// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/nav/document.hpp>

namespace tangle::nav {
	document::document(std::shared_ptr<doc_backend> impl) : m_impl(impl) {}

	document::document() = default;
	document::document(const document&) = default;
	document& document::operator=(const document&) = default;
	document::document(document&&) = default;
	document& document::operator=(document&&) = default;

	document document::open(uri const& loc) {
		if (!m_impl) return {};
		return m_impl->open(loc);
	}

	document document::open(request const& req) {
		if (!m_impl) return {};
		return m_impl->open(req);
	}

	uri const& document::location() const noexcept {
		static uri const empty;
		if (!m_impl) return empty;
		return m_impl->location();
	}

	std::string const& document::text() const noexcept {
		static std::string const empty;
		if (!m_impl) return empty;
		return m_impl->text();
	}

	std::string&& document::moveable_text() noexcept {
		static std::string empty;
		if (!m_impl) return std::move(empty);
		return m_impl->moveable_text();
	}

	int document::status() const noexcept {
		if (!m_impl) return false;
		return m_impl->status();
	}

	std::string const& document::status_text() const noexcept {
		static std::string empty;
		if (!m_impl) return empty;
		return m_impl->status_text();
	}

	int document::conn_status() const noexcept {
		if (!m_impl) return false;
		return m_impl->conn_status();
	}

	std::string const& document::conn_status_text() const noexcept {
		static std::string empty;
		if (!m_impl) return empty;
		return m_impl->conn_status_text();
	}

	bool document::exists() const noexcept {
		if (!m_impl) return false;
		return m_impl->exists();
	}

	bool document::is_link() const noexcept {
		if (!m_impl) return false;
		return m_impl->is_link();
	}

	nav::headers const& document::headers() const noexcept {
		static nav::headers const empty{};
		if (!m_impl) return empty;
		return m_impl->headers();
	}

	doc_backend::~doc_backend() = default;

	document doc_backend::open(uri const& loc) {
		if (!exists()) return {};
		return open(request{loc}.referrer(location()));
	}
}  // namespace tangle::nav
