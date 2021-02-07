// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <assert.h>
#include <iostream>
#include <tangle/html_forms.hpp>
#include <tangle/html_split.hpp>

namespace tangle {
	struct form_attrs_view {
		std::string_view id{};
		std::string_view name{};
		std::string_view action{};
		std::string_view method{};
		std::string_view enctype{};
	};

	struct input_attrs_view {
		std::string_view name{};
		std::string_view type{};
		std::string_view value{};
		bool is_checked = false;
	};

	html_form::html_form(std::string&& id,
	                     std::string&& action,
	                     std::string&& method,
	                     std::string&& enctype)
	    : id{std::move(id)}
	    , action{std::move(action)}
	    , method{std::move(method)}
	    , enctype{std::move(enctype)} {}

	void html_form::add(std::string const& decoded_name,
	                    std::string const& decoded_value) {
		auto it = indexes_.find(decoded_name);
		if (it != indexes_.end()) {
			auto const index = it->second;
			assert(index < fields.size());
			fields[index].values.push_back(decoded_value);
			return;
		}

		indexes_[decoded_name] = fields.size();
		fields.push_back({decoded_name, {decoded_value}});
	}

	void html_form::set(std::string const& decoded_name,
	                    std::string const& decoded_value) {
		auto it = indexes_.find(decoded_name);
		if (it != indexes_.end()) {
			auto const index = it->second;
			assert(index < fields.size());
			fields[index].values = {decoded_value};
			return;
		}

		indexes_[decoded_name] = fields.size();
		fields.push_back({decoded_name, {decoded_value}});
	}

	nav::request html_form::make_request(uri const& page_url) const {
		uri::params params{};
		for (auto const& field : fields) {
			for (auto const& value : field.values) {
				params.add(field.name, value);
			}
		}

		auto action_url = page_url;
		if (!action.empty()) action_url = action;

		auto const nav_method =
		    method == "post" ? nav::method::post : nav::method::get;

		if (nav_method == nav::method::get) action_url.query(params.string());

		nav::request result{nav_method, action_url};
		result.referrer(page_url).max_redir(0);
		if (nav_method == nav::method::post)
			result.form_fields(params.string(uri::form_urlencoded));
		return result;
	}

	form_attrs_view form_attrs(
	    std::unordered_map<std::string_view, attr_pos> const& attrs) {
		form_attrs_view result{};
		for (auto const& [name, attr] : attrs) {
			if (attr_pos::is(name, "id"sv))
				result.id = attr.value;
			else if (attr_pos::is(name, "name"sv))
				result.name = attr.value;
			else if (attr_pos::is(name, "action"sv))
				result.action = attr.value;
			else if (attr_pos::is(name, "method"sv))
				result.method = attr.value;
			else if (attr_pos::is(name, "enctype"sv))
				result.enctype = attr.value;
		}
		return result;
	}

	input_attrs_view input_attrs(
	    std::unordered_map<std::string_view, attr_pos> const& attrs) {
		input_attrs_view result{};
		for (auto const& [name, attr] : attrs) {
			if (attr_pos::is(name, "name"sv))
				result.name = attr.value;
			else if (attr_pos::is(name, "type"sv))
				result.type = attr.value;
			else if (attr_pos::is(name, "value"sv))
				result.value = attr.value;
			else if (attr_pos::is(name, "checked"sv))
				result.is_checked = true;
		}
		return result;
	}

	std::unordered_map<std::string, html_form> html_forms(
	    std::string_view html) {
		std::unordered_map<std::string, html_form> result{};
		auto elems = html_split(html);

		bool in_form = false;
		std::string form_id;

		size_t prev_end = std::string::npos;
		size_t prev_textarea = std::string::npos - 1;
		std::string_view prev_textarea_name{};

		html_form* current{nullptr};

		struct lazy_assign {
			size_t* prev_end_ptr;
			size_t current_end;

			lazy_assign(size_t& prev_end_ptr, size_t current_end)
			    : prev_end_ptr{&prev_end_ptr}, current_end{current_end} {}
			~lazy_assign() { *prev_end_ptr = current_end; }
		};

		for (auto const& tag : elems) {
			lazy_assign save{prev_end, tag.name.stop};
			if (!in_form) {
				if (tag.name.is("form")) {
					auto [id, name, action, method, enctype] =
					    form_attrs(tag.attrs);
					if (!id.empty()) {
						form_id = attr_decode(id);
					} else if (!name.empty()) {
						form_id = attr_decode(name);
						auto id = result.find(form_id);
						// name has lower priority from id
						if (id != result.end()) form_id.clear();
					}
					in_form = !form_id.empty();
					if (in_form) {
						current = &result[form_id];
						*current = {std::move(form_id), attr_decode(action),
						            attr_decode(method), attr_decode(enctype)};
					}
				}
				continue;
			}

			auto const is_input = tag.name.is("input"sv);
			auto const is_select = tag.name.is("select"sv);
			auto const is_textarea = tag.name.is("textarea"sv);
			auto const is_textarea_end = tag.name.is("/textarea"sv);
			auto const is_form_end = tag.name.is("/form"sv);

			if (is_input || is_select) {
				auto [name, type, value, is_checked] = input_attrs(tag.attrs);
				if (name.empty()) continue;

				auto const is_radio = attr_pos::is(type, "radio"sv);
				auto const is_checkbox = attr_pos::is(type, "checkbox"sv);

				if ((is_radio || is_checkbox) && !is_checked) continue;

				if (is_radio)
					current->set(attr_decode(name), attr_decode(value));
				else
					current->add(attr_decode(name), attr_decode(value));

				continue;
			}

			if (is_textarea) {
				auto [name, type, value, is_checked] = input_attrs(tag.attrs);
				if (name.empty()) continue;
				prev_textarea = tag.name.stop;
				prev_textarea_name = name;
				continue;
			}

			if (is_textarea_end) {
				if (prev_textarea_name.empty() || prev_textarea != prev_end)
					continue;
				auto value =
				    html.substr(prev_textarea, tag.name.start - prev_textarea);

				current->add(attr_decode(prev_textarea_name),
				             attr_decode(value));
				continue;
			}

			if (is_form_end) {
				in_form = false;
				current = nullptr;
				continue;
			}
		}

		return result;
	}
}  // namespace tangle
