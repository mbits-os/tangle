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

#pragma once
#include <string>
#include <cstring>

namespace tangle { namespace msg {
	class hasher {
	public:
		hasher();
		size_t value() const { return m_value; }
		hasher& append(const void* buffer, size_t length);
		static size_t hash(const std::string&);
		static size_t hash(const char*, size_t);
	private:
		size_t m_value;
	};

	class cstring {
	public:
		constexpr cstring() = default;
		constexpr cstring(const char* string, size_t length)
			: m_data(string)
			, m_length(length)
		{
		}

		cstring(const char* string)
			: cstring(string, string ? std::strlen(string) : 0)
		{
		}

		int compare(const cstring& rhs) const;
		bool operator < (const cstring& rhs) const
		{
			return compare(rhs) < 0;
		}
		bool operator == (const cstring& rhs) const
		{
			return compare(rhs) == 0;
		}

		const char* c_str() const { return m_data; }
		size_t length() const { return m_length; }
		size_t hash() const { return hasher::hash(m_data, m_length); }
		const char* begin() const { return m_data; }
		const char* end() const { return m_data + m_length; }

		std::string str() const { return { c_str(), length() }; }
	protected:
		void update(const char* string, size_t length)
		{
			m_data = string;
			m_length = length;
		}
	private:
		const char* m_data = nullptr;
		size_t m_length = 0;
	};

	class string {
	public:
		string() = default;
		explicit string(const std::string& stg)
			: m_data { stg }
		{
		}
		explicit string(std::string&& stg)
			: m_data { std::move(stg) }
		{
		}
		explicit string(const char* stg)
			: m_data { stg }
		{
		}
		string(const char* stg, size_t length)
			: m_data { stg, length }
		{
		}

		int compare(const string& rhs) const
		{
			return m_data.compare(rhs.m_data);
		}
		bool operator < (const string& rhs) const
		{
			return m_data < rhs.m_data;
		}
		bool operator == (const string& rhs) const
		{
			return m_data == rhs.m_data;
		}

		bool operator == (const cstring& rhs) const
		{
			return rhs == *this;
		}

		const char* c_str() const { return m_data.c_str(); }
		size_t length() const { return m_data.length(); }
		size_t hash() const { return hasher::hash(m_data); }

		const std::string& str() const { return m_data; }
		operator cstring() const
		{
			return { c_str(), length() };
		}
	private:
		const std::string m_data;
	};

	class combined_string {
	public:
		combined_string()
		{
			new (&m_cstr) cstring { };
		}

		combined_string(const cstring& key)
		{
			new (&m_cstr) cstring { key };
		}

		combined_string(const string& key) : m_use_cstr { false }
		{
			new (&m_str) string { key };
		}

		combined_string(string&& key) : m_use_cstr { false }
		{
			new (&m_str) string { std::move(key) };
		}

		combined_string(const combined_string& rhs) : m_use_cstr { rhs.m_use_cstr }
		{
			if (m_use_cstr)
				new (&m_cstr) cstring { rhs.m_cstr };
			else
				new (&m_str) string { rhs.m_str };
		}

		combined_string& operator=(const combined_string& rhs)
		{
			if (this == &rhs)
				return *this;

			if (m_use_cstr)
				m_cstr.~cstring();
			else
				m_str.~string();

			m_use_cstr = rhs.m_use_cstr;
			if (m_use_cstr)
				new (&m_cstr) cstring { rhs.m_cstr };
			else
				new (&m_str) string { rhs.m_str };

			return *this;
		}

		~combined_string()
		{
			if (m_use_cstr)
				m_cstr.~cstring();
			else
				m_str.~string();
		}

		const char* c_str() const
		{
			return m_use_cstr ? m_cstr.c_str() : m_str.c_str();
		}

		size_t length() const
		{
			return m_use_cstr ? m_cstr.length() : m_str.length();
		}

		size_t hash() const
		{
			return m_use_cstr ? m_cstr.hash() : m_str.hash();
		}

		std::string str() const { return { c_str(), length() }; }
		operator cstring() const
		{
			return { c_str(), length() };
		}

		bool operator == (const combined_string& rhs) const
		{
			return (cstring)*this == rhs;
		}

	private:
		union {
			string m_str;
			cstring m_cstr;
		};
		bool m_use_cstr = true;
	};

}};

namespace std {
	template <>
	struct hash<tangle::msg::cstring> : unary_function<tangle::msg::cstring, size_t> {
		size_t operator()(const tangle::msg::cstring& key) const
		{
			return key.hash();
		}
	};
	template <>
	struct hash<tangle::msg::string> : unary_function<tangle::msg::string, size_t> {
		size_t operator()(const tangle::msg::string& key) const
		{
			return key.hash();
		}
	};
	template <>
	struct hash<tangle::msg::combined_string> : std::unary_function<tangle::msg::combined_string, size_t> {
		size_t operator()(const tangle::msg::combined_string& key) const
		{
			return key.hash();
		}
	};

};