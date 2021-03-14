// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/base_parser.hpp>
#include <tangle/nav/headers.hpp>
#include <tangle/str.hpp>

namespace tangle::nav {
	int is_http_tchar(int c) noexcept;
	int is_http_token(std::string_view sv) noexcept;

	struct field_iter : base_parser {
		using base_parser::base_parser;

		void skip_token() noexcept;
		std::string_view token() noexcept;
		std::string_view quoted() noexcept;
		static std::string extract_quoted(std::string_view escaped);

		template <typename Pred>
		void parse_params(Pred on_header) {
			while (!eof()) {
				if (peek(',')) {
					break;
				}
				get(';');
				skip_ws();

				auto const name_view = text_until<'=', ';', ','>();
				std::string value{};

				if (!get('=')) {
					// either ';' or eof
					continue;
				}

				if (peek('"')) {
					auto escaped = quoted();
					if (!get('"')) continue;
					look_for<';', ','>();
					value = extract_quoted(escaped);
				} else {
					auto const value_view = rstrip_sv(text_until<';', ','>());
					if (!is_http_token(value_view)) continue;
					value.assign(value_view);
				}

				if (name_view.empty() || !is_http_token(name_view)) continue;

				std::string name{name_view.data(), name_view.length()};
				tolower_inplace(name);

				on_header(std::move(name), std::move(value));
			}
		}
	};

	namespace impl {
		content_type_value get_content_type(headers const&);
	}
}  // namespace tangle::nav
