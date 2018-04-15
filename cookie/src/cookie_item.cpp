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

#include <tangle/cookie/item.h>
#include <cctype>
#include <ctime>

namespace tangle { namespace cookie {
	static inline std::string to_string(const std::string_view& sv) {
		return { sv.data(), sv.length() };
	}

	item::item(const std::string& name, const std::string& value, const scope_type& scope)
		: m_name(name)
		, m_value(value)
		, m_scope(scope)
	{
	}

	item::item(const std::string& name, const std::string& value, const scope_type& scope, cookie::flags flags, time_point exp)
		: m_name(name)
		, m_value(value)
		, m_scope(scope)
		, m_flags(flags)
		, m_expires(exp)
	{
	}

	item::item(const std::string& name, const std::string& value, const scope_type& scope, cookie::flags flags, time_point exp, time_point created)
		: m_name(name)
		, m_value(value)
		, m_scope(scope)
		, m_flags(flags)
		, m_expires(exp)
		, m_creation_time(created)
	{
	}

	item::item() = default;
	item::item(const item&) = default;
	item::item(item&&) = default;
	item& item::operator=(const item&) = default;
	item& item::operator=(item&&) = default;

	bool item::matches(const scope_type& upstream, match options, time_point when) const
	{
		// secure cookie over non-secure channel?
		if ((m_flags & cookie::flags::secure) == cookie::flags::secure && (options & match::secure) != match::secure)
			return false;

		// http-only cookie over non-http API?
		if ((m_flags & cookie::flags::http_only) == cookie::flags::http_only && (options & match::http) == match::none)
			return false;

		// expired?
		if ((m_flags & cookie::flags::persistent) == cookie::flags::persistent && when > m_expires)
			return false;

		// domain matches?
		return m_scope.matches(upstream, (m_flags & cookie::flags::host_only) == cookie::flags::host_only);
	}

	namespace {
		bool parse_pair(std::string_view pair, std::string& name, std::string& value)
		{
			name.clear();
			value.clear();
			auto last = pair.length();
			decltype(last) cur = 0;

			while (cur != last && std::isspace((uint8_t)pair[cur])) ++cur;
			auto nstart = cur;

			while (cur != last && pair[cur] != '=') ++cur;
			bool result = cur != last;
			auto nstop = cur;

			while (nstop > nstart && std::isspace((uint8_t)pair[nstop - 1])) --nstop;
			auto nspan = pair.substr(nstart, nstop - nstart);
			name.assign(nspan.data(), nspan.length());

			if (!result)
				return false;
			++cur;

			while (cur != last && std::isspace((uint8_t)pair[cur])) ++cur;
			while (cur < last && std::isspace((uint8_t)pair[last - 1])) --last;
			auto vspan = pair.substr(cur, last - cur);
			value.assign(vspan.data(), vspan.length());

			return true;
		}

		template <typename offset_t, typename chars_t, typename pred_t>
		bool next_chunk(const chars_t& header, offset_t& cur, offset_t last, pred_t pred)
		{
			while (cur != last && std::isspace((uint8_t)header[cur])) ++cur;
			auto nstart = cur;

			while (cur != last && header[cur] != '=' && header[cur] != ';') ++cur;
			bool has_value = cur != last && header[cur] == '=';
			auto nstop = cur;

			while (nstop > nstart && std::isspace((uint8_t)header[nstop - 1])) --nstop;
			auto name = header.substr(nstart, nstop - nstart);

			if (cur != last)
				++cur;

			auto vstart = cur;
			auto vstop = cur;
			auto one_more = cur != last;
			if (has_value) {
				while (cur != last && std::isspace((uint8_t)header[cur])) ++cur;
				vstart = cur;

				while (cur != last && header[cur] != ';') ++cur;
				vstop = cur;
				while (vstop > vstart && std::isspace((uint8_t)header[vstop - 1])) --vstop;

				one_more = cur != last;
				if (one_more)
					++cur;
			}

			auto value = header.substr(vstart, vstop - vstart);
			pred(name, value, has_value);
			return one_more;
		}

		std::string lower(std::string_view span)
		{
			std::string out;
			out.reserve(span.length() + 1);
			for (uint8_t c : span)
				out.push_back(std::tolower(c));
			return out;
		}

