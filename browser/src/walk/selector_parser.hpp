// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/base_parser.hpp>
#include <tangle/browser/walk/selector.hpp>
#include <tangle/str.hpp>

namespace tangle::browser::walk {
	template <typename Final, typename Base = base_parser>
	struct base_selector_parser : Base {
		base_selector_parser(std::string_view text) : Base{text} {}

		static int is_token_char(int c) noexcept {
			return std::isalnum(c) || c == '_' || c == '-' || c == ':' ||
			       (c >= 0x80);
		}

		void skip_ident_token() { this->skip(Final::is_token_char); }

		std::string_view ident_token() {
			return this->template substr<Final>(
			    [](auto& self) { self.skip_ident_token(); });
		}

		bool read_class_selector() {
			if (!this->get('.')) return false;
			if (this->eof()) return false;

			struct selector_enhance {
				char code;
				attr_match function;
			};

			static constexpr selector_enhance enhancements[] = {
			    {'|', attr_match::dash_starts_with},
			    {'^', attr_match::starts_with},
			    {'$', attr_match::ends_with},
			    {'*', attr_match::contains},
			};

			auto function = attr_match::exactly;
			for (auto [code, fn] : enhancements) {
				if (code != *this->pos) continue;
				function = fn;
				++this->pos;
				break;
			}

			auto token = ident_token();
			if (token.empty()) return false;

			auto* self = static_cast<Final*>(this);
			return self->add_class_selector(token, function);
		}

		bool read_id() {
			if (!this->get('#')) return false;

			auto token = ident_token();
			if (token.empty()) return false;

			auto* self = static_cast<Final*>(this);
			return self->set_id(token);
		}

		bool read_attribute_selector() {
			if (!this->get('[')) return false;

			auto attr_name = ident_token();
			if (attr_name.empty() || this->eof()) return false;

			struct selector_info {
				char code;
				attr_match function;
			};

			static constexpr selector_info codes[] = {
			    {'~', attr_match::contains_token},
			    {'|', attr_match::dash_starts_with},
			    {'^', attr_match::starts_with},
			    {'$', attr_match::ends_with},
			    {'*', attr_match::contains},
			};

			auto function = attr_match::unused;
			if (this->peek(']')) {
				function = attr_match::exists;
			} else if (this->peek('=')) {
				this->get('=');
				function = attr_match::exactly;
			} else {
				for (auto [code, fn] : codes) {
					if (code != *this->pos) continue;
					function = fn;
					++this->pos;
					if (!this->get('=')) return false;
					break;
				}
			}

			if (function == attr_match::unused) return false;

			std::string value{};
			if (function != attr_match::exists) {
				auto token = ident_token();
				if (token.empty()) return false;
				value.assign(token);
			}

			if (!this->get(']')) return false;

			std::string name;
			name.assign(attr_name);
			tolower_inplace(name);
			tolower_inplace(value);

			auto* self = static_cast<Final*>(this);
			if (function == attr_match::contains_token && name == "class"sv) {
				return self->add_class_selector(value, function);
			}

			return self->add_attr_selector(std::move(name), std::move(value),
			                               function);
		}

		bool parse_selector() {
			{
				auto type = ident_token();
				auto* self = static_cast<Final*>(this);
				self->set_type_name(type);
			}

			while (!this->eof()) {
				if (this->peek('.')) {
					if (!read_class_selector()) return false;
					continue;
				}

				if (this->peek('#')) {
					if (!read_id()) return false;
					continue;
				}

				if (this->peek('[')) {
					if (!read_attribute_selector()) return false;
					continue;
				}

				break;
			}

			return true;
		}
	};
}  // namespace tangle::browser::walk
