// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/browser/html_dtd_flags.hpp>
#include <tangle/browser/html_split.hpp>
#include <iostream>
#include <string_view>
#include <vector>

namespace tangle::browser {
	struct childset {
		elem_flag flags{};
		std::vector<std::string_view> names{};

		bool matches(elem_flag categories, std::string_view name) const
		    noexcept {
			if ((categories & flags) != elem_flag::none) return true;
			for (auto& prelowered : names) {
				if (equal_ignore_case(name, prelowered)) return true;
			}
			return false;
		}
	};

	struct element_info;

	element_info const& get_info(std::string_view name) noexcept;
	unsigned element_index(std::string_view name);

	struct element_info {
		elem_flag flags{};
		childset allowed{};
		childset disallowed{};

		static constexpr elem_flag EMPTYish =
		    elem_flag::empty | elem_flag::text_only;

		bool allows(std::string_view name) const noexcept {
			if ((flags & EMPTYish) != elem_flag::none) return false;
			return allows(get_info(name).flags, name);
		}

		bool allows(elem_flag categories, std::string_view name) const
		    noexcept {
			if ((flags & EMPTYish) != elem_flag::none) return false;
			if (disallowed.matches(categories, name)) return false;
			if ((flags & elem_flag::any) == elem_flag::any) return true;
			return allowed.matches(categories, name);
		}
	};
}  // namespace tangle::browser
