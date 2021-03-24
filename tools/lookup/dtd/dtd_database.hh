// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include "dtd_types.hh"
#include "file.hh"
#include "lookup.hh"

using element_info_list =
    std::vector<std::tuple<size_t, std::set<std::string>, elem_info>>;

struct database {
	categories bit_info{};
	element_info_list items{};
	lookup_tree lookup{};
	unsigned unknown_element_category = 0u | builtin_category::any;

	void load(buffered_file& input);
	void print_header(std::ostream& out, std::string_view file_header) const;
	void print_code(std::ostream& out, std::string_view file_header) const;
	void print_tests(std::ostream& out, std::string_view file_header) const;
};
