// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std::literals;

namespace tangle {
	struct attr_pos {
		size_t name_start{};
		size_t start{};
		size_t stop{};
		std::string_view value{};
		static bool is(std::string_view attr_name,
		               std::string_view prelowered) noexcept;
	};

	struct elem {
		struct name_info {
			std::string_view value{};
			size_t start{};
			size_t stop{};
			bool autoclose{false};
			bool is(std::string_view prelowered) const noexcept;
		} name;
		std::unordered_map<std::string_view, attr_pos> attrs{};
	};

	std::vector<elem> html_split(std::string_view text);
	std::string attr_encode(std::string_view text);
	std::string attr_decode(std::string_view text);
}  // namespace tangle
