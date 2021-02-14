// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/nav/request.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace tangle::browser {
	struct form_field {
		std::string name{};
		std::vector<std::string> values{};
	};

	struct form {
		std::string id{};
		std::string action{};
		std::string method{};
		std::string enctype{};

		std::vector<form_field> fields{};

		form() = default;
		form(std::string&& id,
		     std::string&& action,
		     std::string&& method,
		     std::string&& enctype);

		void add(std::string const& decoded_name,
		         std::string const& decoded_value);
		void set(std::string const& decoded_name,
		         std::string const& decoded_value);
		nav::request make_request(uri const& page_url) const;

	private:
		std::unordered_map<std::string, size_t> indexes_;
	};

	std::unordered_map<std::string, form> forms(std::string_view html);
}  // namespace tangle::browser
