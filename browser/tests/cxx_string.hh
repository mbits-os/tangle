// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <iomanip>
#include <iostream>
#include <string_view>

namespace tangle::browser::testing {
	struct cxx_string {
		std::string_view val;
		std::string_view::size_type len{std::string_view::npos};

		friend std::ostream& operator<<(std::ostream& out,
		                                cxx_string const& data) {
			if (data.val.empty()) return out << "{}";
			out << '"';
			std::string_view::size_type count{};
			for (auto c : data.val) {
				if (count == data.len) return out << "...";
				++count;
				auto const uc = static_cast<unsigned char>(c);
				switch (c) {
					case '\\':
						out << "\\\\";
						break;
					case '\"':
						out << "\\\"";
						break;
					case '\a':
						out << "\\a";
						break;
					case '\b':
						out << "\\b";
						break;
					case '\f':
						out << "\\f";
						break;
					case '\n':
						out << "\\n";
						break;
					case '\r':
						out << "\\r";
						break;
					case '\t':
						out << "\\t";
						break;
					case '\v':
						out << "\\v";
						break;
					default:
						if (std::isprint(uc)) {
							out << c;
						} else {
							out << "\\x" << std::hex << std::setw(2)
							    << std::setfill('0')
							    << static_cast<unsigned>(uc) << std::dec;
						}
						break;
				}
			}
			return out << "\"sv";
		}
	};
}  // namespace tangle::browser::testing
