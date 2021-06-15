// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "lookup.hh"
#include <algorithm>
#include <array>
#include <iomanip>
#include <random>
#include <set>

using namespace std::literals;

void lookup_tree::quick_print(indent in) const {
	if (leaf) {
		std::cout << in << ">>> " << leaf->second << ' ' << leaf->first << '\n';
	}
	if (children.empty()) return;
	std::cout << in << "{\n";
	for (auto& [key, child] : children) {
		std::cout << in + 1 << '[' << key << "]\n";
		child.quick_print(in + 2);
	}
	std::cout << in << "}\n";
}

void lookup_tree::insert(std::string_view word,
                         std::string const& comment,
                         std::string const& value) {
	if (word.empty()) {
		leaf = {comment, value};
		return;
	}

	children[{word.front()}].insert(word.substr(1), comment, value);
}

lookup_tree lookup_tree::gather_endings() const {
	lookup_tree copy{{}, leaf};

	if (children.size() == 1) {
		auto& [key, child] = *children.begin();
		auto child_copy = child.gather_endings();

		if (!child_copy.leaf && child_copy.children.size() == 1) {
			auto& [child_key, grandchild] = *child_copy.children.begin();

			if (grandchild.leaf_only()) {
				copy.children[key + child_key] = std::move(grandchild);

				return copy;
			}
		}

		copy.children[key] = std::move(child_copy);
		return copy;
	}

	for (auto& [key, child] : children) {
		copy.children[key] = child.gather_endings();
	}

	return copy;
}

void lookup_tree::print(std::ostream& out, size_t level, indent in) const {
	if (leaf) {
		out << in << "ON_LEAF_NOBR(" << level << ", " << leaf->second
		    << ");  // " << leaf->first << '\n';
	}

	if (children.size() == 1) {
		auto& [key, child] = *children.begin();
		if (child.leaf_only()) {
			out << in << "ON_SUFFIX(" << level << ", \"" << key << "\", "
			    << child.leaf->second << ");  // " << child.leaf->first << '\n';
			return;
		}
	}

	out << in << "CHAR_AT(" << level << ")\n";
	for (auto& [key, child] : children) {
		out << in + 1 << "CHAR('" << key << "')";

		if (child.leaf && child.children.empty()) {
			out << " ON_LEAF(" << level + 1 << ", " << child.leaf->second
			    << ");  // " << child.leaf->first << '\n';
			continue;
		}

		if (!child.leaf && child.children.size() == 1) {
			auto& [subkey, sub] = *child.children.begin();
			if (sub.leaf_only()) {
				out << " ON_SUFFIX(" << level + 1 << ", \"" << subkey << "\", "
				    << sub.leaf->second << ");  // " << sub.leaf->first << '\n';
				continue;
			}
		}

		out << '\n';
		child.print(out, level + 1, in + 2);
	}
	if (level)
		out << in << "CHAR_END()\n";
	else
		out << in << "CHAR_END_NOBR()\n";
}

void lookup_tree::function(std::ostream& out,
                           std::string_view rtype,
                           std::string name,
                           std::string final_value,
                           sensitivity case_sensitivity) const {
	if (case_sensitivity == sensitivity::on) {
		out << "#define CHAR_AT(NO) if (length > NO) { switch "
		       "(name[NO]) {\n";
	} else {
		out << "#define CHAR_AT(NO) if (length > NO) { switch "
		       "(std::tolower(static_cast<unsigned char>(name[NO]))) {\n";
	}
	out << R"(#define CHAR(C) case C:
#define CHAR_END_NOBR() }}
#define CHAR_END() }} break;
)";
	if (case_sensitivity == sensitivity::on) {
		out << "#define ON_SUFFIX(NO, suffix, result) \\\n  if "
		       "(name.substr(NO) == suffix ## sv) { return result; } "
		       "\\\n  break\n";
	} else {
		out << "#define ON_SUFFIX(NO, suffix, result) \\\n  if "
		       "(equal_ignore_case(name.substr(NO), suffix ## sv)) { "
		       "return result; } \\\n  break\n";
	}
	out << R"(#define ON_LEAF_NOBR(NO, result) if (length == NO) return result
#define ON_LEAF(NO, result) ON_LEAF_NOBR(NO, result); break

)";

	out << rtype << ' ' << name << "(std::string_view name) {\n";
	out << lvl
	    << "auto const length = name.length();\n"
	       "\n";

	print(out);

	out << '\n' << lvl << "return " << final_value << ";\n";
	out << R"(}

#undef CHAR_AT
#undef CHAR
#undef CHAR_END_NOBR
#undef CHAR_END
#undef ON_SUFFIX
#undef ON_LEAF_NOBR
#undef ON_LEAF

)";
}

struct cxx_string {
	std::string_view val;

