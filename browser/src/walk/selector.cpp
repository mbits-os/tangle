// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/base_parser.hpp>
#include <tangle/browser/walk/selector.hpp>
#include <tangle/str.hpp>
#include <algorithm>

#include "selector_parser.hpp"

namespace tangle::browser::walk {
	namespace {
		struct selector_parser : base_selector_parser<selector_parser> {
			selector& dst;

			selector_parser(selector& dst, std::string_view text)
			    : base_selector_parser<selector_parser>{text}, dst{dst} {}

			bool set_id(std::string_view name) {
				dst.id.assign(name);
				return true;
			}

			bool set_type_name(std::string_view name) {
				dst.name.assign(name);
				tolower_inplace(dst.name);
				return true;
			}

			bool add_class_selector(std::string_view name,
			                        attr_match function) {
				class_token_selector sel{{}, function};
				sel.value.assign(name);
				tolower_inplace(sel.value);
				dst.class_list.push_back(std::move(sel));
				return true;
			}

			bool add_attr_selector(std::string&& name,
			                       std::string&& value,
			                       attr_match function) {
				tolower_inplace(name);
				tolower_inplace(value);
				dst.attr_list.push_back(
				    {std::move(name), std::move(value), function});
				return true;
			}

			bool parse() {
				if (!base_selector_parser<selector_parser>::parse_selector())
					return false;
				if (eof() || is_space(static_cast<unsigned char>(*pos)))
					return true;
				return false;
			}
		};

		bool equals(std::string_view value,
		            std::string_view prelowered) noexcept {
			return tangle::browser::equal_ignore_case(value, prelowered);
		}

		bool dashStartsWith(std::string_view value,
		                    std::string_view prelowered) noexcept {
			if (equals(value, prelowered)) return true;
			if (value.size() < (prelowered.size() + 1)) return false;

			auto sub = value.substr(0, prelowered.size());
			if (!equals(sub, prelowered)) return false;

			return value[prelowered.size()] == '-';
		}

		bool startsWith(std::string_view value,
		                std::string_view prelowered) noexcept {
			auto sub = value.substr(0, prelowered.size());
			return equals(sub, prelowered);
		}

		bool endsWith(std::string_view value,
		              std::string_view prelowered) noexcept {
			if (value.size() < prelowered.size()) return false;
			auto sub = value.substr(value.size() - prelowered.size());
			return equals(sub, prelowered);
		}

		bool contains(std::string_view value,
		              std::string_view prelowered) noexcept {
			if (value.size() < prelowered.size()) return false;
			auto const start_positions = value.size() - prelowered.size() + 1;
			for (size_t start = 0; start < start_positions; ++start) {
				auto sub = value.substr(start, prelowered.size());
				if (equals(sub, prelowered)) return true;
			}
			return false;
		}

		template <typename Pred>
		bool find_token(std::vector<std::string> const& tokens,
		                std::string_view prelowered,
		                Pred pred) {
			for (auto const& token : tokens) {
				if (pred(token, prelowered)) return true;
			}
			return false;
		}
	}  // namespace

	std::ostream& operator<<(std::ostream& out, selector const& descr) {
		out << descr.name;

		if (!descr.id.empty()) out << '#' << descr.id;

		for (auto const& [class_name, function] : descr.class_list) {
			if (class_name.empty() || function == attr_match::unused) continue;
			out << '.';
			switch (function) {
				case attr_match::unused:
				case attr_match::exists:
				case attr_match::exactly:
				case attr_match::contains_token:
					break;
				case attr_match::dash_starts_with:
					out << '|';
					break;
				case attr_match::starts_with:
					out << '^';
					break;
				case attr_match::ends_with:
					out << '$';
					break;
				case attr_match::contains:
					out << '*';
					break;
			}
			out << class_name;
		}

		for (auto const& [name, value, function] : descr.attr_list) {
			if (name.empty() ||
			    (value.empty() && function != attr_match::exists) ||
			    function == attr_match::unused)
				continue;

			out << '[' << name;
			switch (function) {
				case attr_match::unused:
				case attr_match::exists:
					break;
				case attr_match::exactly:
					out << '=';
					break;
				case attr_match::contains_token:
					out << "~=";
					break;
				case attr_match::dash_starts_with:
					out << "|=";
					break;
				case attr_match::starts_with:
					out << "^=";
					break;
				case attr_match::ends_with:
					out << "$=";
					break;
				case attr_match::contains:
					out << "*=";
					break;
			}
			out << value << ']';
		}

		return out;
	}

