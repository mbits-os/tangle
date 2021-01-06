// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once
#include <ostream>
#include <string>
#include <string_view>

namespace tangle::msg {
	class hasher {
	public:
		hasher();
		size_t value() const { return m_value; }
		hasher& append(const void* buffer, size_t length);
		static size_t hash(const std::string& s) {
			return hash(s.data(), s.length());
		}
		static size_t hash(std::string_view s) {
			return hash(s.data(), s.length());
		}
		static size_t hash(const char*, size_t);

	private:
		size_t m_value;
	};

	class combined_string {
	public:
		combined_string() { new (&m_cstr) std::string_view{}; }

		combined_string(std::string_view key) {
			new (&m_cstr) std::string_view{key};
		}

		combined_string(const char* key) {
			new (&m_cstr) std::string_view{key};
		}

		combined_string(const std::string& key) : m_use_cstr{false} {
			new (&m_str) std::string{key};
		}

		combined_string(std::string&& key) : m_use_cstr{false} {
			new (&m_str) std::string{std::move(key)};
		}

		combined_string(const combined_string& rhs)
		    : m_use_cstr{rhs.m_use_cstr} {
			if (m_use_cstr)
				new (&m_cstr) std::string_view{rhs.m_cstr};
			else
				new (&m_str) std::string{rhs.m_str};
		}

		combined_string& operator=(const combined_string& rhs) {
			if (this == &rhs) return *this;

			if (m_use_cstr)
				m_cstr.~basic_string_view();
			else
				m_str.~basic_string();

			m_use_cstr = rhs.m_use_cstr;
			if (m_use_cstr)
				new (&m_cstr) std::string_view{rhs.m_cstr};
			else
				new (&m_str) std::string{rhs.m_str};

			return *this;
		}

		~combined_string() {
			if (m_use_cstr)
				m_cstr.~basic_string_view();
			else
				m_str.~basic_string();
		}

		const char* data() const {
			return m_use_cstr ? m_cstr.data() : m_str.data();
		}

		size_t length() const {
			return m_use_cstr ? m_cstr.length() : m_str.length();
		}

		size_t hash() const {
			return m_use_cstr ? hasher::hash(m_cstr) : hasher::hash(m_str);
		}

		std::string str() const { return {data(), length()}; }
		operator std::string_view() const { return {data(), length()}; }
		std::string_view view() const { return {data(), length()}; }

		bool operator==(const combined_string& rhs) const {
			return view() == rhs.view();
		}

		friend inline std::ostream& operator<<(std::ostream& o,
		                                       const combined_string& cs) {
			return o << cs.view();
		}

	private:
		union {
			std::string m_str;
			std::string_view m_cstr;
		};
		bool m_use_cstr = true;
	};

}  // namespace tangle::msg

namespace std {
	template <>
	struct hash<tangle::msg::combined_string> {
		using argument_type = tangle::msg::combined_string;
		using result_type = size_t;
		size_t operator()(const tangle::msg::combined_string& key) const {
			return key.hash();
		}
	};

}  // namespace std
