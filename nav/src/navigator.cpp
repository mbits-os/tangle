// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <cctype>
#include <map>
#include <tangle/nav/jar.hpp>
#include <tangle/nav/navigator.hpp>
#include <tangle/nav/protocol.hpp>

namespace tangle::nav {
	request_trace::~request_trace() = default;

	struct navigator::backend {
		backend(const std::string& user_agent);
		~backend();

		void reg_proto(const std::string& scheme,
		               const std::shared_ptr<protocol>& proto);
		std::shared_ptr<protocol> get_proto(std::string_view scheme);

		const std::string& user_agent() const noexcept { return m_user_agent; }
		jar& cookies() noexcept { return m_jar; }
		const std::vector<std::string>& languages() const noexcept {
			return m_languages;
		}
		std::vector<std::string>& languages() noexcept { return m_languages; }

	private:
		std::string m_user_agent;
		jar m_jar;
		std::vector<std::string> m_languages;
		std::map<std::string, std::shared_ptr<protocol>, std::less<>>
		    m_protocols;
	};

	navigator::~navigator() = default;
	navigator::navigator(const navigator&) = default;
	navigator& navigator::operator=(const navigator&) = default;
	navigator::navigator(navigator&&) noexcept = default;
	navigator& navigator::operator=(navigator&&) noexcept = default;

	navigator::navigator()
	    : m_impl{std::make_shared<backend, std::string>({})} {}

	navigator::navigator(const config& cfg)
	    : m_impl{std::make_shared<backend>(cfg.app_version)} {
		cookies().path(cfg.jar_file);
		m_impl->languages() = cfg.languages;
	}

	void navigator::reg_proto(const std::string& key,
	                          const std::shared_ptr<protocol>& proto) {
		auto scheme = key;
		for (auto& c : scheme)
			c = static_cast<char>(std::tolower(static_cast<uint8_t>(c)));

		m_impl->reg_proto(scheme, proto);
	}

	const std::string& navigator::user_agent() const noexcept {
		return m_impl->user_agent();
	}
	jar& navigator::cookies() noexcept { return m_impl->cookies(); }

	const std::vector<std::string>& navigator::languages() const noexcept {
		return m_impl->languages();
	}

	document navigator::open(const request& req, cookie::time_point) {
		auto addr = req.address();

		if (!addr.has_authority()) return {};

		auto proto = m_impl->get_proto(addr.scheme());
		if (!proto) return {};

		return proto->open(req, *this);
	}

	navigator::backend::backend(const std::string& user_agent)
	    : m_user_agent{user_agent} {}

	navigator::backend::~backend() {}

	void navigator::backend::reg_proto(const std::string& scheme,
	                                   const std::shared_ptr<protocol>& proto) {
		// synchronize...
		if (!proto) {
			m_protocols.erase(scheme);
			return;
		}

		m_protocols[scheme] = proto;
	}

	std::shared_ptr<protocol> navigator::backend::get_proto(
	    std::string_view scheme) {
		// synchronize...
		auto it = m_protocols.find(scheme);
		if (it == m_protocols.end()) return {};
		return it->second;
	}
}  // namespace tangle::nav
