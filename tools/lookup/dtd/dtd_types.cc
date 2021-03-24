// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "dtd_types.hh"
#include "indent.hh"
#include <iomanip>
#include <iostream>

int elem_info::cmp(std::set<std::string> const& lhs,
                   std::set<std::string> const& rhs) noexcept {
	auto lhs_it = lhs.begin();
	auto rhs_it = rhs.begin();
	auto const lhs_size = lhs.size();
	auto const rhs_size = rhs.size();
	auto const size = std::min(lhs_size, rhs_size);

	auto lhs_end = std::next(lhs_it, static_cast<ptrdiff_t>(size));

	for (; lhs_it != lhs_end; ++lhs_it, ++rhs_it) {
		auto& lhs_term = *lhs_it;
		auto& rhs_term = *rhs_it;
		if (lhs_term < rhs_term) return -1;
		if (rhs_term < lhs_term) return 1;
	}

	return lhs_size < rhs_size ? -1 : lhs_size > rhs_size ? 1 : 0;
}

bool elem_info::eq(std::set<std::string> const& lhs,
                   std::set<std::string> const& rhs) noexcept {
	if (lhs.size() != rhs.size()) return false;
	auto it = lhs.begin();
	for (auto const& rhs_term : rhs) {
		auto const& lhs_term = *it++;
		if (!(lhs_term == rhs_term)) return false;
	}
	return true;
}

bool operator<(elem_info const& lhs, elem_info const& rhs) noexcept {
	if (lhs.cat != rhs.cat) return lhs.cat < rhs.cat;
	if (lhs.allowed_cat != rhs.allowed_cat)
		return lhs.allowed_cat < rhs.allowed_cat;
	if (lhs.disallowed_cat != rhs.disallowed_cat)
		return lhs.disallowed_cat < rhs.disallowed_cat;

	if (auto const cmp_v = elem_info::cmp(lhs.allowed, rhs.allowed); cmp_v != 0)
		return cmp_v < 0;

	auto const cmp_v = elem_info::cmp(lhs.disallowed, rhs.disallowed);
	return cmp_v < 0;
}

bool operator==(elem_info const& lhs, elem_info const& rhs) noexcept {
	return lhs.cat == rhs.cat && lhs.allowed_cat == rhs.allowed_cat &&
	       lhs.disallowed_cat == rhs.disallowed_cat &&
	       elem_info::eq(lhs.allowed, rhs.allowed) &&
	       elem_info::eq(lhs.disallowed, rhs.disallowed);
}

unsigned categories::lookup(std::string_view name_view, unsigned use) {
	std::string name{name_view.data(), name_view.size()};

	auto it = name2bit.lower_bound(name);
	if (it != name2bit.end() && it->first == name) {
		++it->second.use_count;
		return it->second.value;
	}

	auto const bit_value = static_cast<unsigned>(bit2name.size());
	bit2name.push_back(name);
	name2bit.insert(it, {name, {bit_value, use}});
	return bit_value;
}

void categories::remove(unsigned& flags, unsigned bit) {
	if (flags < (1u << bit)) return;
	auto const lower_mask = (1u << bit) - 1u;
	auto const upper_mask = ~((1u << (bit + 1)) - 1u);
	auto const lower = flags & lower_mask;
	auto const upper = flags & upper_mask;
	flags = lower | (upper >> 1);
}

void categories::remove_unused(
    std::map<std::string, elem_info>& element_categories) {
	for (auto& [_, bit] : name2bit) {
		if (bit.use_count) continue;
		for (unsigned index = bit.value + 1; index < bit2name.size(); ++index) {
			auto& ref = *name2bit.find(bit2name[index]);
			--ref.second.value;
		}
		bit2name.erase(std::next(bit2name.begin(), bit.value));

		for (auto& [__, info] : element_categories) {
			remove(info.cat, bit.value);
			remove(info.allowed_cat, bit.value);
			remove(info.disallowed_cat, bit.value);
		}
	}
}

void categories::print(std::ostream& out,
                       unsigned flags,
                       std::set<std::string> const& names) const {
	if (!flags && names.empty()) {
		out << "{}";
		return;
	}

	out << "{";
	print(out, flags);
	if (!names.empty()) {
		out << ", {";
		bool first = true;
		for (auto& name : names) {
			if (first)
				first = false;
			else
				out << ", ";
			out << '"' << name << '"';
		}
		out << '}';
	}
	out << '}';
}

void categories::print(std::ostream& out, unsigned flags) const {
	if (flags == 0) {
		out << "elem_flag::none";
		return;
	}

	auto value = 1u;
	bool first = true;
	for (auto const& name : bit2name) {
		auto const current_value = value;
		value *= 2;
		if ((flags & current_value) == 0) continue;
		flags &= ~current_value;
		if (first)
			first = false;
		else
			out << " | ";
		out << "elem_flag::" << name;
	}
	if (flags) {
		if (!first) out << " | ";
		out << std::hex << "0x" << flags << std::dec;
	}
}

void categories::print(std::ostream& out) const {
	out << std::hex;

	out << lvl << "enum class elem_flag : unsigned {\n";
	out << lvl_2 << "none = 0x0000u,\n";
	auto value = 1;
	for (auto const& name : bit2name) {
		out << lvl_2 << name << " = 0x" << std::setw(4) << std::setfill('0')
		    << value << "u,\n";
		value *= 2;
	}
	out << lvl
	    << "};\n"
	       "\n"
	    << lvl
	    << "inline constexpr elem_flag operator|(elem_flag lhs, elem_flag rhs) "
	       "noexcept {\n"
	    << lvl_2
	    << "return static_cast<elem_flag>(static_cast<unsigned>(lhs) |\n"
	    << lvl_2
	    << "                              static_cast<unsigned>(rhs));\n"
	    << lvl
	    << "}\n"
	       "\n"
	    << lvl
	    << "inline constexpr elem_flag operator&(elem_flag lhs, elem_flag rhs) "
	       "noexcept {\n"
	    << lvl_2
	    << "return static_cast<elem_flag>(static_cast<unsigned>(lhs) &\n"
	    << lvl_2
	    << "                              static_cast<unsigned>(rhs));\n"
	    << lvl
	    << "}\n"
	       "\n"
	    << lvl
	    << "inline constexpr elem_flag operator~(elem_flag lhs) noexcept {\n"
	    << lvl_2
	    << "return static_cast<elem_flag>(~static_cast<unsigned>(lhs));\n"
	    << lvl << "}\n"
	    << std::dec;
}