		inline bool is_delim(uint8_t c)
		{
			// %x09 / %x20-2F / %x3B-40 / %x5B-60 / %x7B-7E

			static constexpr uint32_t delims[] = {
				//FEDCBA98 76543210 FEDCBA98 76543210
				0b00000000'00000000'00000010'00000000, // 0. | 0
				0b11111000'00000000'11111111'11111111, // 2. | 1
				0b11111000'00000000'00000000'00000001, // 4. | 2
				0b01111000'00000000'00000000'00000001, // 6. | 3
				0b00000000'00000000'00000000'00000000, // 8. | 4
				0b00000000'00000000'00000000'00000000, // A. | 5
				0b00000000'00000000'00000000'00000000, // C. | 6
				0b00000000'00000000'00000000'00000000, // E. | 7
			};

			return (delims[(c >> 5) & 0x7] & (1 << (c & 0x1F))) != 0;
		}

		bool cookie_date_int(std::string_view tok, int& value)
		{
			for (uint8_t c : tok) {
				if (!std::isdigit(c))
					return false;
			}

			char* pos = nullptr;
			auto ret = std::strtol(tok.data(), &pos, 10);
			if (pos != tok.length() + tok.data())
				return false;
			value = ret;
			return true;
		}

		bool cookie_date_month(std::string_view tok, int& value)
		{
			auto mnth = lower(tok);
			static const char* mnths[] = {
				"jan", "feb", "mar", "apr",
				"may", "jun", "jul", "aug",
				"sep", "oct", "nov", "dec"
			};
			size_t ndx = 1;
			for (auto m : mnths) {
				if (mnth == m) {
					value = ndx;
					return true;
				}
				++ndx;
			}
			return false;
		}

		bool cookie_date_time(std::string_view date, int& hour, int& minute, int& second)
		{
			if (date.length() < 8) return false;
			if (!std::isdigit((uint8_t)date[0])) return false;
			if (!std::isdigit((uint8_t)date[1])) return false;
			if (date[2] != ':') return false;
			if (!std::isdigit((uint8_t)date[3])) return false;
			if (!std::isdigit((uint8_t)date[4])) return false;
			if (date[5] != ':') return false;
			if (!std::isdigit((uint8_t)date[6])) return false;
			if (!std::isdigit((uint8_t)date[7])) return false;

			char* pos = nullptr;
			char buf[3] = { 0, 0, 0 };

			buf[0] = date[0];
			buf[1] = date[1];
			auto vhour = std::strtol(buf, &pos, 10);
			if (pos != buf + 2) return false;

			pos = nullptr;
			buf[0] = date[3];
			buf[1] = date[4];
			auto vminute = std::strtol(buf, &pos, 10);
			if (pos != buf + 2) return false;

			pos = nullptr;
			buf[0] = date[6];
			buf[1] = date[7];
			auto vsecond = std::strtol(buf, &pos, 10);
			if (pos != buf + 2) return false;

			hour = vhour;
			minute = vminute;
			second = vsecond;
			return true;
		}

		time_point cookie_date(std::string_view date)
		{
			auto last = date.length();
			decltype(last) cur = 0;

			enum {
				found_time = 0x1,
				found_mday = 0x2,
				found_month = 0x4,
				found_year = 0x8,
				found_all = 0xF
			};

			int hour = 0, minute = 0, second = 0;
			int mday = 0, month = 0, year = 0;

			int found = 0;

			while (cur != last) {
				while (cur != last && is_delim(date[cur])) ++cur;
				auto start = cur;
				while (cur != last && !is_delim(date[cur])) ++cur;
				if (cur != start) {
					auto tok = date.substr(start, cur - start);

					if (!(found & found_time) && cookie_date_time(tok, hour, minute, second)) {
						found |= found_time;
						continue;
					}

					if (!(found & found_mday) && cookie_date_int(tok, mday)) {
						found |= found_mday;
						continue;
					}

					if (!(found & found_month) && cookie_date_month(tok, month)) {
						found |= found_month;
						continue;
					}

					if (!(found & found_year) && cookie_date_int(tok, year)) {
						found |= found_year;
						if (year < 100) // 0-69: +2000, 70-99: +1900
							year += year < 70 ? 2000 : 1900;
						continue;
					}
				}
			}

			if (found != found_all)
				return clock::max_epoch();

			return clock::from_date({ year, month, mday, hour, minute, second });
		}
	}

	std::vector<item> from_client(const std::string& origin, std::string_view header)
	{
		std::vector<item> out;
		scope_type orig { origin, "/" };

		auto last = header.length();
		decltype(last) cur = 0;

		auto emplace = [&](auto name, auto value, bool has_value) {
			if (has_value)
				out.emplace_back(item { to_string(name), to_string(value), orig });
		};

		while (next_chunk(header, cur, last, emplace));

		return out;
	}

