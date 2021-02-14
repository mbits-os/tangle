// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/jar.hpp>
#include <memory>

namespace tangle::cookie {
	jar::const_iterator jar::find(const std::string& name,
	                              const scope_type& scope) const {
		for (auto cur = begin(), last = end(); cur != last; ++cur) {
			auto& item = *cur;
			if (item.name() == name && item.scope() == scope) return cur;
		}

		return end();
	}

	void jar::add(const item& cookie, bool for_http) {
		add_impl(cookie, for_http);
	}

	void jar::add(item&& cookie, bool for_http) {
		add_impl(std::move(cookie), for_http);
	}

	template <typename Forwarded>
	void jar::add_impl(Forwarded&& cookie, bool for_http) {
		auto it = find(cookie.name(), cookie.scope());

		if (it == end()) {
			if ((cookie.flags() & flags::persistent) != flags::persistent ||
			    cookie.expires() > clock::now())
				m_items.push_back(std::forward<Forwarded>(cookie));
			return;
		}

		if ((it->flags() & flags::http_only) == flags::http_only && (!for_http))
			return;

		if ((cookie.flags() & flags::persistent) == flags::persistent &&
		    cookie.expires() <= clock::now()) {
			m_items.erase(it);
			return;
		}
		auto save_creation_time = it->creation_time();
		auto& dst = const_cast<item&>(*it);
		dst = std::forward<Forwarded>(cookie);
		dst.creation_time(save_creation_time);
	}

	std::string jar::str(const scope_type& uri,
	                     match options,
	                     time_point when) {
		static constexpr char con[] = "; ";
		std::vector<cookie::item*> cookies;
		size_t length = 0;

		cookies.reserve(m_items.size());
		for (auto& item : m_items) {
			if (!item.matches(uri, options, when)) continue;

			cookies.push_back(&item);
			if (length) length += sizeof(con) - 1;
			length += item.name().length() + item.value().length() + 1;
		}

		std::string out;
		out.reserve(length + 1);

		for (auto ptr : cookies) {
			if (out.length()) out.append(con);
			out.append(ptr->name());
			out.push_back('=');
			out.append(ptr->value());
			ptr->last_access_time(when);
		}

		return out;
	}

	void jar::prune(time_point when) {
		size_t valid = 0;
		for (auto& item : m_items) {
			if (item.has_expired(when)) continue;
			++valid;
		}

		std::vector<item> local;
		local.reserve(valid);
		for (auto& item : m_items) {
			if (item.has_expired(when)) continue;
			local.emplace_back(std::move(item));  // move away the strings
		}
		m_items = std::move(local);
	}

	struct file {
		void operator()(std::FILE* f) {
			if (f) std::fclose(f);
		}

		using ptr = std::unique_ptr<FILE, file>;

#ifdef WIN32
		static ptr open(const std::string& path, const char* mode) {
			FILE* handle = 0;
			(void)fopen_s(&handle, path.c_str(), mode);
			return ptr{handle};
		}
#else
		static ptr open(const std::string& path, const char* mode) {
			return ptr{std::fopen(path.c_str(), mode)};
		}
#endif
	};

	bool jar::store(const std::string& path, time_point when) const {
		auto f = file::open(path, "wb");
		if (!f) return false;
		return store_raw(f.get(), when);
	}

	bool jar::load(const std::string& path, time_point when) {
		auto f = file::open(path, "rb");
		if (!f) return false;
		return load_raw(f.get(), when);
	}
}  // namespace tangle::cookie
