/*
* Copyright (C) 2013 midnightBITS
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

#include <unordered_map>
#include <vector>
#include <tangle/cstring.h>

namespace tangle {
	std::string urlencode(const char* in, size_t in_len);
	std::string urldecode(const char* in, size_t in_len);

	inline std::string urlencode(const std::string& in)
	{
		return urlencode(in.c_str(), in.length());
	}

	inline std::string urldecode(const std::string& in)
	{
		return urldecode(in.c_str(), in.length());
	}

	inline std::string urlencode(const cstring& in)
	{
		return urlencode(in.c_str(), in.length());
	}

	inline std::string urldecode(const cstring& in)
	{
		return urldecode(in.c_str(), in.length());
	}

	class uri {
		std::string m_uri;
		using size_type = std::string::size_type;

		static constexpr size_type ncalc = (size_type)(-2);
		static constexpr size_type npos = (size_type)(-1);

		mutable size_type m_schema = ncalc;
		mutable size_type m_path = ncalc;
		mutable size_type m_query = ncalc;
		mutable size_type m_part = ncalc;

		void ensure_schema() const;
		void ensure_path() const;
		void ensure_query() const;
		void ensure_fragment() const;

		void invalidate_fragment()
		{
			m_part = ncalc;
		}

		void invalidate_query()
		{
			invalidate_fragment();
			m_query = ncalc;
		}

		void invalidate_path()
		{
			invalidate_query();
			m_path = ncalc;
		}

		void invalidate_schema()
		{
			invalidate_path();
			m_schema = ncalc;
		}

	public:
		uri();
		uri(const uri&);
		uri(uri&&);
		uri& operator=(const uri&);
		uri& operator=(uri&&);

		uri(const cstring& ident);

		struct auth_builder {
			std::string userInfo;
			std::string host;
			std::string port;

			static auth_builder parse(const cstring&);
			std::string string() const;
		};

		enum query_flag {
			form_urlencoded = false,
			start_with_qmark = true,
		};
		struct query_builder {
			std::unordered_map<std::string, std::vector<std::string>> m_values;
		public:
			static query_builder parse(const cstring& query);
			query_builder& add(const std::string& name, const std::string& value)
			{
				m_values[name].push_back(value);
				return *this;
			}

			std::string string(query_flag = start_with_qmark) const;
			std::vector<std::pair<std::string, std::string>> list() const;
		};

		bool hierarchical() const;
		bool opaque() const { return !hierarchical(); }
		bool relative() const;
		bool absolute() const { return !relative(); }
		cstring scheme() const;
		cstring authority() const;
		cstring path() const;
		cstring query() const;
		cstring fragment() const;
		void scheme(const cstring& value);
		void authority(const cstring& value);
		void path(const cstring& value);
		void query(const cstring& value);
		void fragment(const cstring& value);
		std::string string() const { return m_uri; }

		static uri make_base(const uri& document);
		static uri make_base(const cstring& document)
		{
			return make_base(uri { document });
		}

		static uri canonical(const uri& identifier, const uri& base);
		static uri canonical(const cstring& identifier, const uri& base)
		{
			return canonical(uri { identifier }, base);
		}

		static uri normal(uri identifier);
		static uri normal(const cstring& identifier)
		{
			return normal(uri { identifier });
		}
	};
}

