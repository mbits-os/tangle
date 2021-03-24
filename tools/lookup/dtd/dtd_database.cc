// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "dtd_database.hh"
#include "dtd_parser.hh"
#include <algorithm>

using namespace std::literals;

struct references {
	size_t lookup(elem_info const& info, std::string const& name) {
		auto it = elem2id.lower_bound(info);
		if (it != elem2id.end() && it->first == info) {
			it->second.second.insert(name);
			return it->second.first;
		}

		size_t id = elem2id.size();
		elem2id.insert(it, {info, {id, {name}}});
		return id;
	}

	std::map<elem_info, std::pair<size_t, std::set<std::string>>> elem2id;
};

void database::load(buffered_file& input) {
	bit_info.lookup("empty"sv);
	bit_info.lookup("text_only"sv);
	bit_info.lookup("any"sv);

	std::map<std::string, elem_info> element_categories{};
	dtd_parser{bit_info, element_categories}.parse(input);
	bit_info.remove_unused(element_categories);

	references refs{};
	std::map<std::string, size_t> elements{};
	for (auto& [name, info] : element_categories) {
		elements[name] = refs.lookup(info, name);
	}
	element_categories.clear();

	items.reserve(refs.elem2id.size());
	std::transform(
	    refs.elem2id.begin(), refs.elem2id.end(), std::back_inserter(items),
	    [](auto& pair) -> std::tuple<size_t, std::set<std::string>, elem_info> {
		    return {
		        pair.second.first,
		        std::move(pair.second.second),
		        std::move(pair.first),
		    };
	    });
	refs.elem2id.clear();

	std::sort(items.begin(), items.end());

	for (auto& [name, id] : elements) {
		lookup.insert(name, name, std::to_string(id));
		if (name == "img"sv) {
			auto& db_ref = std::get<2>(items[id]);
			auto const non_void = db_ref.cat & ~builtin_category::empty;
			unknown_element_category |= non_void;
		}
	}

	lookup = lookup.gather_endings();
}

void database::print_header(std::ostream& out,
                            std::string_view file_header) const {
	out << file_header;
	out << R"(#pragma once

namespace tangle::browser {
)";

	bit_info.print(out);

	out << R"(} // namespace tangle::browser
// clang-format on
)";
}

void database::print_code(std::ostream& out,
                          std::string_view file_header) const {
	out << file_header;
	out << R"(#include <tangle/browser/html_dtd.hpp>
#include <cctype>
#include <string>

namespace tangle::browser {
)";

	out << "static element_info const& get_info(unsigned index) {\n";
	out << lvl << "static element_info const infos[] = {\n";
	size_t index = 0;
	for (auto& [id, used_by, info] : items) {
		auto const ndx = std::to_string(index++);

		out << lvl_2 << "{\n";

		out << lvl___3 << "// " << ndx << ": ";
		size_t length = lvl___3.depth() + 5 + ndx.size();
		bool first = true;
		for (auto const& name : used_by) {
			length += (first ? 0 : 2) + name.size();
			if (!first && length > 79) {
				out << ",\n" << lvl___3 << "// ";
				length = lvl___3.depth() + 3 + name.size();
				first = true;
			}

			if (first)
				first = false;
			else
				out << ", ";

			out << name;
		}
		out << '\n';

		out << lvl___3;
		bit_info.print(out, info.cat);
		out << ",\n";

		bool has_allowed = info.allowed_cat != 0 || !info.allowed.empty();
		bool has_disallowed =
		    info.disallowed_cat != 0 || !info.disallowed.empty();

		if (has_allowed || has_disallowed) {
			out << lvl___3;
			bit_info.print(out, info.allowed_cat, info.allowed);
			out << ",\n";
		}
		if (has_disallowed) {
			out << lvl___3;
			bit_info.print(out, info.disallowed_cat, info.disallowed);
			out << ",\n";
		}
		out << lvl_2 << "},\n";
	}
	out << lvl_2 << "{\n" << lvl___3;
	bit_info.print(out, unknown_element_category);
	out << '\n' << lvl_2 << "},\n";
	out << lvl
	    << "};\n"
	       "\n";
	out << lvl
	    << "return infos[index];\n"
	       "}\n"
	       "\n";

	lookup.function(out, "unsigned", "element_index",
	                std::to_string(items.size()), sensitivity::off);

	out << R"(element_info const& get_info(std::string_view name) noexcept {
)" << lvl
	    << R"(return get_info(element_index(name));
}

} // namespace tangle::browser
// clang-format on
)";
}

void database::print_tests(std::ostream& out,
                           std::string_view file_header) const {
	out << file_header;
	out << R"(#include <tangle/browser/html_dtd.hpp>
#include <gtest/gtest.h>

namespace tangle::browser::testing {
)";
	lookup.tests(out, "html_element", "unsigned", "element_index",
	             std::to_string(items.size()), false);
	out << R"(}  // namespace tangle::browser::testing
// clang-format on
)";
}
