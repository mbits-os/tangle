// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "dtd_parser.hh"
#include "dtd_types.hh"
#include "file.hh"
#include "str.hh"
#include <cctype>
#include <vector>

using namespace std::literals;

inline bool isspace(char c) noexcept {
	return std::isspace(static_cast<unsigned char>(c));
}

inline bool ispunct(char cur) {
	switch (cur) {
		case '(':
		case ')':
		case '+':
		case '?':
		case '*':
		case '-':
		case '|':
		case ',':
		case '&':
			return true;
	}
	return false;
}

std::string_view dtd_parser::token(char const*& cur, char const* end) {
	if (cur == end) return {};

	if (isspace(*cur)) {
		while (cur != end && isspace(*cur))
			++cur;
		return " "sv;
	}

	if (ispunct(*cur)) return {cur++, 1};

	auto start = cur;
	while (cur != end && !isspace(*cur) && !ispunct(*cur))
		++cur;
	auto length = static_cast<size_t>(cur - start);
	return {start, length};
}

unsigned dtd_parser::parse_group(char const*& cur,
                                 char const* end,
                                 bool allowed,
                                 std::set<elem_ref>& refs) {
	bool was_space = false;
	bool was_possible_neg = false;
	unsigned flags{};

	for (auto next = token(cur, end); !next.empty() && next != ")"sv;
	     next = token(cur, end)) {
		if (next == " "sv) {
			was_space = true;
			continue;
		}
		if (next == "-"sv) {
			if (was_space) was_possible_neg = true;
			was_space = false;
			continue;
		}

		was_space = false;
		if (next == "("sv) {
			auto sub_allowed = allowed;
			if (was_possible_neg) sub_allowed = false;
			was_possible_neg = false;
			flags |= parse_group(cur, end, sub_allowed, refs);
			continue;
		}
		was_possible_neg = false;

		if (next.size() == 1) continue;

		if (next == "ANY"sv) {
			flags = flags | builtin_category::any;
			continue;
		}

		if (next == "EMPTY"sv) {
			flags = flags | builtin_category::empty;
			continue;
		}

		if (next == "#PCDATA"sv || next == "CDATA"sv || next == "RCDATA"sv) {
			flags = flags | builtin_category::text;
			continue;
		}

		refs.insert({allowed, {next.data(), next.size()}});
	}

	return flags;
}

std::set<elem_ref> dtd_parser::parse_sgml(std::string_view children,
                                          unsigned* flags) {
	std::set<elem_ref> result{};
	auto cur = children.data();
	*flags = parse_group(cur, cur + children.size(), true, result);
	return result;
}

bool dtd_parser::parse_section_name(std::string_view line,
                                    bool* in_categories) {
	if (line.substr(0, 5) != "<!--="sv) return false;
	line = line.substr(5);
	while (!line.empty() && line.front() == '=')
		line = line.substr(1);

	if (line.substr(line.size() - 3) == "-->"sv) {
		line = line.substr(0, line.size() - 3);
	}

	auto length = line.size();
	while (length && line[length - 1] == '=')
		--length;
	line = strip(line.substr(0, length));

	*in_categories = line == "Element categories"sv;
	return true;
}

#define SKIP_WS()                                  \
	while (!line.empty() && isspace(line.front())) \
	line = line.substr(1)

void dtd_parser::parse_category(std::string_view line) {
	static constexpr auto entity = "<!ENTITY"sv;
	if (line.substr(0, entity.size()) != entity) return;
	line = line.substr(entity.size());

	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	if (line.empty() || line.front() != '%') return;
	line = line.substr(1);

	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	auto size = line.size();
	size_t index = 0;
	while (size > index && !isspace(line[index]))
		++index;

	auto cat_name = line.substr(0, index);
	if (cat_name == "all"sv) return;
	auto const HTML_all = false;
	// cat_name == "HTML.all"sv;

	line = line.substr(index);
	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	if (line.empty() || line.front() != '"') return;
	line = line.substr(1);

	size = line.size();
	index = 0;
	while (size > index && line[index] != '"')
		++index;
	auto elems = split_sv('|', line.substr(0, index));

	auto const bit = HTML_all ? 0 : bit_info.lookup(cat_name, 0);
	auto const shifted = HTML_all ? 0u : (1u << bit);

	for (auto const& elem : elems) {
		std::string key;
		key.assign(strip(elem));
		element_categories[key].cat |= shifted;
	}
}

void dtd_parser::parse_element(std::string_view line) {
	static constexpr auto element = "<!ELEMENT"sv;
	if (line.substr(0, element.size()) != element) return;
	line = line.substr(element.size());

	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	auto size = line.size();
	size_t index = 0;
	while (size > index && !isspace(line[index]))
		++index;

	auto elem_name = line.substr(0, index);
	if (elem_name.empty()) return;

	std::vector<std::string> names{};
	if (elem_name.front() == '(') {
		elem_name = elem_name.substr(1);
		if (!elem_name.empty() && elem_name.back() == ')')
			elem_name = elem_name.substr(0, elem_name.size() - 1);

		names = split_s('|', elem_name);
	} else {
		names.emplace_back(elem_name.data(), elem_name.size());
	}

	std::vector<elem_info*> elems{};
	elems.reserve(names.size());
	for (auto const& name : names) {
		elems.push_back(&element_categories[name]);
	}

	line = line.substr(index);
	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	if (line.empty() || (line.front() != '-' && line.front() != 'O')) return;
	line = line.substr(1);

	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	if (line.empty() || (line.front() != '-' && line.front() != 'O')) return;
	line = line.substr(1);

	if (line.empty() || !isspace(line.front())) return;
	SKIP_WS();

	if (!line.empty() && line.back() == '>')
		line = line.substr(0, line.size() - 1);

	auto cat = 0u;
	auto children = parse_sgml(line, &cat);
	if (cat & builtin_category::text && !children.empty()) {
		cat = cat & ~builtin_category::text;
	}

	std::set<elem_ref> refs{};
	std::set<std::string> allowed{};
	std::set<std::string> disallowed{};

	for (auto& ref : children) {
		if (ref.name.front() == '%' && ref.name.back() == ';') {
			auto name = ref.name.substr(1, ref.name.size() - 2);
			refs.insert({ref.allowed, std::move(name)});
			continue;
		}
		(ref.allowed ? allowed : disallowed).insert(std::move(ref.name));
	}

	for (auto elem_ptr : elems) {
		unsigned allowed_cat{};
		unsigned disallowed_cat{};

		for (auto& ref : refs) {
			auto const bit = bit_info.lookup(ref.name);
			(ref.allowed ? allowed_cat : disallowed_cat) |= 1u << bit;
		}

		elem_ptr->cat |= cat;
		elem_ptr->allowed_cat = allowed_cat;
		elem_ptr->disallowed_cat = disallowed_cat;
		elem_ptr->allowed = allowed;
		elem_ptr->disallowed = disallowed;
	}
}

void dtd_parser::parse(buffered_file& input) {
	std::string line;

	bool in_categories = false;

	while (input.readline(line)) {
		auto view = strip(line);

		if (parse_section_name(view, &in_categories)) continue;

		if (in_categories) {
			parse_category(view);
			continue;
		}

		parse_element(view);
	}
}
