// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once
#include <tangle/msg/hasher.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace tangle::msg {
	enum class parsing { reading, separator, error };

	class field_parser {
	public:
		class span {
		public:
			constexpr span() = default;
			constexpr span(size_t offset, size_t length)
			    : m_offset(offset), m_length(length) {}

			constexpr size_t offset() const { return m_offset; }
			constexpr size_t length() const { return m_length; }

		private:
			size_t m_offset = 0;
			size_t m_length = 0;
			size_t m_hash = 0;
		};

		using dict_t =
		    std::unordered_map<combined_string, std::vector<std::string>>;

		std::pair<size_t, parsing> append(const char* data, size_t length);

		dict_t const& dict() const { return m_dict; }

		bool headers_seen() const noexcept { return m_headers_seen; }
		void reset_parser() { m_headers_seen = false; }

	private:
		std::vector<char> m_contents;
		std::vector<std::tuple<span, span>> m_field_list;
		dict_t m_dict;
		size_t m_last_line_end = 0;
		bool m_headers_seen{false};

		void rearrange();
		std::string_view get(span s) {
			return {m_contents.data() + s.offset(), s.length()};
		}
	};
}  // namespace tangle::msg
