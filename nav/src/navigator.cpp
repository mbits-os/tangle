// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cache/cache.h>
#include <tangle/nav/jar.h>
#include <tangle/nav/navigator.h>
#include <tangle/nav/protocol.h>
#include <cctype>
#include <map>

namespace tangle::nav {
	namespace impl {
		struct empty_cache : tangle::cache::cache {
			bool storage_backed() const noexcept override { return false; }
			std::shared_ptr<file> get(const uri&) override {
				return {};
			}

			std::shared_ptr<file> create(const uri&) override {
				return {};
			}
		};

		struct dummy_cache : tangle::cache::cache {
			bool storage_backed() const noexcept override { return true; }
			std::shared_ptr<file> get(const uri&) override {
				return {};
			}

			std::shared_ptr<file> create(const uri&) override {
				return {};
			}
		};
	}  // namespace impl

	struct navigator::backend {
		backend(const std::string& user_agent);
		~backend();

		void set_cache(std::unique_ptr<tangle::cache::cache> cache) {
			m_cache = std::move(cache);
		}

		void reg_proto(const std::string& scheme,
		               const std::shared_ptr<protocol>& proto);
		std::shared_ptr<protocol> get_proto(std::string_view scheme);

		const std::string& user_agent() const noexcept { return m_user_agent; }
		jar& cookies() noexcept { return m_jar; }
		const jar& cookies() const noexcept { return m_jar; }
		tangle::cache::cache& cache() noexcept { return *m_cache; }
		const tangle::cache::cache& cache() const noexcept { return *m_cache; }
		const std::vector<std::string>& languages() const noexcept {
			return m_languages;
		}
		std::vector<std::string>& languages() noexcept { return m_languages; }

	private:
		std::string m_user_agent;
		jar m_jar;
		std::unique_ptr<tangle::cache::cache> m_cache;
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
	    : m_impl{std::make_shared<backend, std::string>({})} {
		m_impl->set_cache(std::make_unique<impl::empty_cache>());
	}

	navigator::navigator(const config& cfg)
	    : m_impl{std::make_shared<backend>(cfg.app_version)} {
		cookies().path(cfg.jar_file);

		if (cfg.cache_dir.empty())
			m_impl->set_cache(std::make_unique<impl::empty_cache>());
		else {
			// TODO: missing cache implementation
			m_impl->set_cache(std::make_unique<impl::dummy_cache>());
		}

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
	const jar& navigator::cookies() const noexcept { return m_impl->cookies(); }
	tangle::cache::cache& navigator::cache() noexcept {
		return m_impl->cache();
	}
	const tangle::cache::cache& navigator::cache() const noexcept {
		return m_impl->cache();
	}

	const std::vector<std::string>& navigator::languages() const noexcept {
		return m_impl->languages();
	}

	tangle::cache::document navigator::open(const request& req,
	                                        cookie::time_point) {
		auto& addr = req.address();

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
