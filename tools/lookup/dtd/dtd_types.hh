// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

enum class builtin_category : unsigned {
	empty = 1,
	text = 2,
	any = 4,
};

inline unsigned operator|(unsigned flags, builtin_category cat) {
	return flags | static_cast<unsigned>(cat);
}

inline unsigned operator&(unsigned flags, builtin_category cat) {
	return flags & static_cast<unsigned>(cat);
}

inline builtin_category operator~(builtin_category cat) {
	return static_cast<builtin_category>(~static_cast<unsigned>(cat));
}

struct elem_info {
	unsigned cat{}, disallowed_cat{}, allowed_cat{};
	std::set<std::string> disallowed{};
	std::set<std::string> allowed{};

	static int cmp(std::set<std::string> const& lhs,
	               std::set<std::string> const& rhs) noexcept;
	static bool eq(std::set<std::string> const& lhs,
	               std::set<std::string> const& rhs) noexcept;
	friend bool operator<(elem_info const& lhs, elem_info const& rhs) noexcept;
	friend bool operator==(elem_info const& lhs, elem_info const& rhs) noexcept;
};

struct categories {
	struct bit {
		unsigned value;
		size_t use_count;
	};
	std::map<std::string, bit> name2bit;
	std::vector<std::string> bit2name;

	unsigned lookup(std::string_view name_view, unsigned use = 1);
	static void remove(unsigned& flags, unsigned bit);
	void remove_unused(std::map<std::string, elem_info>& element_categories);
	void print(std::ostream& out,
	           unsigned flags,
	           std::set<std::string> const& names) const;
	void print(std::ostream& out, unsigned flags) const;
	void print(std::ostream& out) const;
};
