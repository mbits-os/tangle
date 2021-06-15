// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "json.hh"
#include "str.hh"
#include <cmath>
#include <limits>
#include <memory>
#include <optional>
#include <stack>

namespace json {
	template <typename T, typename Q>
	struct copy_const {
		using type = Q;
	};

	template <typename T, typename Q>
	struct copy_const<T const, Q> {
		using type = Q const;
	};

	template <typename Map>
	typename copy_const<Map, value>::type* from_json_impl(
	    Map& jsn,
	    std::string_view key) {
		using ret_type = typename copy_const<Map, value>::type;
		auto path = split_s('.', key);

		auto it = jsn.find(path.front());
		if (it == jsn.end()) return nullptr;

		ret_type* ctx = &it->second;

		size_t count = path.size();
		for (size_t index = 1; index < count; ++index) {
			auto* dict = cast<object>(*ctx);
			if (!dict) return nullptr;

			auto const& property = path[index];
			it = dict->find(property);
			if (it == dict->end()) return nullptr;

			ctx = &it->second;
		}

		return ctx;
	}

	value* from_json(object& jsn, std::string_view path) {
		return from_json_impl(jsn, path);
	}

	value const* from_json(object const& jsn, std::string_view path) {
		return from_json_impl(jsn, path);
	}

