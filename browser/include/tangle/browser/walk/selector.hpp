// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <tangle/browser/html_split.hpp>

namespace tangle::browser::walk {

	enum class attr_match {
		unused,
		exists,
		exactly,           // =
		contains_token,    // ~=
		dash_starts_with,  // |=
		starts_with,       // ^=
		ends_with,         // $=
		contains,          // *=
	};

	struct attr_selector {
		std::string name;
		std::string value;
		attr_match function;
	};

	struct class_token_selector {
		std::string value;
		attr_match function;
	};

	struct selector {
		std::string name{};
		std::string id{};
		std::vector<class_token_selector> class_list{};
		std::vector<attr_selector> attr_list{};

		bool matches(elem const& element) const;

		friend std::ostream& operator<<(std::ostream& out,
		                                selector const& descr);

		static std::vector<std::string> make_token_list(
		    std::string_view classes);
		static selector parse(std::string_view text);
		static selector matching(elem const& element);

	private:
		bool match_class_list(std::string_view attr_value) const noexcept;
		size_t match_attr(std::string_view attr_name,
		                  std::string_view attr_value) const noexcept;
	};
}  // namespace tangle::browser::walk
