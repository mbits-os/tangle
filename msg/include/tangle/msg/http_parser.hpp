// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once
#include <tangle/msg/base_parser.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace tangle::msg {
	struct http_version {
		int http_major;
		int http_minor;
		http_version(int http_major = 0, int http_minor = 0)
		    : http_major(http_major), http_minor(http_minor) {}
	};

	template <typename Final>
	class http_parser_base {
	public:
		using dict_t = base_parser::dict_t;
		std::pair<size_t, parsing> append(const char* data, size_t length);
		const http_version& proto() const { return m_proto; }
		dict_t const& dict() const { return m_fields.dict(); }

	protected:
		void set_proto(int http_major, int http_minor) {
			m_proto = {http_major, http_minor};
		}

	private:
		base_parser m_fields;
		http_version m_proto;
		bool m_needs_first_line = true;
	};

	template <typename Final>
	inline std::pair<size_t, parsing> http_parser_base<Final>::append(
	    const char* data,
	    size_t length) {
		if (m_needs_first_line) {
			auto& thiz = static_cast<Final&>(*this);
			auto ret = thiz.first_line(data, length);

			if (std::get<parsing>(ret) != parsing::separator) return ret;

			m_needs_first_line = false;
			auto offset = std::get<size_t>(ret);
			ret = m_fields.append(data + offset, length - offset);
			std::get<size_t>(ret) += offset;
			return ret;
		}
		return m_fields.append(data, length);
	}

	class http_request : public http_parser_base<http_request> {
		friend class http_parser_base<http_request>;
		std::pair<size_t, parsing> first_line(const char* data, size_t length);

	public:
		const std::string& method() const { return m_method; }
		const std::string& resource() const { return m_resource; }

	private:
		std::string m_method;
		std::string m_resource;
	};

	class http_response : public http_parser_base<http_response> {
		friend class http_parser_base<http_response>;
		std::pair<size_t, parsing> first_line(const char* data, size_t length);

	public:
		int status() const { return m_status; }
		const std::string& reason() const { return m_reason; }

	private:
		int m_status = 0;
		std::string m_reason;
	};
}  // namespace tangle::msg
