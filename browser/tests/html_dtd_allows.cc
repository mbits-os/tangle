// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/html_dtd.hpp>
#include <gtest/gtest.h>

namespace tangle::browser {
	std::ostream& PrintTo(elem_flag flags, std::ostream* outp) {
		auto& out = *outp;
		if (flags == elem_flag::none) return out << "HTML.all";

		static constexpr std::string_view names[] = {
		    "empty"sv,    "text_only"sv,   "any"sv,        "metadata"sv,
		    "flow"sv,     "flow_only"sv,   "sectioning"sv, "heading"sv,
		    "phrasing"sv, "interactive"sv, "scripting"sv,
		};

		auto first = true;
		auto test = 1u;
		for (auto& name : names) {
			auto const curr = test;
			test *= 2;
			if ((static_cast<unsigned>(flags) & curr) == 0) continue;
			if (first)
				first = false;
			else
				out << ", ";
			out << name;
		}

		return out;
	}  // namespace tangle::browser

	struct print_flags {
		elem_flag flag;
		friend std::ostream& operator<<(std::ostream& out,
		                                print_flags const& data) {
			return PrintTo(data.flag, &out);
		}
	};
}  // namespace tangle::browser

namespace tangle::browser::testing {
	struct html_element_allows_data {
		std::string_view first;
		std::string_view second;
		bool expected;
		friend std::ostream& operator<<(std::ostream& out,
		                                html_element_allows_data const& data) {
			return out << data.first << " > " << data.second;
		}
	};

	static constexpr auto std_flags =
	    elem_flag::any | elem_flag::empty | elem_flag::text_only;

	struct print_childset {
		childset const& set;
		friend std::ostream& operator<<(std::ostream& out,
		                                print_childset const& ref) {
			auto first = true;
			if (ref.set.flags != elem_flag::none) {
				out << print_flags{ref.set.flags};
				first = false;
			} else if (ref.set.names.empty()) {
				return out << "NOTHING";
			}

			for (auto& name : ref.set.names) {
				if (first)
					first = false;
				else
					out << ", ";
				out << '"' << name << '"';
			}

			return out;
		}
	};

	struct print_info {
		element_info const& info;
		friend std::ostream& operator<<(std::ostream& out,
		                                print_info const& ref) {
			if ((ref.info.flags & element_info::EMPTYish) != elem_flag::none) {
				return out << "should reject EVERYTHING";
			}

			out << "should ";
			if (ref.info.disallowed.flags != elem_flag::none ||
			    !ref.info.disallowed.names.empty()) {
				out << "reject " << print_childset{ref.info.disallowed}
				    << " and ";
			}

			if ((ref.info.flags & elem_flag::any) == elem_flag::any) {
				out << "accept ANYthing";
			} else {
				out << "accept " << print_childset{ref.info.allowed};
			}
			return out;
		}
	};

	struct info {
		html_element_allows_data const& param;
		friend std::ostream& operator<<(std::ostream& out, info const& ref) {
			auto& param = ref.param;
			return out << '"' << param.first << "\" "
			           << print_info{get_info(param.first)} << '\n'
			           << '"' << param.second << "\" is "
			           << print_flags{get_info(param.second).flags &
			                          ~std_flags};
		}
	};

	class html_element_allows
	    : public ::testing::TestWithParam<html_element_allows_data> {};
	TEST(html_element_allows, math) {
		ASSERT_EQ(elem_flag::metadata & ~elem_flag::metadata, elem_flag::none);
	}
	TEST_P(html_element_allows, by_name) {
		auto& param = GetParam();
		auto const actual = get_info(param.first).allows(param.second);

		ASSERT_EQ(param.expected, actual) << info{param};
	}
	TEST_P(html_element_allows, by_flags) {
		auto& param = GetParam();
		auto const actual =
		    get_info(param.first)
		        .allows(get_info(param.second).flags, param.second);

		ASSERT_EQ(param.expected, actual) << info{param};
	}

	static constexpr html_element_allows_data names[] = {
	    {"P", "A", true},           {"P", "H1", false},
	    {"UL", "LI", true},         {"H3", "LI", false},
	    {"H3", "DIV", false},       {"IMG", "EM", false},
	    {"NOSCRIPT", "BODY", true}, {"SVG", "P", true},
	    {"SVG", "path", true},      {"a", "em", true},
	    {"a", "input", false},      {"TextArea", "em", false},
	};

	INSTANTIATE_TEST_SUITE_P(names,
	                         html_element_allows,
	                         ::testing::ValuesIn(names));
}  // namespace tangle::browser::testing
