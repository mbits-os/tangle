// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <cctype>
#include <string>
#include <tangle/html_split.hpp>

namespace tangle {
	namespace {
		inline char lower(char c) noexcept {
			return static_cast<char>(
			    std::tolower(static_cast<unsigned char>(c)));
		}

		inline bool equal_ignore_case(std::string_view compared,
		                              std::string_view prelowered) noexcept {
			if (compared.length() != prelowered.length()) return false;
			auto it = compared.begin();
			for (auto r_ch : prelowered) {
				if (r_ch != lower(*it++)) return false;
			}
			return true;
		}

		inline bool is_text_elem(std::string_view name) {
			auto const length = name.length();
			if (length > 1) {
				switch (lower(name[0])) {
					case 'o':
						return equal_ignore_case(name.substr(1), "ption"sv);
					case 'r':
						return length == 2 && lower(name[1]) == 'p';
					case 's':
						if (length > 2) {
							switch (lower(name[1])) {
								case 'c':
									return equal_ignore_case(name.substr(2),
									                         "ript"sv);
								case 't':
									return equal_ignore_case(name.substr(2),
									                         "yle"sv);
								default:
									break;
							}
						}
						break;
					case 't':
						if (length > 2) {
							switch (lower(name[1])) {
								case 'e':
									return equal_ignore_case(name.substr(2),
									                         "xtarea"sv);
								case 'i':
									return equal_ignore_case(name.substr(2),
									                         "tle"sv);
								default:
									break;
							}
						}
						break;
					default:
						break;
				}
			}
			return false;
		}

		struct parser {
			std::string_view text;
			size_t index{};
			size_t const end{text.length()};

			bool skip_comment() {
				if (index == end || text[index] != '<') return false;
				if (text.substr(index + 1, 3) != "!--") return false;
				auto pos = text.find("-->", index + 3);
				index = pos == std::string_view::npos ? end : pos + 3;
				return true;
			}

			inline void skip_text(std::string_view elem_name) {
				std::string elem{};
				elem.reserve(elem_name.length() + 1);
				for (auto c : elem_name)
					elem.push_back(lower(c));
				elem.push_back('>');

				auto pos = text.find("</"sv, index);
				while (pos != std::string_view::npos) {
					auto const name = text.substr(pos + 2, elem.length());
					if (equal_ignore_case(name, elem)) {
						index = pos;
						return;
					}
					pos = text.find("</"sv, pos + 2);
				}
				index = end;
			}

			inline void skip_ws() {
				while (index < end &&
				       std::isspace(static_cast<unsigned char>(text[index])))
					++index;
			}

			inline void skip_name() {
				while (index < end &&
				       !std::isspace(static_cast<unsigned char>(text[index])) &&
				       text[index] != '>')
					++index;
			}

			inline void skip_attr() {
				while (index < end &&
				       !std::isspace(static_cast<unsigned char>(text[index])) &&
				       text[index] != '>' && text[index] != '=')
					++index;
			}

			elem get_tag() {
				elem result{};
				result.name.start = index;

				++index;

				auto name_start = index;
				skip_name();
				result.name.value = text.substr(name_start, index - name_start);

				skip_ws();

				while (index < end && text[index] != '>') {
					name_start = index;
					skip_attr();
					auto attr_name =
					    text.substr(name_start, index - name_start);
					if (index < end && text[index] == '=') {
						++index;
						if (index == end) break;

						attr_pos attr{name_start};

						auto const open_quot = text[index];
						if (open_quot == '\'' || open_quot == '"') {
							attr.start = index;
							++index;
							while (index < end && text[index] != open_quot)
								++index;
							if (index < end) ++index;
							attr.stop = index;
							attr.value = text.substr(
							    attr.start + 1, attr.stop - attr.start - 2);
						} else {
							attr.start = index;
							skip_name();
							attr.stop = index;
							attr.value =
							    text.substr(attr.start, attr.stop - attr.start);
						}

						result.attrs[attr_name] = attr;
					} else {
						if (attr_name == "/"sv && text[index] == '>') {
							result.name.autoclose = true;
						} else if (attr_name == "?"sv && text[index] == '>') {
							// pass
						} else {
							result.attrs[attr_name] =
							    attr_pos{name_start, index, index};
						}
					}

					skip_ws();
				}

				result.name.stop = index < end ? index + 1 : end;
				return result;
			}

			std::vector<elem> split() {
				std::vector<elem> result{};
				auto was_text = false;
				while (index < end) {
					if (text[index] != '<') {
						++index;
						continue;
					}

					if (!was_text) {
						if (skip_comment()) continue;
					}
					result.emplace_back(get_tag());
					auto const& back = result.back();
					was_text = is_text_elem(back.name.value);
					if (was_text) skip_text(back.name.value);
				}

				return result;
			}
		};
	}  // namespace

	bool attr_pos::is(std::string_view attr_name,
	                  std::string_view prelowered) noexcept {
		return equal_ignore_case(attr_name, prelowered);
	}

	bool elem::name_info::is(std::string_view prelowered) const noexcept {
		return equal_ignore_case(value, prelowered);
	}

	std::vector<elem> html_split(std::string_view text) {
		return parser{text}.split();
	}
}  // namespace tangle