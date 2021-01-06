// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cache/impl/loader_impl.hpp>
#include <tangle/cache/loader.hpp>

namespace tangle::cache {
	loader::loader(std::shared_ptr<loader_impl> impl) : m_impl(impl) {}

	loader::loader() = default;
	loader::loader(const loader&) = default;
	loader& loader::operator=(const loader&) = default;
	loader::loader(loader&&) = default;
	loader& loader::operator=(loader&&) = default;

	loader& loader::on_opened(const std::function<bool(loader&)>&) {
		return *this;
	}
	loader& loader::on_data(
	    const std::function<void(loader&, const void*, size_t)>&) {
		return *this;
	}

	bool loader::exists() const {
		if (!m_impl) return false;
		return m_impl->exists();
	}

	bool loader::is_link() const {
		if (!m_impl) return false;
		return m_impl->is_link();
	}

	document::document(std::shared_ptr<doc_impl> impl) : m_impl(impl) {}

	document::document() = default;
	document::document(const document&) = default;
	document& document::operator=(const document&) = default;
	document::document(document&&) = default;
	document& document::operator=(document&&) = default;

	document document::open(uri const& loc) {
		if (!m_impl) return {};
		return m_impl->open(loc);
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

	bool document::exists() const noexcept {
		if (!m_impl) return false;
		return m_impl->exists();
	}

	bool document::is_link() const noexcept {
		if (!m_impl) return false;
		return m_impl->is_link();
	}

	std::vector<std::pair<std::string, std::string>> const& document::headers()
	    const noexcept {
		static std::vector<std::pair<std::string, std::string>> const empty{};
		if (!m_impl) return empty;
		return m_impl->headers();
	}
}  // namespace tangle::cache
