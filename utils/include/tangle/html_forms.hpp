// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace tangle {
	struct html_form_field {
		std::string name{};
		std::vector<std::string> values{};
	};

	struct html_form {
		std::string id{};
		std::string action{};
		std::string method{};
		std::string enctype{};

		std::vector<html_form_field> fields{};

		html_form() = default;
		html_form(std::string&& id, std::string&& action, std::string&& method, std::string&& enctype);

		void add(std::string const& decoded_name, std::string const& decoded_value);
		void set(std::string const& decoded_name, std::string const& decoded_value);
	private:
		std::unordered_map<std::string, size_t> indexes_;
	};

	std::unordered_map<std::string, html_form> html_forms(std::string_view html);
}  // namespace tangle
