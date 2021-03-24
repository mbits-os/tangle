// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>

struct elem_ref {
	bool allowed;
	std::string name;

	friend bool operator<(elem_ref const& lhs, elem_ref const& rhs) noexcept {
		if (lhs.allowed != rhs.allowed) return lhs.allowed;
		return lhs.name < rhs.name;
	}

	friend bool operator==(elem_ref const& lhs, elem_ref const& rhs) noexcept {
		return lhs.allowed == rhs.allowed && lhs.name == lhs.name;
	}
};

struct categories;
struct elem_info;
class buffered_file;

struct dtd_parser {
	categories& bit_info;
	std::map<std::string, elem_info>& element_categories;
	static std::string_view token(char const*& cur, char const* end);
	static unsigned parse_group(char const*& cur,
	                            char const* end,
	                            bool allowed,
	                            std::set<elem_ref>& refs);
	static std::set<elem_ref> parse_sgml(std::string_view children,
	                                     unsigned* flags);
	static bool parse_section_name(std::string_view line, bool* in_categories);
	void parse_category(std::string_view line);
	void parse_element(std::string_view line);
	void parse(buffered_file& input);
};
