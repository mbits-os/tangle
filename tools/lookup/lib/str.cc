// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "str.hh"
#include <cctype>

namespace {
	size_t calc_separators(char sep, std::string_view data, size_t max) {
		auto pos = data.find(sep);
		decltype(pos) prev = 0;

		size_t result = 1;

		while (max && pos != std::string_view::npos) {
			prev = pos + 1;
			pos = data.find(sep, prev);
			if (max != std::string::npos) --max;

			++result;
		}

		return result;
	}

	template <typename String>
	struct helper;

	template <>
	struct helper<std::string_view> {
		static inline std::string_view as_str(std::string_view str) noexcept {
			return str;
		}
	};

	template <>
	struct helper<std::string> {
		static inline std::string as_str(std::string_view str) {
			return {str.data(), str.size()};
		}
	};

	template <typename String>
	std::vector<String> split(char sep, std::string_view data, size_t max) {
		std::vector<String> result{};

		result.reserve(calc_separators(sep, data, max));

		auto pos = data.find(sep);
		decltype(pos) prev = 0;

		while (max && pos != std::string_view::npos) {
			auto const view = data.substr(prev, pos - prev);
			prev = pos + 1;
			pos = data.find(sep, prev);
			if (max != std::string_view::npos) --max;

			result.push_back(helper<String>::as_str(view));
		}

		result.push_back(helper<String>::as_str(data.substr(prev)));

		return result;
	}
}  // namespace

std::string_view lstrip(std::string_view data) {
	auto new_stop = data.size();
	decltype(new_stop) new_start = 0;

	while (new_start < new_stop &&
	       std::isspace(static_cast<unsigned char>(data[new_start])))
		++new_start;

	return data.substr(new_start);
}

std::string_view rstrip(std::string_view data) {
	auto new_stop = data.size();

	while (new_stop > 0 &&
	       std::isspace(static_cast<unsigned char>(data[new_stop - 1])))
		--new_stop;

	return data.substr(0, new_stop);
}

std::string_view strip(std::string_view data) {
	return lstrip(rstrip(data));
}

std::vector<std::string_view> split_sv(char sep,
                                       std::string_view data,
                                       size_t max) {
	return split<std::string_view>(sep, data, max);
}
std::vector<std::string> split_s(char sep, std::string_view data, size_t max) {
	return split<std::string>(sep, data, max);
}
