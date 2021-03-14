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
		void skip(Pred pred) noexcept(
		    noexcept(pred(std::declval<unsigned char>()))) {
			while (pos < end && pred(static_cast<unsigned char>(*pos)))
				++pos;
		}

		template <typename Pred>
		void skip_until(Pred pred) noexcept(
		    noexcept(pred(std::declval<unsigned char>()))) {
			while (pos < end && !pred(static_cast<unsigned char>(*pos)))
				++pos;
		}

		template <char... C>
		void look_for() noexcept {
			skip_until([](int c) noexcept { return ((C == c) || ...); });
		}

		template <char... C>
		std::string_view text_until() noexcept {
			return substr<base_parser>([](auto& self) noexcept {
				self.template look_for<C...>();
			});
		}

		template <typename Final, typename Oper>
		std::string_view substr(Oper op) noexcept(
		    noexcept(op(std::declval<Final&>()))) {
			auto start = pos;
			op(*static_cast<Final*>(this));
			return {start, static_cast<size_t>(pos - start)};
		}

		void skip_ws() noexcept { skip(is_space); }

		bool peek(char c) noexcept { return pos < end && *pos == c; }
		bool get(char c) noexcept {
			auto const result = peek(c);
			if (result) ++pos;
			return result;
		}
	};
}  // namespace tangle
