// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <cstddef>
#include <iostream>

struct tab_indent {
	static constexpr size_t size = 4;
	size_t lvl{};

	inline constexpr size_t depth() const noexcept { return lvl * size; }

	friend tab_indent operator+(tab_indent lhs, unsigned rhs) noexcept {
		return {lhs.lvl + rhs};
	}

	friend std::ostream& operator<<(std::ostream& out, tab_indent in) {
		for (size_t i = 0; i < in.lvl; ++i)
			out << '\t';

		return out;
	}
};

template <size_t Size>
struct space_indent {
	static constexpr size_t size = Size;
	size_t lvl{};

	inline constexpr size_t depth() const noexcept { return lvl * size; }

	friend space_indent<Size> operator+(space_indent<Size> lhs,
	                                    unsigned rhs) noexcept {
		return {lhs.lvl + rhs};
	}

	template <size_t>
	static constexpr char space() noexcept {
		return ' ';
	}

	template <size_t... I>
	static std::string_view space_indent_string(std::index_sequence<I...>) {
		static constexpr char buffer[] = {space<I>()..., 0};
		static constexpr std::string_view result{buffer, sizeof...(I)};

		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, space_indent<Size> in) {
		for (size_t i = 0; i < in.lvl; ++i)
			out << space_indent_string(std::make_index_sequence<Size>{});

		return out;
	}
};

// using indent = space_indent<2>;
using indent = tab_indent;

static constexpr indent lvl{1};
static constexpr indent lvl_2{2};
static constexpr indent lvl___3{3};
static constexpr indent lvl_____4{4};

static constexpr space_indent<indent::size> arr_indent{1};
