// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/base_parser.hpp>
#include <tangle/browser/entities.hpp>
#include <tangle/browser/html_split.hpp>
#include <tangle/str.hpp>
#include <cctype>
#include <string>

namespace tangle::browser {
	namespace {
		inline int is_name_start(int c) noexcept {
			return std::isalpha(c) || c == ':' || c == '_' || c >= 0x80;
		}

		inline int is_name_char(int c) noexcept {
			return std::isalnum(c) || c == ':' || c == '_' || c == '.' ||
			       c == '-' || c >= 0x80;
		}

		inline size_t usize(ptrdiff_t diff) noexcept {
			if (diff < 0) return 0;
			return static_cast<size_t>(diff);
		}

		inline char lower(char c) noexcept {
			return static_cast<char>(
			    std::tolower(static_cast<unsigned char>(c)));
		}

		inline bool is_text_elem(std::string_view name) {
			// option, rp, script, style, textarea, title
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

		inline bool is_void_element(std::string_view name) {
			// area, base, br, col, command, embed, hr, img, input, keygen,
			// link, meta, param, source, track, wbr
			auto const length = name.length();
			if (length > 1) {
				switch (lower(name[0])) {
					case 'a':
						return equal_ignore_case(name.substr(1), "rea"sv);
					case 'b':
						if (length == 2 && lower(name[1]) == 'r') return true;
						return equal_ignore_case(name.substr(1), "ase"sv);
					case 'c':
						if (length > 2 && lower(name[1]) == 'o') {
							if (length == 3 && lower(name[2]) == 'l')
								return true;
							return equal_ignore_case(name.substr(2), "mmand"sv);
						}
						break;
					case 'e':
						return equal_ignore_case(name.substr(1), "mbed"sv);
					case 'h':
						return length == 2 && lower(name[1]) == 'r';
					case 'i':
						if (length > 2) {
							switch (lower(name[1])) {
								case 'm':
									return length == 3 && lower(name[2]) == 'g';
								case 'n':
									return equal_ignore_case(name.substr(2),
									                         "put"sv);
								default:
									break;
							}
						}
						break;
					case 'k':
						return equal_ignore_case(name.substr(1), "eygen"sv);
					case 'l':
						return equal_ignore_case(name.substr(1), "ink"sv);
					case 'm':
						return equal_ignore_case(name.substr(1), "eta"sv);
					case 'p':
						return equal_ignore_case(name.substr(1), "aram"sv);
					case 's':
						return equal_ignore_case(name.substr(1), "ource"sv);
					case 't':
						return equal_ignore_case(name.substr(1), "rack"sv);
					case 'w':
						return equal_ignore_case(name.substr(1), "br"sv);
					default:
						break;
				}
			}
			return false;
		}

		struct parser : base_parser {
			std::vector<elem>& output;
			parser(std::string_view text, std::vector<elem>& output)
			    : base_parser{text}, output{output} {}

			std::string_view get_name(char skip_first = 0) noexcept {
				return substr<parser>([skip_first](parser & self) noexcept {
					if (skip_first && !self.get(skip_first)) return;

					if (!self.is(is_name_start)) {
						if (skip_first) --self.pos;
						return;
					}
					++self.pos;
					self.skip(is_name_char);
				});
			}

			bool read_comment() {
				auto start = index() - 1;
				if (text.substr(start + 1, 3) != "!--"sv) return false;

				auto stop = text.find("-->"sv, start + 4);
				if (stop == std::string_view::npos)
					stop = text.size();
				else
					stop += 3;
				pos = text.data() + stop;

				output.emplace_back(elem{{"!--", start, stop, true}});
				return true;
			}

			bool ends_with(char potential_end) const noexcept {
				auto ptr = pos;
				++ptr;
				return peek('>') ||
				       (peek(potential_end) && ptr < end && *ptr == '>');
			}

			bool read_attributes(
			    std::unordered_map<std::string_view, attr_pos>& attrs,
			    char potential_end) {
				while (!eof()) {
					skip_ws();
					if (peek(potential_end)) {
						++pos;
						if (eof() || peek('>')) {
							--pos;
							break;
						}
						--pos;
					}
					if (peek('>')) break;

					auto const name_start = index();
					auto attr_name = get_name();
					if (attr_name.empty()) return false;

					auto save = pos;
					skip_ws();
					if (!peek('=')) {
						pos = save;
					}

					if (!get('=')) {
						// value-less attribute
						auto const here = index();
						attrs[attr_name] = attr_pos{name_start, here, here};
						continue;
					}

					skip_ws();

					if (peek('\'') || peek('"')) {
						auto const start = index();
						auto const open_quot = *pos++;
						skip_until([open_quot](int c) noexcept {
							return c == open_quot;
						});
						if (!get(open_quot)) return false;
						auto const stop = index();

						attrs[attr_name] = attr_pos{
						    name_start,
						    start,
						    stop,
						    text.substr(start + 1, stop - start - 2),
						};

						continue;
					}

					auto const start = index();
					while (pos < end &&
					       !std::isspace(static_cast<unsigned char>(*pos)) &&
					       *pos != '>')
						++pos;
					auto const stop = index();

					attrs[attr_name] = attr_pos{
					    name_start,
					    start,
					    stop,
					    text.substr(start, stop - start),
					};
				}
				return true;
			}

			bool read_pi() {
				elem result{};
				result.name.start = index() - 1;

				result.name.value = get_name('?');
				if (result.name.value.empty()) return false;
				if (!read_attributes(result.attrs, '?')) return false;
				if (!get('?')) return false;
				if (!get('>')) return false;

				result.name.stop = index();
				output.emplace_back(std::move(result));
				return true;
			}

			bool read_open_tag() {
				elem result{};
				result.name.start = index() - 1;

				result.name.value = get_name();
				if (result.name.value.empty()) return false;
				if (!read_attributes(result.attrs, '/')) return false;
				if (peek('/')) {
					result.name.autoclose = true;
					++pos;
				}
				if (!get('>')) return false;

				result.name.stop = index();
				if (!result.name.autoclose &&
				    is_void_element(result.name.value))
					result.name.autoclose = true;
				output.emplace_back(std::move(result));
				return true;
			}

			bool read_close_tag() {
				auto start = index() - 1;
				auto name = get_name('/');
				skip_ws();
				if (!get('>')) return false;
				auto stop = index();
				output.emplace_back(elem{{name, start, stop}});
				return true;
			}

			bool read_doctype() {
				auto start = index() - 1;
				auto name = get_name('!');
				look_for<'>'>();
				if (!get('>')) return false;
				auto stop = index();
				output.emplace_back(elem{{name, start, stop}});
				return true;
			}

			bool read_tag() {
				if (!get('<')) return false;
				if (peek('?')) return read_pi();
				if (peek('/')) return read_close_tag();
				if (peek('!')) {
					auto ptr = pos + 1;
					if (ptr < end && *ptr == '-') return read_comment();
					return read_doctype();
				}
				return read_open_tag();
			}

			inline void skip_text(std::string_view elem_name) {
				std::string elem{};
				elem.assign(elem_name);
				tolower_inplace(elem);

				auto start = text.find("</"sv, index());
				while (start != std::string_view::npos) {
					auto const name = text.substr(start + 2, elem.size());
					if (equal_ignore_case(name, elem)) {
						pos = text.data() + start;
						return;
					}
					start = text.find("</"sv, start + 2);
				}
				pos = end;
			}

			bool split() {
				while (!eof()) {
					if (*pos != '<') {
						++pos;
						continue;
					}

					if (!read_tag()) return false;

					auto const& back = output.back();
					if (is_text_elem(back.name.value))
						skip_text(back.name.value);
				}

				return true;
			}

			static std::vector<elem> split(std::string_view text) {
				std::vector<elem> output{};
				if (!parser{text, output}.split()) {
					output.clear();
				}

				return output;
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
		return parser::split(text);
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
		return 0;  // GCOV_EXCL_LINE -- hex_digit is behind std::isxdigit
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