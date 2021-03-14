// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <cctype>
#include <string_view>

namespace tangle {
	// Under libstdc++, std::isspace is visible as an overload set
	// consisting of more, than one function. This alias narrows this down
	// to single-function overload set available for base_parser::skip<>.
	//
	// Additionally, this gives us the possibility to slap a noexcept on this
	// one.
	inline int is_space(int c) noexcept { return std::isspace(c); }

	struct base_parser {
		std::string_view text;
		char const* pos = text.data();
		char const* end = pos + text.size();

		base_parser(std::string_view text) : text{text} {}

		bool eof() const noexcept { return pos == end; }

		template <typename Pred>
		void skip(Pred pred) {
			while (pos < end && pred(static_cast<unsigned char>(*pos)))
				++pos;
		}

		template <typename Pred>
		void skip_until(Pred pred) {
			while (pos < end && !pred(static_cast<unsigned char>(*pos)))
				++pos;
		}

		template <char... C>
		void look_for() {
			skip_until([](int c) { return ((C == c) || ...); });
		}

		template <char... C>
		std::string_view text_until() {
			return substr<base_parser>(
			    [](auto& self) { self.template look_for<C...>(); });
		}

		template <typename Final, typename Oper>
		std::string_view substr(Oper op) {
			auto start = pos;
			op(*static_cast<Final*>(this));
			return {start, static_cast<size_t>(pos - start)};
		}

		void skip_ws() { skip(is_space); }

		bool peek(char c) { return pos < end && *pos == c; }
		bool get(char c) {
			auto const result = peek(c);
			if (result) ++pos;
			return result;
		}
	};
}  // namespace tangle
