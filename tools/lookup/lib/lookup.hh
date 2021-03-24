// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <map>
#include <optional>
#include <string>
#include <utility>

#include "indent.hh"

enum class sensitivity { on, off };

struct lookup_tree {
	std::map<std::string, lookup_tree> children;
	std::optional<std::pair<std::string, std::string>> leaf;

	bool leaf_only() const noexcept { return leaf && children.empty(); }

	void insert(std::string_view word,
	            std::string const& comment,
	            std::string const& value);
	lookup_tree gather_endings() const;

	void quick_print(indent in = {}) const;

	void print(std::ostream& out, size_t level = 0, indent in = {1}) const;
	void function(std::ostream& out,
	              std::string_view rtype,
	              std::string name,
	              std::string final_value,
	              sensitivity case_sensitivity) const;

	void test_good(std::ostream& out, std::string const& prefix = {}) const;
	void test_bad(std::ostream& out, std::string const& prefix = {}) const;
	void tests(std::ostream& out,
	           std::string_view suite_name,
	           std::string_view expected_type,
	           std::string_view function_name,
	           std::string_view failing_value,
	           bool is_c_string) const;
};
