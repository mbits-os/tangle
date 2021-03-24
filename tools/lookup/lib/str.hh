// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

std::string_view lstrip(std::string_view data);
std::string_view rstrip(std::string_view data);
std::string_view strip(std::string_view data);

std::vector<std::string_view> split_sv(char sep,
                                       std::string_view data,
                                       size_t max = std::string_view::npos);
std::vector<std::string> split_s(char sep,
                                 std::string_view data,
                                 size_t max = std::string_view::npos);
