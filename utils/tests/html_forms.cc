// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/html_forms.hpp>
#include "cxx_string.hh"

using namespace std::literals;

namespace tangle::testing {
	struct html_field_test_data {
		std::string_view name{};
		std::initializer_list<std::string_view> values{};
	};

	struct html_form_test_data {
		struct header_data {
			std::string_view id{};
			std::string_view action{};
			std::string_view method{};
			std::string_view enctype{};
		} data;
		std::initializer_list<html_field_test_data> fields{};
	};

	struct html_forms_test_data {
		std::string_view html{};
		std::initializer_list<html_form_test_data> forms{};
	};

	inline std::string as_str(std::string_view view) {
		return {view.data(), view.size()};
	}

	class html_forms : public ::testing::TestWithParam<html_forms_test_data> {};
	TEST_P(html_forms, parse) {
		auto& param = GetParam();
		auto forms = tangle::html_forms(param.html);

#if 0
		for (auto const& [id, form] : forms) {
			std::cerr << "{\n  {" << cxx_string{form.id} << ", "
			          << cxx_string{form.action} << ", "
			          << cxx_string{form.method} << ", "
			          << cxx_string{form.enctype} << "}, {\n";

			for (auto const& field : form.fields) {
				std::cerr << "    {" << cxx_string{field.name} << ", {";
				bool first = true;
				for (auto const& value : field.values) {
					if (first)
						first = false;
					else
						std::cerr << ", ";
					std::cerr << cxx_string{value};
				}
				std::cerr << "}},\n";
			}
			std::cerr << "  },\n},\n";
		}
#endif
		ASSERT_EQ(param.forms.size(), forms.size());
		for (auto const& exp_form : param.forms) {
			auto form_it = forms.find(as_str(exp_form.data.id));
			ASSERT_NE(form_it, forms.end()) << "Key is: " << exp_form.data.id;
			auto const& [act_id, act_form] = *form_it++;
			ASSERT_EQ(exp_form.data.id, act_form.id);
			ASSERT_EQ(exp_form.data.action, act_form.action);
			ASSERT_EQ(exp_form.data.method, act_form.method);
			ASSERT_EQ(exp_form.data.enctype, act_form.enctype);

			ASSERT_EQ(exp_form.fields.size(), act_form.fields.size());
			auto field_it = act_form.fields.begin();
			for (auto const& exp_field : exp_form.fields) {
				auto const& act_field = *field_it++;
				ASSERT_EQ(exp_field.name, act_field.name);
				ASSERT_EQ(exp_field.values.size(), act_field.values.size());
				auto value_it = act_field.values.begin();
				for (auto const& exp_value : exp_field.values) {
					auto const& act_value = *value_it++;
					ASSERT_EQ(exp_value, act_value);
				}
			}
		}
	}

	static constexpr html_forms_test_data samples[] = {
	    {
	        R"(<html><body>
<form id="form-0" method="post"
      enctype="application/x-www-form-urlencoded"
      action="https://pizza.example.com/order.cgi">
 <p><label>Customer name: <input name="custname"></label></p>
 <p><label>Telephone: <input type=tel name="custtel"></label></p>
 <p><label>Email address: <input type=email name="custemail"></label></p>
 <fieldset>
  <!-- comments -->
  <legend> Pizza Size </legend>
  <p><label> <input type=radio name=size value="small"> Small </label></p>
  <p><label> <input type=radio name=size value="medium" checked> Medium </label></p>
  <p><label> <input type=radio name=size value="large" checked> Large </label></p>
 </fieldset>
 <fieldset>
  <legend> Pizza Toppings </legend>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; bacon"> Bacon </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo cheese" checked> Extra Cheese </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; onion"> Onion </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; mushroom" checked> Mushroom </label></p>
 </fieldset>
 <p><label>Preferred delivery time: <input type=time min="11:00" max="21:00" step="900" name="delivery"></label></p>
 <p><label>Delivery instructions: <TextArea name="comments">    a
text inside

</nope

textarea: <b>bold</b></textarea></label></p>
 <p> <textarea name="buffer" readonly># System-wide .bashrc file for interactive bash(1) shells.

# To enable the settings / commands in this file for login shells as well,
# this file has to be sourced in /etc/profile.

# If not running interactively, don't do anything
[ -z "$PS1" ] &amp;&amp; return <!--

...</textarea>
--></p>
 <p><button>Submit order</button></p>
</form></body></html>)"sv,
	        {
	            {
	                {
	                    "form-0"sv,
	                    "https://pizza.example.com/order.cgi"sv,
	                    "post"sv,
	                    "application/x-www-form-urlencoded"sv,
	                },
	                {
	                    {"custname"sv, {{}}},
	                    {"custtel"sv, {{}}},
	                    {"custemail"sv, {{}}},
	                    {"size"sv, {"large"sv}},
	                    {
	                        "topping"sv,
	                        {"topping \xc2\xbb cheese"sv,
	                         "topping \xc2\xbb mushroom"sv},
	                    },
	                    {"delivery"sv, {{}}},
	                    {
	                        "comments"sv,
	                        {"    a\ntext inside\n\n</nope\n\ntextarea: <b>bold</b>"sv},
	                    },
	                    {"buffer"sv,
	                     {"# System-wide .bashrc file for interactive bash(1) "
	                      "shells.\n\n# To enable the settings / commands in "
	                      "this "
	                      "file for login shells as well,\n# this file has to "
	                      "be "
	                      "sourced in /etc/profile.\n\n# If not running "
	                      "interactively, don't do anything\n[ -z \"$PS1\" ] "
	                      "&& "
	                      "return <!--\n\n..."sv}},
	                },
	            },
	        },
	    },
	    {
	        R"(<form id="form-0"></form>
<form name="form-1"></form>
<form></form>
<form name="form-0"></form>
)",
	        {
	            {{"form-0"sv, {}, {}, {}}, {}},
	            {{"form-1"sv, {}, {}, {}}, {}},
	        },
	    },
	    {
	        R"(<form name="A"><textarea name="comments"></textarea></form>
<form name="B"><input name="input" value="value"></textarea></form>
)",
	        {
	            {
	                {"A"sv, {}, {}, {}},
	                {{"comments"sv, {{}}}},
	            },
	            {
	                {"B"sv, {}, {}, {}},
	                {{"input"sv, {"value"sv}}},
	            },
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples, html_forms, ::testing::ValuesIn(samples));
}  // namespace tangle::testing