	item from_server(const std::string& origin, std::string_view header, time_point created)
	{
		std::string name;
		std::string value;
		std::string domain;
		std::string path;
		std::string_view expires;
		std::string max_age;
		auto flags = cookie::flags::none;
		auto until = clock::max_epoch();

		auto last = header.length();
		decltype(last) cur = 0;
		bool cont = next_chunk(header, cur, last, [&](auto n, auto v, bool has_value) {
			if (!has_value)
				return;
			name = to_string(n);
			value = to_string(v);
		});

		if (name.empty())
			return { };

		auto cookie_av = [&](auto n, auto v, bool has_value) {
			auto name = lower(n);
			if (has_value) {
				if (name == "domain") {
					if (!v.empty() && v[0] == '.')
						domain = to_string(v.substr(1));
					else
						domain = to_string(v);
					return;
				}

				if (name == "path") {
					path = to_string(v);
					return;
				}

				if (name == "expires") {
					expires = v;
					return;
				}

				if (name == "max-age") {
					max_age = to_string(v);
					return;
				}

				return;
			}
			if (name == "secure") {
				flags = flags | cookie::flags::secure;
				return;
			}
			if (name == "httponly") {
				flags = flags | cookie::flags::http_only;
				return;
			}
		};

		while (next_chunk(header, cur, last, cookie_av));

		// First max-age, then expires, then latest representable:
		//
		// https://tools.ietf.org/html/rfc6265#section-5.3
		// Otherwise, if the cookie-attribute-list contains an attribute
		// with an attribute-name of "Expires" (and does not contain an
		// attribute with an attribute-name of "Max-Age")
		if (!max_age.empty()) {
			char* pos = nullptr;
			auto secs = std::strtol(max_age.c_str(), &pos, 10);
			if (pos == max_age.c_str() + max_age.length()) {
				until = created + std::chrono::seconds(secs);
				flags = flags | cookie::flags::persistent;
			}
		} else if (!expires.empty()) {
			until = cookie_date(expires);
			flags = flags | cookie::flags::persistent;
		}

		if (domain.empty()) {
			domain = origin;
			flags = flags | cookie::flags::host_only;
		} else if (domain != origin) {
			// do not allow cookies from outside the scope
			if (!scope_type{ domain, {}}.matches({ origin, {}}, false))
				return {};
		}

		if (path.empty())
			path = "/";

		return { name, value, { domain, path }, flags, until, created };
	}

	std::string item::server_string(bool prefer_maxage, time_point when) const
	{
		static constexpr char s_dom[] = "; Domain=";
		static constexpr char s_path[] = "; Path=";
		static constexpr char s_expires[] = "; Expires=";
		static constexpr char s_maxage[] = "; Max-Age=";
		static constexpr char s_secure[] = "; Secure";
		static constexpr char s_http_only[] = "; HttpOnly";
		size_t len = m_name.length() + m_value.length() + 1;
		std::string date;

		if ((m_flags & cookie::flags::host_only) != cookie::flags::host_only && !m_scope.domain().empty()) {
			len += sizeof(s_dom) - 1;
			len += m_scope.domain().length();
		}

		if (!m_scope.path().empty()) {
			len += sizeof(s_path) - 1;
			len += m_scope.path().length();
		}

		if ((m_flags & cookie::flags::persistent) == cookie::flags::persistent) {
			if (prefer_maxage) {
				date = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(m_expires - when).count());
				len += sizeof(s_maxage) - 1;
			} else {
				date = clock::to_string(m_expires);
				len += sizeof(s_expires) - 1;
			}
			len += date.length();
		}

		if ((m_flags & cookie::flags::secure) == cookie::flags::secure)
			len += sizeof(s_secure) - 1;

		if ((m_flags & cookie::flags::http_only) == cookie::flags::http_only)
			len += sizeof(s_http_only) - 1;

		std::string out;
		out.reserve(len);

		out.append(m_name);
		out.push_back('=');
		out.append(m_value);

		if ((m_flags & cookie::flags::host_only) != cookie::flags::host_only && !m_scope.domain().empty()) {
			out.append(s_dom);
			out.append(m_scope.domain());
		}

		if (!m_scope.path().empty()) {
			out.append(s_path);
			out.append(m_scope.path());
		}

		if ((m_flags & cookie::flags::persistent) == cookie::flags::persistent) {
			if (prefer_maxage)
				out.append(s_maxage);
			else
				out.append(s_expires);

			out.append(date);
		}

		if ((m_flags & cookie::flags::secure) == cookie::flags::secure)
			out.append(s_secure);

		if ((m_flags & cookie::flags::http_only) == cookie::flags::http_only)
			out.append(s_http_only);

		return out;
	}
}}
