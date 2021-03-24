// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-dtd
// build/bin/html-dtd tools/lookup/html.dtd browser

#pragma once

namespace tangle::browser {
	enum class elem_flag : unsigned {
		none = 0x0000u,
		empty = 0x0001u,
		text_only = 0x0002u,
		any = 0x0004u,
		metadata = 0x0008u,
		flow = 0x0010u,
		flow_only = 0x0020u,
		sectioning = 0x0040u,
		heading = 0x0080u,
		phrasing = 0x0100u,
		interactive = 0x0200u,
		scripting = 0x0400u,
	};

	inline constexpr elem_flag operator|(elem_flag lhs, elem_flag rhs) noexcept {
		return static_cast<elem_flag>(static_cast<unsigned>(lhs) |
		                              static_cast<unsigned>(rhs));
	}

	inline constexpr elem_flag operator&(elem_flag lhs, elem_flag rhs) noexcept {
		return static_cast<elem_flag>(static_cast<unsigned>(lhs) &
		                              static_cast<unsigned>(rhs));
	}

	inline constexpr elem_flag operator~(elem_flag lhs) noexcept {
		return static_cast<elem_flag>(~static_cast<unsigned>(lhs));
	}
} // namespace tangle::browser
// clang-format on