	std::vector<std::string> selector::make_token_list(
	    std::string_view classes) {
		auto list = tangle::split_s(' ', classes);
		auto it = std::remove_if(list.begin(), list.end(),
		                         [](auto const& s) { return s.empty(); });
		list.erase(it, list.end());
		for (auto& val : list)
			tolower_inplace(val);
		return list;
	}

	selector selector::parse(std::string_view text) {
		selector result{};
		if (!selector_parser{result, text}.parse()) {
			result = {};
		}
		return result;
	}

	selector selector::matching(elem const& element) {
		selector result{};
		result.name.assign(element.name.value);
		tolower_inplace(result.name);
		for (auto& [name, value] : element.attrs) {
			if (equal_ignore_case(name, "id"sv)) {
				result.id.assign(value.value);
				tolower_inplace(result.id);
				continue;
			}
			if (equal_ignore_case(name, "class"sv)) {
				auto names = make_token_list(value.value);
				result.class_list.clear();
				result.class_list.reserve(names.size());
				std::transform(
				    names.begin(), names.end(),
				    std::back_inserter(result.class_list),
				    [](auto& name) -> class_token_selector {
					    return {std::move(name), attr_match::exactly};
				    });
				continue;
			}
		}
		return result;
	}

	bool selector::matches(elem const& element) const {
		using namespace tangle::browser;

		if (!name.empty() && !element.name.is(name)) {
			return false;
		}

		size_t expected = attr_list.size();
		if (!id.empty()) ++expected;
		if (!class_list.empty()) ++expected;

		size_t matched = 0;

		for (auto& [attr_name, attr] : element.attrs) {
			if (equals(attr_name, "id"sv)) {
				if (!id.empty()) {
					if (!equals(attr_decode(attr.value), id)) return false;
					++matched;
				}
			}

			if (equals(attr_name, "class"sv) && !class_list.empty()) {
				if (!match_class_list(attr.value)) return false;
				++matched;
			}

			if (!attr_list.empty()) {
				auto partial = match_attr(attr_name, attr.value);
				matched += partial;
			}
		}

		return matched == expected;
	}

	bool selector::match_class_list(std::string_view attr_value) const
	    noexcept {
		auto attr_decoded = tangle::browser::attr_decode(attr_value);
		auto attr_classes = make_token_list(attr_decoded);

		for (auto prelowered : class_list) {
			switch (prelowered.function) {
				case attr_match::unused:
					return false;
				case attr_match::exists:
				case attr_match::exactly:
				case attr_match::contains_token:
					if (!find_token(attr_classes, prelowered.value, equals))
						return false;
					break;
				case attr_match::dash_starts_with:
					if (!find_token(attr_classes, prelowered.value,
					                dashStartsWith))
						return false;
					break;
				case attr_match::starts_with:
					if (!find_token(attr_classes, prelowered.value, startsWith))
						return false;
					break;
				case attr_match::ends_with:
					if (!find_token(attr_classes, prelowered.value, endsWith))
						return false;
					break;
				case attr_match::contains:
					if (!find_token(attr_classes, prelowered.value, contains))
						return false;
					break;
			}
		}

		return true;
	}

	size_t selector::match_attr(std::string_view attr_name,
	                            std::string_view attr_value) const noexcept {
		size_t matched = 0;
		for (auto const& [next_name, value, function] : attr_list) {
			if (!equals(attr_name, next_name)) continue;
			++matched;

			switch (function) {
				case attr_match::unused:
					return 0;
				case attr_match::exists:
					break;
				case attr_match::exactly:
					if (!equals(tangle::browser::attr_decode(attr_value),
					            value))
						return 0;
					break;
				case attr_match::contains_token: {
					auto attr_classes = make_token_list(
					    tangle::browser::attr_decode(attr_value));
					if (!find_token(attr_classes, value, equals)) return 0;
					break;
				}
				case attr_match::dash_starts_with:
					if (!dashStartsWith(
					        tangle::browser::attr_decode(attr_value), value))
						return 0;
					break;
				case attr_match::starts_with:
					if (!startsWith(tangle::browser::attr_decode(attr_value),
					                value))
						return 0;
					break;
				case attr_match::ends_with:
					if (!endsWith(tangle::browser::attr_decode(attr_value),
					              value))
						return 0;
					break;
				case attr_match::contains:
					if (!contains(tangle::browser::attr_decode(attr_value),
					              value))
						return 0;
					break;
			}
		}

		return matched;
	}
}  // namespace tangle::browser::walk
