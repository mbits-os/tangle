/*
 * Copyright (C) 2017 midnightBITS
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

#include <tangle/nav/jar.h>
#include <tangle/nav/cache.h>
#include <tangle/nav/navigator.h>
#include <tangle/nav/protocol.h>
#include <cctype>

namespace tangle { namespace nav {
	namespace impl {
		struct empty_cache : cache {
			bool storage_backed() const noexcept { return false; }
			std::shared_ptr<file> get(const uri& address) override
			{
				return {};
			}

			std::shared_ptr<file> create(const uri& address) override
			{
				return {};
			}
		};

		struct dummy_cache : cache {
			bool storage_backed() const noexcept { return true; }
			std::shared_ptr<file> get(const uri& address) override
			{
				return {};
			}

			std::shared_ptr<file> create(const uri& address) override
			{
				return {};
			}
		};
	}
	
	struct navigator::backend {
		backend(const std::string& user_agent);
		~backend();

		void set_cache(std::unique_ptr<nav::cache> cache) { m_cache = std::move(cache); }

		void reg_proto(const std::string& scheme, const std::shared_ptr<protocol>& proto);
		std::shared_ptr<protocol> get_proto(const std::string& scheme);

		const std::string& user_agent() const noexcept { return m_user_agent; }
		jar& cookies() noexcept { return m_jar; }
		const jar& cookies() const noexcept { return m_jar; }
		nav::cache& cache() noexcept { return *m_cache; }
		const nav::cache& cache() const noexcept { return *m_cache; }
		const std::vector<std::string>& languages() const noexcept { return m_languages; }
		std::vector<std::string>& languages() noexcept { return m_languages; }

	private:
		std::string m_user_agent;
		jar m_jar;
		std::unique_ptr<nav::cache> m_cache;
		std::vector<std::string> m_languages;
		std::unordered_map<std::string, std::shared_ptr<protocol>> m_protocols;
	};

	navigator::~navigator() = default;
	navigator::navigator(const navigator&) = default;
	navigator& navigator::operator=(const navigator&) = default;
	navigator::navigator(navigator&&) noexcept = default;
	navigator& navigator::operator=(navigator&&) noexcept = default;

	navigator::navigator()
		: m_impl{ std::make_shared<backend, std::string>({}) }
	{
		m_impl->set_cache(std::make_unique<impl::empty_cache>());
	}

	navigator::navigator(const config& cfg)
		: m_impl{ std::make_shared<backend>(cfg.app_version) }
	{
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
		const std::shared_ptr<protocol>& proto)
	{
		auto scheme = key;
		for (auto& c : scheme)
			c = std::tolower((uint8_t)c);

		m_impl->reg_proto(scheme, proto);
	}


	const std::string& navigator::user_agent() const noexcept { return m_impl->user_agent(); }
	jar& navigator::cookies() noexcept { return m_impl->cookies(); }
	const jar& navigator::cookies() const noexcept { return m_impl->cookies(); }
	nav::cache& navigator::cache() noexcept { return m_impl->cache(); }
	const nav::cache& navigator::cache() const noexcept { return m_impl->cache(); }

	const std::vector<std::string>& navigator::languages() const noexcept
	{
		return m_impl->languages();
	}

	loader navigator::open(const request& req, bool refreshing, cookie::time_point when)
	{
		auto addr = req.address();
		addr.fragment({});
		addr = uri::normal(addr);

		if (!addr.has_authority())
			return {};

		if (!refreshing) {
			auto file = cache().get(addr);
			if (file && (file->is_active() || file->is_fresh(when)))
				return file->get_loader();
		}

		auto scheme = addr.scheme().str();
		auto proto = m_impl->get_proto(scheme);
		if (!proto)
			return {};

		return proto->open(req, *this);
	}


	navigator::backend::backend(const std::string& user_agent)
		: m_user_agent{ user_agent }
	{
	}

	navigator::backend::~backend()
	{
	}

	void navigator::backend::reg_proto(const std::string& scheme,
		const std::shared_ptr<protocol>& proto)
	{
		// synchronize...
		if (!proto) {
			m_protocols.erase(scheme);
			return;
		}

		m_protocols[scheme] = proto;
	}

	std::shared_ptr<protocol> navigator::backend::get_proto(const std::string& scheme)
	{
		// synchronize...
		auto it = m_protocols.find(scheme);
		if (it == m_protocols.end())
			return {};
		return it->second;
	}
}}