	namespace {
		using uchar = unsigned char;
		using iterator = std::string_view::iterator;

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
					*--midp =
					    static_cast<uchar>(ch | firstByteMark[bytesToWrite]);
			}
			for (int i = 0; i < bytesToWrite; ++i)
				target.push_back(static_cast<char>(*midp++));
		}

		void skip_ws(iterator&, iterator const&);
		value read_string(iterator&, iterator const&);
		value read_number(iterator&, iterator const&);
		value read_keyword(iterator&, iterator const&);

		void encode(uint32_t ch, std::string& target);

		void skip_ws(iterator& it, iterator const& end) {
			while (it != end) {
				switch (*it) {
					case ' ':
					case '\r':
					case '\n':
					case '\t':
						break;
					default:
						return;
				}
				++it;
			}
		}

		struct reader_state;
		enum class reader { push, replace };
		struct reader_result {
			std::variant<std::monostate,
			             std::unique_ptr<reader_state>,
			             json::value>
			    result{};
			reader result_mode{reader::push};

			reader_result() = default;
			reader_result(reader result_mode) : result_mode{result_mode} {}
			reader_result(std::unique_ptr<reader_state> next_reader,
			              reader result_mode = reader::push)
			    : result{std::move(next_reader)}, result_mode{result_mode} {}
			reader_result(json::value&& result,
			              reader result_mode = reader::replace)
			    : result{std::move(result)}, result_mode{result_mode} {}
		};

		struct reader_state {
			virtual ~reader_state() = default;
			virtual reader_result read(iterator& it,
			                           iterator const& end,
			                           value& val) = 0;

		protected:
			template <typename Reader>
			static reader_result push() {
				return {std::make_unique<Reader>()};
			}
			template <typename Reader>
			static reader_result replace() {
				return {std::make_unique<Reader>(), reader::replace};
			}
			static inline reader_result read_value();

			/*
			<before_loop>
			while (<expression>) {
			    <in_loop>
			    // recursion
			    auto val = read_value(...);
			    <has_value>
			}
			<post_loop>
			return value{std::move(result)};
			becomes
			while(true) {
			    switch(state_) {
			        case before_loop:
			            <before_loop>
			            break;
			        case in_loop:
			            <in_loop>
			            // coroutine suspend?
			            stage = has_value;
			            return read_value();
			        case has_value:
			            // coroutine resume?
			            <has_value>
			            break;
			    }
			    if (!<expression>) break;
			    stage = in_loop;
			}
			<past_loop>
			return value{std::move(result)};
			*/
			enum while_stage { before_loop, in_loop, has_value };

			while_stage stage{before_loop};
		};

		class value_reader final : public reader_state {
		public:
			reader_result read(iterator& it,
			                   iterator const& end,
			                   value& val) final;
		};

		inline reader_result reader_state::read_value() {
			return push<value_reader>();
		}

		class array_reader final : public reader_state {
		public:
			reader_result read(iterator& it,
			                   iterator const& end,
			                   value& val) final;

		private:
			array result{};
		};

		class object_reader final : public reader_state {
		public:
			reader_result read(iterator& it,
			                   iterator const& end,
			                   value& val) final;

		private:
			std::optional<std::string> read_object_key(iterator& it,
			                                           iterator const& end);
			object result{};
			std::string current_key_{};
		};

		reader_result value_reader::read(iterator& it,
		                                 iterator const& end,
		                                 value&) {
			skip_ws(it, end);
			if (it == end) return {};

			if (*it == '{') return replace<object_reader>();
			if (*it == '[') return replace<array_reader>();
			if (*it == '"' || *it == '\'') return read_string(it, end);
			if (std::isdigit(static_cast<uchar>(*it)) || *it == '-' ||
			    *it == '+' || *it == '.')
				return read_number(it, end);
			return read_keyword(it, end);
		}

		reader_result array_reader::read(iterator& it,
		                                 iterator const& end,
		                                 value& val) {
			while (true) {
				switch (stage) {
					case before_loop:
						++it;

						skip_ws(it, end);
						break;

					case in_loop:
						skip_ws(it, end);
						stage = has_value;
						return read_value();

					case has_value:
						if (!val.index()) return {};
						result.push_back(std::move(val));
						skip_ws(it, end);
						if (it == end) return {};
						if (*it != ',') {
							if (*it == ']') break;
							return {};
						}
						++it;
						skip_ws(it, end);
						break;
				}
				if (it == end || *it == ']') break;
				stage = in_loop;
			}

			if (it == end || *it != ']') return {};
			++it;
			return value{std::move(result)};
		}

		reader_result object_reader::read(iterator& it,
		                                  iterator const& end,
		                                  value& val) {
			while (true) {
				switch (stage) {
					case before_loop:
						++it;

						skip_ws(it, end);
						break;

					case in_loop:
						// this comment forbids the formatter from placing open
						// bracket at case line
						{
							auto key = read_object_key(it, end);
							if (!key) return {};
							current_key_ = std::move(*key);
						}
						skip_ws(it, end);
						if (it == end || *it != ':') return {};
						++it;
						skip_ws(it, end);
						stage = has_value;
						return read_value();

					case has_value:
						if (!val.index()) return {};
						{
							auto it_inner = result.find(current_key_);
							if (it_inner != result.end() &&
							    it_inner->first == current_key_) {
								it_inner->second = std::move(val);
							} else {
								result.insert(
								    it_inner,
								    {std::move(current_key_), std::move(val)});
							}
						}
						skip_ws(it, end);
						if (it == end) return {};
						if (*it != ',') {
							if (*it == '}') break;
							return {};
						}
						++it;
						skip_ws(it, end);
						break;
				}
				if (it == end || *it == '}') break;
				stage = in_loop;
			}

			if (it == end || *it != '}') return {};
			++it;
			return value{std::move(result)};
		}

		std::optional<std::string> object_reader::read_object_key(
		    iterator& it,
		    iterator const& end) {
			if (*it == '"') {
				auto val = read_string(it, end);
				if (std::holds_alternative<std::string>(val))
					return std::get<std::string>(val);
				return std::nullopt;
			}

			return std::nullopt;
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
			return 16;
		}

		unsigned hex_digit(iterator& it, iterator const& end) {
			if (it == end) return 16;
			auto const val = hex_digit(*it);
			if (val < 16) ++it;
			return val;
		}

		uint32_t utf16_escape(iterator& it, iterator const& end) {
			static constexpr auto max = std::numeric_limits<uint32_t>::max();

			++it;
			if (it == end) return max;
			uint32_t val = 0;
			for (int i = 0; i < 4; ++i) {
				auto const dig = hex_digit(it, end);
				if (dig == 16) return max;
				val *= 16;
				val += dig;
			}
			return val;
		}

		uint32_t utf32_escape(uint32_t high,
		                      iterator& it,
		                      iterator const& end) {
			auto save = it;
			if (it == end || *it != '\\') return high;
			++it;
			if (it == end || *it != 'u') {
				--it;
				return high;
			}

			auto const low = utf16_escape(it, end);
			if (low < UNI_SUR_LOW_START || low > UNI_SUR_LOW_END) {
				it = save;
				return high;
			}

			return ((high - UNI_SUR_HIGH_START) * 0x400) +
			       (low - UNI_SUR_LOW_START) + 0x10000;
		}

		value read_string(iterator& it, iterator const& end) {
			++it;

			std::string result{};
			bool in_string = true;
			bool in_escape = false;
			while (it != end && in_string) {
				if (in_escape) {
					switch (*it) {
						case '\r':
							return {};
						case '\n':
							return {};
						case 'b':
							result.push_back('\b');
							break;
						case 'f':
							result.push_back('\f');
							break;
						case 'n':
							result.push_back('\n');
							break;
						case 'r':
							result.push_back('\r');
							break;
						case 't':
							result.push_back('\t');
							break;
						case 'v':
							return {};
							result.push_back('\v');
							break;
						case 'x':
							return {};
						case 'u': {
							auto val = utf16_escape(it, end);
							if (val == std::numeric_limits<uint32_t>::max())
								return {};
							if (val >= UNI_SUR_HIGH_START &&
							    val <= UNI_SUR_HIGH_END) {
								val = utf32_escape(val, it, end);
								if (val == std::numeric_limits<uint32_t>::max())
									return {};
							}
							encode(val, result);
							--it;
							break;
						}
						case '"':
						case '\\':
						case '/':
							result.push_back(*it);
							break;
						default:
							return {};
					}
					++it;
					in_escape = false;
					continue;
				}

				if (*it == '"') {
					++it;
					in_string = false;
					continue;
				}

				switch (*it) {
					case '\\':
						in_escape = true;
						break;
					default:
						if (static_cast<unsigned char>(*it) < 0x20) return {};
						result.push_back(*it);
				}
				++it;
			}

			if (in_string) return {};
			return value{std::move(result)};
		}

		std::optional<std::pair<unsigned long long, unsigned>> read_digits(
		    iterator& it,
		    iterator const& end) {
			std::pair<unsigned long long, unsigned> result{};
			auto& [value, power] = result;

			bool read = false;
			while (it != end) {
				auto const digit = hex_digit(*it);
				if (digit > 9) {
					if (!read) return std::nullopt;
					break;
				}
				read = true;
				++it;
				++power;
				auto const overflow_guard = value;
				value *= 10;
				value += digit;
				if (overflow_guard > value) return std::nullopt;
			}

			return result;
		}

		value read_number(iterator& it, iterator const& end) {
			int neg = 1;

			if (*it == '+') {
				return {};
			} else if (*it == '-') {
				neg = -1;
				++it;
			}

			if (it == end) return {};
			if (*it == '0') {
				++it;
				if (it == end) return 0ll;
				switch (*it) {
					default:
						return 0ll;
					case '.':
					case 'e':
					case 'E':
						--it;
						break;
				}
			}

			auto const whole = read_digits(it, end);
			if (!whole) return {};

			if (it == end || (*it != '.' && *it != 'e' && *it != 'E'))
				return static_cast<long long>(whole->first) * neg;

			std::pair<unsigned long long, unsigned> fraction{};
			if (*it == '.') {
				++it;
				auto const fraction_ = read_digits(it, end);
				if (!fraction_) return {};

				if (it == end || (*it != 'e' && *it != 'E')) {
					auto const div = std::pow(10.0, fraction_->second);
					return (double(whole->first) +
					        double(fraction_->first) / div) *
					       neg;
				}

				fraction = *fraction_;
			}

			++it;

			int exp_neg = 1;
			if (*it == '+')
				++it;
			else if (*it == '-') {
				exp_neg = -1;
				++it;
			}

			auto const exponent = read_digits(it, end);
			if (!exponent) return {};

			auto const int_exp =
			    static_cast<long long>(exponent->first) * exp_neg;
			auto const full_exp =
			    int_exp + static_cast<long long>(whole->second - 1);

			if (full_exp < std::numeric_limits<double>::min_exponent10 ||
			    full_exp > std::numeric_limits<double>::max_exponent10)
				return {};

			auto const pow10 = std::pow(10.0, int_exp);
			auto const div = std::pow(10.0, fraction.second);
			return (double(whole->first) + double(fraction.first) / div) *
			       pow10 * neg;
		}

		value read_keyword(iterator& it, iterator const& end) {
			auto start = &*it;
			while (it != end && std::isalpha(static_cast<uchar>(*it)))
				++it;
			std::string_view view{start, static_cast<size_t>(&*it - start)};
			if (view == "null") return nullptr;
			if (view == "true") return true;
			if (view == "false") return false;
			return {};
		}
	}  // namespace

	value read_json(std::string_view text) {
		auto it = text.begin();
		auto end = text.end();

		std::stack<std::unique_ptr<reader_state>> stack;
		stack.push(std::make_unique<value_reader>());

		value jsn;
		while (!stack.empty()) {
			auto reader = std::move(stack.top());
			stack.pop();
			auto [result, result_mode] = reader->read(it, end, jsn);
			jsn = {};

			if (std::holds_alternative<std::monostate>(result)) return {};

			if (std::holds_alternative<std::unique_ptr<reader_state>>(result)) {
				auto& next = std::get<std::unique_ptr<reader_state>>(result);
				if (result_mode != json::reader::replace)
					stack.push(std::move(reader));

				stack.push(std::move(next));
				continue;
			}

			jsn = std::move(std::get<value>(result));
		}

		skip_ws(it, text.end());
		if (it != text.end()) return {};
		return jsn;
	}
}  // namespace json