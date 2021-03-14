// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/entities.hpp>
#include <tangle/browser/html_split.hpp>
#include <cctype>
#include <string>

namespace tangle::browser {
	namespace {
		inline char lower(char c) noexcept {
			return static_cast<char>(
			    std::tolower(static_cast<unsigned char>(c)));
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

	inline bool equal_ignore_case(std::string_view compared,
	                              std::string_view prelowered) noexcept {
		if (compared.length() != prelowered.length()) return false;
		auto it = compared.begin();
		for (auto r_ch : prelowered) {
			if (r_ch != lower(*it++)) return false;
		}
		return true;
	}

	bool elem::name_info::is(std::string_view prelowered) const noexcept {
		return equal_ignore_case(value, prelowered);
	}

	std::vector<elem> html_split(std::string_view text) {
		return parser{text}.split();
	}

	unsigned hex_digit(char c) {
		switch (c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return static_cast<unsigned>(c - '0');
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return static_cast<unsigned>(c - 'A' + 10);
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				return static_cast<unsigned>(c - 'a' + 10);
		}
		return 0;
	}

	using uchar = unsigned char;
	static constexpr const uchar firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0,
	                                                 0xF0, 0xF8, 0xFC};

	enum : uint32_t {
		UNI_SUR_HIGH_START = 0xD800,
		UNI_SUR_HIGH_END = 0xDBFF,
		UNI_SUR_LOW_START = 0xDC00,
		UNI_SUR_LOW_END = 0xDFFF,
		UNI_REPLACEMENT_CHAR = 0x0000FFFD,
		UNI_MAX_BMP = 0x0000FFFF,
		UNI_MAX_UTF16 = 0x0010FFFF,
		UNI_MAX_LEGAL_UTF32 = 0x0010FFFF
	};

	static constexpr const uint32_t byteMask = 0xBF;
	static constexpr const uint32_t byteMark = 0x80;

	void encode(uint32_t ch, std::string& target) {
		unsigned short bytesToWrite = 0;

		/* Figure out how many bytes the result will require */
		if (ch < 0x80u)
			bytesToWrite = 1;
		else if (ch < 0x800u)
			bytesToWrite = 2;
		else if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		} else if (ch < 0x10000u)
			bytesToWrite = 3;
		else if (ch <= UNI_MAX_LEGAL_UTF32)
			bytesToWrite = 4;
		else {
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}

		uchar mid[4];
		uchar* midp = mid + sizeof(mid);
		switch (bytesToWrite) { /* note: everything falls through. */
			case 4:
				*--midp = static_cast<uchar>((ch | byteMark) & byteMask);
				ch >>= 6;
				[[fallthrough]];
			case 3:
				*--midp = static_cast<uchar>((ch | byteMark) & byteMask);
				ch >>= 6;
				[[fallthrough]];
			case 2:
				*--midp = static_cast<uchar>((ch | byteMark) & byteMask);
				ch >>= 6;
				[[fallthrough]];
			case 1:
				*--midp = static_cast<uchar>(ch | firstByteMark[bytesToWrite]);
		}
		for (int i = 0; i < bytesToWrite; ++i)
			target.push_back(static_cast<char>(*midp++));
	}

	std::string attr_encode(std::string_view attr) {
		size_t len = attr.size();
		for (auto c : attr) {
			switch (c) {
				case '<':
				case '>':
					len += 3;
					break;
				case '&':
					len += 4;
					break;
				case '"':
					len += 6;
					break;
			}
		}
		std::string result{};
		result.reserve(len);
		for (auto c : attr) {
			switch (c) {
				case '<':
					result.append("&lt;"sv);
					break;
				case '>':
					result.append("&gt;"sv);
					break;
				case '&':
					result.append("&amp;"sv);
					break;
				case '"':
					result.append("&quot;"sv);
					break;
				default:
					result.push_back(c);
			}
		}
		return result;
	}

	std::string attr_decode(std::string_view attr) {
		std::string result{};
		result.reserve(attr.size());

		auto it = attr.begin();
		auto end = attr.end();

		while (it != end) {
			auto start = it;
			while (it != end && *it != '&')
				++it;

			result.append(start, it);

			if (it == end) break;

			++it;
			if (it == end) break;

			if (*it == '#') {
				++it;
				if (it == end) break;

				uint32_t character{};

				if (*it == 'x' || *it == 'X') {
					++it;
					while (it != end &&
					       std::isxdigit(static_cast<unsigned char>(*it))) {
						character *= 16;
						character += hex_digit(*it);
						++it;
					}
				} else {
					while (it != end &&
					       std::isdigit(static_cast<unsigned char>(*it))) {
						character *= 10;
						character += static_cast<unsigned char>(*it - '0');
						++it;
					}
				}
				if (it != end && *it == ';') ++it;

				encode(character, result);
				continue;
			}

			start = it;
			while (it != end && std::isalnum(static_cast<unsigned char>(*it)))
				++it;
			auto const has_semicolon = it != end && *it == ';';

			auto name =
			    std::string_view{&*start, static_cast<size_t>(&*it - &*start)};

			auto entity = browser::entity(name);
			if (entity) {
				result.append(entity);
				if (has_semicolon) ++it;
			} else {
				result.push_back('&');
				result.append(name);
			}
		}
		return result;
	}
}  // namespace tangle::browser