/*
 * Copyright (C) 2016 midnightBITS
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

#include <tangle/cookie/jar.h>
#include <memory>

namespace net { namespace cookie {
	jar::const_iterator jar::find(const std::string& name, const scope_type& scope) const
	{
		for (auto cur = begin(), last = end(); cur != last; ++cur) {
			auto& item = *cur;
			if (item.name() == name &&
				item.scope().domain() == scope.domain() &&
				item.scope().path() == scope.path())
				return cur;
		}

		return end();
	}

	void jar::add(const item& cookie, bool for_http)
	{
		add_impl(cookie, for_http);
	}

	void jar::add(item&& cookie, bool for_http)
	{
		add_impl(std::move(cookie), for_http);
	}

	template <typename Forwarded>
	void jar::add_impl(Forwarded&& cookie, bool for_http)
	{
		auto it = find(cookie.name(), cookie.scope());

		if (it == end()) {
			if ((cookie.flags() & flags::persistent) != flags::persistent || cookie.expires() > clock::now())
				m_items.push_back(std::move(cookie));
			return;
		}

		if ((it->flags() & flags::http_only) == flags::http_only && (!for_http))
			return;

		if ((cookie.flags() & flags::persistent) == flags::persistent && cookie.expires() <= clock::now()) {
			m_items.erase(it);
			return;
		}
		auto save_creation_time = it->creation_time();
		auto& dst = const_cast<item&>(*it);
		dst = std::forward<Forwarded>(cookie);
		dst.creation_time(save_creation_time);
	}

	std::string jar::str(const scope_type& uri, match options, time_point when)
	{
		static constexpr char con[] = "; ";
		std::vector<cookie::item*> cookies;
		size_t length = 0;

		cookies.reserve(m_items.size());
		for (auto& item : m_items) {
			if (!item.matches(uri, options, when))
				continue;

			cookies.push_back(&item);
			if (length)
				length += sizeof(con) - 1;
			length += item.name().length() + item.value().length() + 1;
		}

		std::string out;
		out.reserve(length + 1);

		for (auto ptr : cookies) {
			if (out.length())
				out.append(con);
			out.append(ptr->name());
			out.push_back('=');
			out.append(ptr->value());
			ptr->last_access_time(when);
		}

		return out;
	}

	void jar::prune(time_point when)
	{
		size_t valid = 0;
		for (auto& item : m_items) {
			if (item.has_expired(when))
				continue;
			++valid;
		}

		std::vector<item> local;
		local.reserve(valid);
		for (auto& item : m_items) {
			if (item.has_expired(when))
				continue;
			local.emplace_back(std::move(item)); // move away the strings
		}
		m_items = std::move(local);
	}

	struct file {
		void operator()(std::FILE* f)
		{
			if (f)
				std::fclose(f);
		}

		using ptr = std::unique_ptr<FILE, file>;
	};

#ifdef WIN32
	FILE* pfopen(const fs::path& path, const char* mode)
	{
		wchar_t wmode[100];
		auto ptr = wmode;
		while (*ptr++ = *mode++); // naive ASCII-to-UCS

		FILE* handle = 0;
		(void)_wfopen_s(&handle, path.native().c_str(), wmode);
		return handle;
	}
#else
	FILE* pfopen(const fs::path& path, const char* mode)
	{
		return std::fopen(path.string().c_str(), mode);
	}
#endif

	bool jar::store(const fs::path& path, time_point when) const
	{
		file::ptr file{ pfopen(path, "wb") };
		if (!file)
			return false;
		return store_raw(file.get(), when);
	}

	bool jar::load(const fs::path& path, time_point when)
	{
		file::ptr file { pfopen(path, "rb") };
		if (!file)
			return false;
		return load_raw(file.get(), when);
	}
}}
