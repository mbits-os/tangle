// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>

#include <tangle/browser/html_split.hpp>
#include <tangle/browser/walk/span.hpp>

namespace tangle::browser::walk {
	struct href_text {
		std::string href{};
		std::string text{};
	};

	struct cursor {
		using view = span<elem>;
		using iterator = view::pointer;

		view elements{};
		std::string_view text{};
		size_t offset{0};

		iterator it = elements.data();
		iterator end = it + elements.size();

		bool eof() const noexcept { return it == end; }
		auto const& operator*() const noexcept { return *it; }
		cursor& operator++() noexcept {
			++it;
			return *this;
		}

		cursor reset() const noexcept { return {elements, text, offset}; };

		std::string innerText() const;
		std::string attr(std::string_view elem,
		                 std::string_view attr_name) const;

		href_text aHrefText() const;

		cursor forCurrentElement();
		cursor innerHTML();
		cursor copyFromHere() const;
	};

	struct root_cursor {
		root_cursor(std::string_view text) : text_{text} {}

		cursor& root() & { return root_; }

	private:
		std::string_view text_;
		std::vector<elem> elements_{html_split(text_)};

		cursor root_{elements_, text_};
	};
}  // namespace tangle::browser::walk