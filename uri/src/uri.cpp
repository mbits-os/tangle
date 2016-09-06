/*
 * Copyright (C) 2015 midnightBITS
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

#include <tangle/uri.h>
#include <cctype>

namespace tangle {
	namespace {
		int default_port(const std::string& scheme)
		{
#define KNOWN(proto, port) if (scheme == #proto) return port;

			KNOWN(http, 80); // ~100% of use cases...
			KNOWN(https, 443);

			KNOWN(ftp, 21);
			KNOWN(ssh, 22);
			KNOWN(telnet, 23);

#undef KNOWN
			return -1;
		}

		std::string tolower(std::string s)
		{
			for (auto& c : s)
				c = (char)std::tolower((uint8_t)c);

			return s;
		}

		std::vector<std::string> path_split(const std::string& path)
		{
			auto length = 1;
			for (auto c : path) {
				if (c == '/')
					++length;
			}

			std::vector<std::string> out;
			out.reserve(length);

			auto slash = path.find('/');
			decltype(slash) prev = 0;
			while (slash != std::string::npos) {
				out.emplace_back(path.substr(prev, slash - prev));
				prev = slash + 1;
				slash = path.find('/', prev);
			}
			out.emplace_back(path.substr(prev));

			return out;
		}

		std::string path_join(const std::vector<std::string>& chunks)
		{
			if (chunks.empty())
				return { };

			auto length = chunks.size() - 1;
			for (auto& ch : chunks)
				length += ch.length();

			std::string out;
			out.reserve(length);

			bool first = true;
			for (auto& ch : chunks) {
				if (first) first = false;
				else out.push_back('/');

				out.append(ch);
			}

			return out;
		}

		inline bool issafe(unsigned char c)
		{
			return std::isalnum(c) || c == '-' || c == '-' || c == '.' || c == '_' || c == '~';
		}

		inline bool auth_issafe(unsigned char c)
		{
			return issafe(c) || c == ':' || c == '[' || c == ']';
		}

		template <typename Pred>
		inline std::string urlencode(const char* in, size_t in_len, Pred&& safe)
		{
			size_t length = in_len;

			auto b = in;
			auto e = b + in_len;

			for (auto it = b; it != e; ++it) {
				if (!safe(*it))
					length += 2;
			}

			static constexpr char hexes[] = "0123456789ABCDEF";
			std::string out;
			out.reserve(length);

			for (auto it = b; it != e; ++it) {
				auto c = *it;
				if (safe(c)) {
					out += c;
					continue;
				}
				out += '%';
				out += hexes[(c >> 4) & 0xF];
				out += hexes[(c) & 0xF];
			}
			return out;
		}

		inline char hex(char c)
		{
			switch (c) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				return c - '0';
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
				return c - 'a' + 10;
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
				return c - 'A' + 10;
			}
			return 0;
		}

		inline std::string authority_urlencode(const char* in, size_t in_len)
		{
			return urlencode(in, in_len, auth_issafe);
		}

		inline std::string authority_urlencode(const std::string& in)
		{
			return authority_urlencode(in.c_str(), in.length());
		}
	}

	std::string urlencode(const char* in, size_t in_len)
	{
		return urlencode(in, in_len, issafe);
	}

	std::string urldecode(const char* in, size_t in_len)
	{
		std::string out;
		out.reserve(in_len);

		for (size_t i = 0; i < in_len; ++i) {
			// go inside only, if there is enough space
			if (in[i] == '%' && (i < in_len - 2) &&
				isxdigit(in[i + 1]) && isxdigit(in[i + 2])) {
				unsigned char c = (hex(in[i + 1]) << 4) | hex(in[i + 2]);
				out += c;
				i += 2;
				continue;
			}
			out += in[i];
		}
		return out;
	}

	uri::auth_builder uri::auth_builder::parse(const std::string& auth)
	{
		auto pos = auth.find('@');
		auto host = pos == std::string::npos ? 0 : pos + 1;

		auto colon = auth.find_last_of(':');
		if (auth.length() > host && auth[host] == '[') {
			// IPv6/IPVFuture...
			auto end = auth.find(']', host);
			if (end == std::string::npos)
				return { };

			if (auth.length() > end && auth[end + 1] != ':' && auth[end + 1] != 0)
				return { };

			colon = end + 1;
			if (colon >= auth.length())
				colon = std::string::npos;
		}
		auto host_count = colon == std::string::npos ? std::string::npos : colon - host;

		auth_builder out { };

		if (host)
			out.userInfo = urldecode(auth.substr(0, pos));

		out.host = urldecode(auth.substr(0, host_count));
		if (colon != std::string::npos)
			out.port = urldecode(auth.substr(colon + 1));

		return out;
	}

	std::string uri::auth_builder::string() const
	{
		auto auserInfo = authority_urlencode(userInfo);
		auto ahost = authority_urlencode(host);
		auto aport = authority_urlencode(port);

		if (aport.empty() && auserInfo.empty())
			return ahost;

		size_t length = ahost.length();
		if (!auserInfo.empty())
			length += auserInfo.length() + 1;
		if (!aport.empty())
			length += aport.length() + 1;

		std::string out;
		out.reserve(length);

		if (!auserInfo.empty()) {
			out.append(auserInfo);
			out.push_back('@');
		}

		out.append(ahost);

		if (!aport.empty()) {
			out.push_back(':');
			out.append(aport);
		}
		return out;
	}


	std::string uri::query_builder::string(query_flag flag) const
	{
		std::string out;
		bool first = true;
		for (auto& pair : m_values) {
			auto name = urlencode(pair.first) + "=";
			for (auto& value : pair.second) {
				if (first) {
					first = false;
					if (flag == start_with_qmark)
						out += "?";
				} else out += "&";

				out += name + urlencode(value);
			}
		}
		return out;
	}

	std::vector<std::pair<std::string, std::string>> uri::query_builder::list() const
	{
		size_t length = 0;
		for (auto& pair : m_values) {
			length += pair.second.size();
		}

		std::vector<std::pair<std::string, std::string>> out;
		out.reserve(length);

		for (auto& pair : m_values) {
			for (auto& value : pair.second)
				out.emplace_back(pair.first, value);
		}

		return out;
	}

#define WS() do { while (isspace((unsigned char)*c) && c < end) ++c; } while(0)
#define LOOK_FOR(ch) do { while (!isspace((unsigned char)*c) && *c != (ch) && c < end) ++c; } while(0)
#define LOOK_FOR2(ch1, ch2) do { while (!isspace((unsigned char)*c) && *c != (ch1) && *c != (ch2) && c < end) ++c; } while(0)
#define IS(ch) (c < end && *c == (ch))

	uri::query_builder uri::query_builder::parse(const std::string& query)
	{
		uri::query_builder out;
		const char* c = query.c_str();
		const char* end = c + query.length();
		while (c < end) {
			WS();
			const char* name_start = c;
			LOOK_FOR2('=', '&');
			std::string name = urldecode(name_start, c - name_start);
			WS();

			if (IS('=')) {
				++c;
				WS();
				const char* value_start = c;
				LOOK_FOR('&');
				out.add(name, urldecode(value_start, c - value_start));
				WS();
			} else
				out.add(name, { });

			if (!IS('&')) break;

			++c;
		}
		return out;
	}

#undef WS
#undef LOOK_FOR
#undef LOOK_FOR2
#undef IS

	uri::uri() = default;
	uri::uri(const uri&) = default;
	uri::uri(uri&&) = default;
	uri& uri::operator=(const uri&) = default;
	uri& uri::operator=(uri&&) = default;

	uri::uri(const std::string& ident)
		: m_uri { ident }
	{
	}

	void uri::ensure_schema() const
	{
		if (m_schema != ncalc)
			return;

		m_schema = npos;
		auto length = m_uri.length();

		auto b = m_uri.data();
		auto c = b;
		auto e = b + length;

		if (c == e || !isalpha((unsigned char)*c))
			return;

		++c;
		while (c != e && (isalnum((unsigned char)*c) || *c == '+' || *c == '-' || *c == '.'))
			++c;

		if (c == e || *c != ':')
			return;

		m_schema = c - b;
	}

	void uri::ensure_path() const
	{
		if (m_path != ncalc)
			return;

		ensure_schema();

		if (m_schema == npos) {
			m_path = 0;
			return;
		}

		auto length = m_uri.length();

		auto c = m_uri.data();

		m_path = m_schema + 1;

		if (m_schema + 2 >= length || c[m_schema + 1] != '/' || c[m_schema + 2] != '/')
			return;

		m_path = m_schema + 3;
		while (m_path < length) {
			switch (c[m_path]) {
			case '/': case '?': case '#':
				return;
			}
			++m_path;
		}
	}

	void uri::ensure_query() const
	{
		if (m_query != ncalc)
			return;

		ensure_path();

		auto length = m_uri.length();

		auto c = m_uri.data();

		m_query = m_path;
		while (m_query < length) {
			switch (c[m_query]) {
			case '?': case '#':
				return;
			}
			++m_query;
		}

	}

	void uri::ensure_fragment() const
	{
		if (m_part != ncalc)
			return;

		ensure_query();

		auto length = m_uri.length();

		auto c = m_uri.data();

		m_part = m_query;
		while (m_part < length) {
			if (c[m_part] == '#')
				return;
			++m_part;
		}
	}

	bool uri::hierarchical() const
	{
		if (relative())
			return true;

		ensure_path();

		if (m_path - m_schema <= 2)
			return false;

		auto c = m_uri.data();
		return c[m_schema + 1] == '/' && c[m_schema + 2] == '/';
	}

	bool uri::relative() const
	{
		ensure_schema();
		return m_schema == npos;
	}

	std::string uri::scheme() const
	{
		if (relative())
			return std::string();

		return m_uri.substr(0, m_schema);
	}

	std::string uri::authority() const
	{
		if (relative() || opaque())
			return std::string();

		auto start = m_schema + 3;
		return m_uri.substr(start, m_path - start);
	}

	std::string uri::path() const
	{
		ensure_query();
		return m_uri.substr(m_path, m_query - m_path);
	}

	std::string uri::query() const
	{
		ensure_fragment();
		return m_uri.substr(m_query, m_part - m_query);
	}

	std::string uri::fragment() const
	{
		ensure_fragment();
		return m_uri.substr(m_part);
	}

	void uri::scheme(const std::string& value)
	{
		if (relative())
			return;

		m_uri.replace(0, m_schema, value);
		invalidate_path();
	}

	void uri::authority(const std::string& value)
	{
		if (relative() || opaque())
			return;

		auto start = m_schema + 3;
		m_uri.replace(start, m_path - start, value);
		invalidate_path();
	}

	void uri::path(const std::string& value)
	{
		ensure_query();
		if (hierarchical() && absolute() && (value.empty() || value[0] != '/')) {
			m_uri.replace(m_path, m_query - m_path, "/");
			++m_path;
			m_query = m_path;
		}
		m_uri.replace(m_path, m_query - m_path, value);
		invalidate_path(); // query -> path due to having possibly taken the '/' branch and having ++m_path
	}

	void uri::query(const std::string& value)
	{
		ensure_fragment();
		m_uri.replace(m_query, m_part - m_query, value);
		invalidate_fragment();
	}

	void uri::fragment(const std::string& value)
	{
		ensure_fragment();
		m_uri.replace(m_part, m_uri.length() - m_part, value);
	}

	std::string remove_filename(std::string path)
	{
		auto find = path.find_last_of('/');
		if (find == std::string::npos)
			return path;
		return path.substr(0, find + 1);
	}

	uri uri::make_base(const uri& document)
	{
		if (document.string().empty())
			return document;

		auto tmp = document;
		if (tmp.relative())
			tmp = "http://" + tmp.string();

		tmp.fragment(std::string());
		tmp.query(std::string());
		tmp.path(remove_filename(tmp.path()));
		tmp.ensure_query();
		return tmp;
	}


	uri uri::canonical(const uri& identifier, const uri& base)
	{
		if (identifier.absolute())
			return normal(identifier);

		// base-schema://base-auth/base-path?uri-query#uri-frag
		auto temp = base;
		temp.fragment(identifier.fragment());
		temp.query(identifier.query());

		auto path = identifier.path();
		if (!path.empty() && path[0] == '/')
			return temp.path(path), normal(std::move(temp));

		auto bpath = base.path();
		if (!bpath.empty())
			return temp.path(base.path() + "/" + path), normal(std::move(temp));
		return temp.path(path), normal(std::move(temp));
	}

	uri uri::normal(uri tmp)
	{
		if (tmp.absolute() && tmp.hierarchical()) {
			auto scheme = tolower(tmp.scheme());
			tmp.scheme(scheme);

			auto auth = auth_builder::parse(tmp.authority());
			if (auth.host.empty()) // decoding failed
				return { };

			// HOST: =======================================================================================
			auth.host = tolower(auth.host);

			// is IPv4, IPv6 or reg-name?
			for (auto c : auth.host) {
				if (!isalnum((uint8_t)c) && c != '-' && c != '.' && c != '[' && c != ']' && c != ':')
					return { };
			}

			// PORT: =======================================================================================
			// empty or digits
			for (auto c : auth.port) {
				if (!isdigit((uint8_t)c))
					return { };
			}

			// if default for the scheme, remove
			if (!auth.port.empty()) {
				auto port = atoi(auth.port.c_str());
				auto def = default_port(scheme);
				if (port == def)
					auth.port.clear();
			}

			tmp.authority(auth.string());
		}
		// PATH: =======================================================================================
		if (tmp.hierarchical()) {

			auto path = path_split(tmp.path());
			for (auto& part : path)
				part = urlencode(urldecode(part));

			bool absolute = (path.size() > 1) && path.front().empty();

			// URL ended with slash; should still end with slash
			// Also, URL path ended with either xxxx/. or xxxxx/..
			// -> after resolving the result should be a "dir"
			bool empty_at_end = (path.size() > 1) &&
				(path.back().empty() || path.back() == "." || path.back() == "..");
			decltype(path) canon;
			canon.reserve(path.size());

			decltype(path) overshots;
			for (auto& p : path) {
				if (p.empty() || p == ".")
					continue;

				if (p == "..") {
					if (canon.empty())
						overshots.push_back(std::move(p));
					else
						canon.pop_back();
					continue;
				}

				canon.push_back(std::move(p));
			}
			if (empty_at_end)
				canon.emplace_back();
			if (absolute)
				tmp.path("/" + path_join(canon));
			else {
				canon.insert(canon.begin(), overshots.begin(), overshots.end());
				tmp.path(path_join(canon));
			}
		}

		return tmp;
	}
}
