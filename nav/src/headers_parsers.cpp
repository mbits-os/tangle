// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "headers_parsers.hpp"
#include <algorithm>
#include <cctype>
#include <optional>

using namespace std::literals;

namespace tangle::nav {
	// https://tools.ietf.org/html/rfc7230#section-3.2.6
	int is_http_tchar(int c) noexcept {
		switch (c) {
			case '!':
			case '#':
			case '$':
			case '%':
			case '&':
			case '\'':
			case '*':
			case '+':
			case '-':
			case '.':
			case '^':
			case '_':
			case '`':
			case '|':
			case '~':
				return 1;
			default:
				return std::isalnum(c);
		}
	}

	int is_http_token(std::string_view sv) noexcept {
		for (auto c : sv) {
			if (!is_http_tchar(static_cast<unsigned char>(c))) return false;
		}
		return true;
	}

	void field_iter::skip_token() noexcept { skip(is_http_tchar); }

	std::string_view field_iter::token() noexcept {
		return substr<field_iter>([](auto& self) noexcept {
			self.skip_token();
		});
	}

	std::string_view field_iter::quoted() noexcept {
		if (!get('"')) return {};

		return substr<field_iter>([](field_iter & self) noexcept {
			bool escaped = false;
			while (self.pos < self.end) {
				if (escaped) {
					escaped = false;
					++self.pos;
					continue;
				}

				switch (*self.pos) {
					case '"':
						return;
					case '\\':
						escaped = true;
						break;
				}

				++self.pos;
			}
		});
	}

	std::string field_iter::extract_quoted(std::string_view escaped) {
		size_t length = 0;
		bool slashed = false;
		for (auto c : escaped) {
			if (!slashed && c == '\\') {
				slashed = true;
				continue;
			}
			slashed = false;
			++length;
		}

		std::string result{};
		result.reserve(length);

		slashed = false;
		for (auto c : escaped) {
			if (!slashed && c == '\\') {
				slashed = true;
				continue;
			}
			slashed = false;
			result.push_back(c);
		}

		return result;
	}

	namespace impl {
		namespace {
			inline size_t usize(ptrdiff_t diff) noexcept {
				if (diff < 0) return 0;
				return static_cast<size_t>(diff);
			}

			bool parse_content_type(std::string_view& header,
			                        content_type_value& result,
			                        std::optional<std::string>& charset) {
				field_iter iterator{header};
				iterator.skip_ws();

				auto const mime_start = iterator.pos;

				// type
				if (iterator.token().empty()) return false;
				result.slash_pos = usize(iterator.pos - mime_start);
				if (!iterator.get('/')) return false;
				// subtype
				if (iterator.token().empty()) return false;

				result.mime.assign(mime_start,
				                   usize(iterator.pos - mime_start));

				tolower_inplace(result.mime);

				iterator.skip_ws();
				result.parameteres.clear();

				if (iterator.peek(';')) {
					auto callback = [&](std::string&& name,
					                    std::string&& value) {
						if (name == "charset") {
							if (!charset)
								charset = std::move(tolower_inplace(value));
						} else {
							auto it = result.parameteres.find(name);
							if (it == result.parameteres.end()) {
								result.parameteres.insert(
								    {std::move(name), std::move(value)});
							}
						}
					};
					iterator.parse_params(callback);
				}

				iterator.get(',');
				header = header.substr(usize(iterator.pos - header.data()));
				return true;
			}

			content_type_value application_octet_stream() {
				static constexpr auto octet_stream =
				    "application/octet-stream"sv;
				static constexpr auto octet_stream_slash =
				    octet_stream.find('/');

				content_type_value result{};
				result.mime.assign(octet_stream);
				result.slash_pos = octet_stream_slash;
				return result;
			}
		}  // namespace

		content_type_value get_content_type(headers const& hdrs) {
			// source: https://fetch.spec.whatwg.org/#content-type-header

			content_type_value result{};
			std::optional<std::string> charset{};

			// 4. Let values be the result of getting, decoding, and splitting
			// `Content-Type` from headers.
			auto it = hdrs.find(header::Content_Type);
			if (it == hdrs.end()) {
				// 5. If values is null, then return failure.
				//
				// replaced by:
				// https://tools.ietf.org/html/rfc7231#section-3.1.1.5 If a
				// Content-Type header field is not present, the recipient MAY
				// either assume a media type of "application/octet-stream"
				// ([RFC2046], Section 4.5.1) or examine the data to determine
				// its type.
				return application_octet_stream();
			}

			// 6. For each value of values:
			bool updated = false;
			for (auto const& header_str : it->second) {
				auto header = std::string_view{header_str};
				while (!header.empty()) {
					// 6.1 Let temporaryMimeType be the result of parsing value.
					content_type_value current{};
					// GCOV_EXCL_START -- constructor optimized out?
					std::optional<std::string> current_charset{};
					// GCOV_EXCL_STOP

					// 6.2 If temporaryMimeType is failure or its essence is
					// "*/*", then continue.
					if (!parse_content_type(header, current, current_charset)) {
						field_iter field_it{header};
						field_it.look_for<','>();
						field_it.get(',');

						header = header.substr(usize(field_it.pos - header.data()));
						continue;
					}

					if (current.mime == "*/*"sv) continue;

					updated = true;
					// 6.3 Set mimeType to temporaryMimeType.
					auto prev_mime = std::move(result.mime);
					std::swap(result, current);

					// 6.4 If mimeType's essence is not essence, then:
					//     1. Set charset to null.
					//     2. If mimeType's parameters["charset"] exists, then
					//     set
					//        charset to mimeType's parameters["charset"].
					//     3. Set essence to mimeType's essence.
					// 6.5 Otherwise, if mimeType's parameters["charset"] does
					// not
					//     exist, and charset is non-null, set mimeType's
					//     parameters["charset"] to charset.
					if (result.mime != prev_mime || current_charset) {
						// 6.5 here is:
						//
						// if (!current_charset)
						// --> parameters["charset"] does not exist
						// --> set parameters["charset"] to charset
						// --> leave optional<> charset as is
						//
						// if (current_charset)
						// --> parameters["charset"] exists
						// --> do nothing
						// --> transfer current_charset to charset to have
						//    'parameters["charset"]' in the end
						charset = std::move(current_charset);
					}
				}
			}

			if (charset) result.parameteres["charset"] = std::move(*charset);

			// 8. Return mimeType.
			if (updated) return result;

			// 7. If mimeType is null, then return failure.
			return application_octet_stream();
		}
	}  // namespace impl
}  // namespace tangle::nav
