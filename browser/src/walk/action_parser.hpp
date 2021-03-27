// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/base_parser.hpp>

namespace tangle::browser::walk {
	template <typename Final, typename Base = base_parser>
	struct base_action_parser : Base {
		base_action_parser(std::string_view text) : Base{text} {}

		static int is_action_token_char(int c) noexcept {
			switch (c) {
				case '#':
				case ':':
				case '-':
				case '.':
				case '_':
				case '/':
					return 1;
			}
			if (c >= 0x80) return 1;
			return std::isalnum(c);
		}

		void skip_action_token() { this->skip(Final::is_action_token_char); }

		std::string_view action_token() {
			return this->template substr<Final>(
			    [](auto& self) { self.skip_action_token(); });
		}

		bool parse_action() {
			if (!this->get('!')) return false;
			if (this->eof()) return false;

			std::string name{};
			std::vector<std::string> arguments{};

			auto view = action_token();
			if (view.empty()) return false;
			name.assign(view);

			if (this->get('[')) {
				while (!this->eof()) {
					this->skip_ws();
					if (this->peek(']')) break;

					view = action_token();
					if (view.empty()) return false;

					if (this->eof()) return false;
					if (!std::isspace(static_cast<unsigned char>(*this->pos)) &&
					    *this->pos != ']')
						return false;

					std::string arg{};
					arg.assign(view);
					arguments.push_back(std::move(arg));
				}
				if (!this->get(']')) return false;
			}

			auto* self = static_cast<Final*>(this);
			return self->set_action(std::move(name), std::move(arguments));
		}
	};
}  // namespace tangle::browser::walk