	friend std::ostream& operator<<(std::ostream& out, cxx_string const& data) {
		if (data.val.empty()) return out << "{}";
		out << '"';
		for (auto c : data.val) {
			auto const uc = static_cast<unsigned char>(c);
			switch (c) {
				case '\\':
					out << "\\\\";
					break;
				case '\"':
					out << "\\\"";
					break;
				case '\a':
					out << "\\a";
					break;
				case '\b':
					out << "\\b";
					break;
				case '\f':
					out << "\\f";
					break;
				case '\n':
					out << "\\n";
					break;
				case '\r':
					out << "\\r";
					break;
				case '\t':
					out << "\\t";
					break;
				case '\v':
					out << "\\v";
					break;
				default:
					if (std::isprint(uc)) {
						out << c;
					} else {
						out << "\\x" << std::hex << std::setw(2)
						    << std::setfill('0') << static_cast<unsigned>(uc)
						    << std::dec;
					}
					break;
			}
		}
		return out << "\"sv";
	}
};

void lookup_tree::test_good(std::ostream& out,
                            std::string const& prefix) const {
	if (leaf) {
		out << lvl << arr_indent << "{" << cxx_string{prefix} << ", "
		    << leaf->second << "},\n";
	}

	for (auto& [key, child] : children) {
		child.test_good(out, prefix + key);
	}
}

std::mt19937& rnd() {
	static std::mt19937 mt{[] {
		std::array<int, std::mt19937::state_size> seed_data;
		std::random_device r;
		std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 mt_inner{seq};
		return mt_inner;
	}()};
	return mt;
}

std::optional<std::string> choose_except(std::set<char> const& except = {}) {
	static constexpr auto letters_digits =
	    "0123456789"
	    "abcdefghijklmnopqrstuvwxyz"
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"sv;
	using dice_t = std::uniform_int_distribution<size_t>;
	dice_t dice{0, letters_digits.size() - 1};

	std::string result{};
	result.reserve(4);

	if (except.empty()) {
		result.push_back(letters_digits[dice(rnd())]);
	} else {
		std::vector<char> allowed{};
		allowed.reserve(letters_digits.size());
		for (auto c : letters_digits) {
			if (except.count(c) != 0) continue;
			allowed.push_back(c);
		}

		if (allowed.empty()) return std::nullopt;

		result.push_back(allowed[dice_t{0, allowed.size() - 1}(rnd())]);
	}

	for (int i = 0; i < 3; ++i)
		result.push_back(letters_digits[dice(rnd())]);

	return std::optional{std::move(result)};
}

void print_bad(std::ostream& out,
               std::string const& prefix,
               std::set<char> const& except,
               bool has_leaf,
               bool has_children) {
	auto suff = choose_except(except);
	if (!suff) return;
	out << lvl << arr_indent << '{' << cxx_string{prefix + *suff} << "},";

	if (has_leaf || has_children) {
		out << "  // fails ";
		if (has_leaf) out << "leaf";
		if (has_leaf && has_children) out << " and ";
		if (has_children) out << "tail";
		out << " of " << cxx_string{prefix};
	}
	out << "\n";
}

void lookup_tree::test_bad(std::ostream& out, std::string const& prefix) const {
	std::set<char> except{};
	auto has_children = false;
	for (auto& [key, _] : children) {
		has_children = true;
		except.insert(key.front());
	}
	print_bad(out, prefix, except, !!leaf, has_children);

	for (auto& [key, child] : children) {
		child.test_bad(out, prefix + key);
	}
}

void lookup_tree::tests(std::ostream& out,
                        std::string_view suite_name,
                        std::string_view expected_type,
                        std::string_view function_name,
                        std::string_view failing_value,
                        bool is_c_string) const {
	out << lvl << "struct " << suite_name << "_data {\n"
	    << lvl_2 << "std::string_view name;\n"
	    << lvl_2 << expected_type << " expected{" << failing_value << "};\n"
	    << lvl_2 << "friend std::ostream& operator<<(std::ostream& out, "
	    << suite_name << "_data const& data) {\n"
	    << lvl___3 << "return out << data.name;\n"
	    << lvl_2 << "}\n"
	    << lvl << "};\n\n";

	out << lvl << "class " << suite_name << ": public ::testing::TestWithParam<"
	    << suite_name << "_data>{};\n"
	    << lvl << "TEST_P(" << suite_name << ", lookup) {\n"
	    << lvl_2 << "auto& param = GetParam();\n";

	out << lvl_2 << "auto actual = browser::" << function_name
	    << "(param.name);\n\n";

	if (is_c_string)
		out << lvl_2 << "ASSERT_STREQ(param.expected, actual);\n";
	else
		out << lvl_2 << "ASSERT_EQ(param.expected, actual);\n";

	out << lvl << "}\n\n";

	out << lvl << "static constexpr " << suite_name << "_data names[] = {\n";
	test_good(out);
	out << lvl << "};\n\n";

	out << lvl << "static constexpr " << suite_name << "_data nosuch[] = {\n";
	if (!leaf) {
		out << lvl << arr_indent << "{\"\"sv},\n";
	}
	test_bad(out);
	out << lvl << "};\n\n";

	out << lvl << "INSTANTIATE_TEST_SUITE_P(names,\n"
	    << lvl << "                         " << suite_name << ",\n"
	    << lvl << "                         ::testing::ValuesIn(names));\n";
	out << lvl << "INSTANTIATE_TEST_SUITE_P(errors,\n"
	    << lvl << "                         " << suite_name << ",\n"
	    << lvl << "                         ::testing::ValuesIn(nosuch));\n";
}
