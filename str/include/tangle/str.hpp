// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

namespace tangle {
	std::vector<std::string_view> split_sv(std::string_view sep,
	                                       std::string& data,
	                                       size_t max = std::string::npos);
	std::vector<std::string_view> split_sv(std::string_view sep,
	                                       std::string_view data,
	                                       size_t max = std::string::npos);
	std::vector<std::string> split_s(std::string_view sep,
	                                 std::string_view data,
	                                 size_t max = std::string::npos);
	std::vector<std::string_view> split_sv(char sep,
	                                       std::string& data,
	                                       size_t max = std::string::npos);
	std::vector<std::string_view> split_sv(char sep,
	                                       std::string_view data,
	                                       size_t max = std::string::npos);
	std::vector<std::string> split_s(char sep,
	                                 std::string_view data,
	                                 size_t max = std::string::npos);

	std::string_view strip_sv(std::string_view);
	std::string_view lstrip_sv(std::string_view);
	std::string_view rstrip_sv(std::string_view);

	std::string strip_s(std::string_view);
	std::string lstrip_s(std::string_view);
	std::string rstrip_s(std::string_view);

	std::string& tolower_inplace(std::string&);
	std::string& toupper_inplace(std::string&);

	std::string join(std::string_view sep,
	                 std::vector<std::string_view> const& items);
	std::string join(std::string_view sep,
	                 std::vector<std::string> const& items);
}  // namespace tangle
