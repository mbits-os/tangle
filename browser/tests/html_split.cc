// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/html_split.hpp>
#include "cxx_string.hh"
#include <gtest/gtest.h>

namespace tangle::browser::testing {
	struct test_attr_pos {
		std::string_view view;
		std::string_view name;
		std::string_view value;
		size_t name_start;
		size_t start;
		size_t stop;
	};

	struct test_elem {
		std::string_view view;
		struct name_info {
			std::string_view value{};
			size_t start{};
			size_t stop{};
			bool autoclose{false};
		} name;
		std::vector<test_attr_pos> attrs{};
	};

	struct html_split_test_data {
		std::string_view text;
		std::vector<test_elem> expected;
		std::vector<std::string_view> textnodes{};
	};

	struct attr_text_test_data {
		std::string_view raw;
		std::string_view decoded{};
		std::string_view reencoded{};
	};

	class html_split : public ::testing::TestWithParam<html_split_test_data> {};
	class attr_text : public ::testing::TestWithParam<attr_text_test_data> {};

	TEST_P(html_split, elems) {
		auto& param = GetParam();
		auto actual = browser::html_split(param.text);
#if 0
		for (auto const& tag : actual) {
			std::cerr << "{R\"("
			          << param.text.substr(tag.name.start,
			                               tag.name.stop - tag.name.start)
			          << ")\", {\"" << tag.name.value << "\", "
			          << tag.name.start << ", " << tag.name.stop;
			if (tag.name.autoclose) std::cerr << ", true";
			std::cerr << "}, {";
			for (auto const& [name, attr] : tag.attrs) {
				std::cerr << "{R\"("
				          << param.text.substr(attr.name_start,
				                               attr.stop - attr.name_start)
				          << ")\", \"" << name << "\", \"" << attr.value
				          << "\", " << attr.name_start << ", " << attr.start
				          << ", " << attr.stop << "},";
			}
			std::cerr << "}},\n";
		}
#endif
		ASSERT_EQ(param.expected.size(), actual.size());

		auto it = actual.begin();
		for (auto const& exp : param.expected) {
			auto const& act = *it++;
			auto const tag_view = param.text.substr(
			    act.name.start, act.name.stop - act.name.start);

			ASSERT_EQ(exp.view, tag_view) << "Tag: " << exp.name.value;
			EXPECT_EQ(exp.name.start, act.name.start)
			    << "Tag: " << exp.name.value;
			EXPECT_EQ(exp.name.stop, act.name.stop)
			    << "Tag: " << exp.name.value;
			EXPECT_EQ(exp.name.autoclose, act.name.autoclose)
			    << "Tag: " << exp.name.value;
			EXPECT_EQ(exp.name.value, act.name.value);
			ASSERT_EQ(exp.attrs.size(), act.attrs.size())
			    << "Tag: " << exp.name.value;

			for (auto const& exp_attr : exp.attrs) {
				auto attr_it = act.attrs.find(exp_attr.name);
				ASSERT_NE(attr_it, act.attrs.end())
				    << "Missing atribute in parsed text: " << exp_attr.name
				    << "\nTag: " << exp.view;
				auto& act_attr = *attr_it;

				auto act_view = param.text.substr(
				    act_attr.second.name_start,
				    act_attr.second.stop - act_attr.second.name_start);

				ASSERT_EQ(exp_attr.view, act_view)
				    << "Attr: " << exp.name.value << '@' << exp_attr.name;
				EXPECT_EQ(exp_attr.name, act_attr.first);
				EXPECT_EQ(exp_attr.name_start, act_attr.second.name_start)
				    << "Attr: " << exp.name.value << '@' << exp_attr.name;
				EXPECT_EQ(exp_attr.start, act_attr.second.start)
				    << "Attr: " << exp.name.value << '@' << exp_attr.name;
				EXPECT_EQ(exp_attr.stop, act_attr.second.stop)
				    << "Attr: " << exp.name.value << '@' << exp_attr.name;
			}
		}
	}

	TEST_P(attr_text, decode_encode) {
		auto& param = GetParam();
		auto decoded = attr_decode(param.raw);
		auto reencoded = attr_encode(decoded);
#if 0
		std::cerr << "{" << cxx_string{param.raw} << ", " << cxx_string{decoded}
		          << ", " << cxx_string{reencoded} << "},\n";
#endif
		ASSERT_EQ(param.decoded, decoded);
		ASSERT_EQ(param.reencoded, reencoded);
	}

	static html_split_test_data const data[] = {
	    {},
	    {
	        R"(<?xml version="1.0" standalone="yes" ?>
<html ns:lang="en-US">
<head></head>
<body>
    <div id="body&amp;div" class="simple-div"></div>
    <funny:looking-here />
    <video src=video.mp4 autoplay></video>
	<Script type=none>
		text of the script
		<!-- this is not a script -->
	</script>
</body>
</html>)",
	        {
	            {R"(<?xml version="1.0" standalone="yes" ?>)",
	             {"?xml", 0, 39},
	             {
	                 {R"(standalone="yes")", "standalone", "yes", 20, 31, 36},
	                 {R"(version="1.0")", "version", "1.0", 6, 14, 19},
	             }},
	            {R"(<html ns:lang="en-US">)",
	             {"html", 40, 62},
	             {
	                 {R"(ns:lang="en-US")", "ns:lang", "en-US", 46, 54, 61},
	             }},
	            {R"(<head>)", {"head", 63, 69}, {}},
	            {R"(</head>)", {"/head", 69, 76}, {}},
	            {R"(<body>)", {"body", 77, 83}, {}},
	            {R"(<div id="body&amp;div" class="simple-div">)",
	             {"div", 88, 130},
	             {
	                 {R"(class="simple-div")", "class", "simple-div", 111, 117,
	                  129},
	                 {R"(id="body&amp;div")", "id", "body&amp;div", 93, 96,
	                  110},
	             }},
	            {R"(</div>)", {"/div", 130, 136}, {}},
	            {R"(<funny:looking-here />)",
	             {"funny:looking-here", 141, 163, true},
	             {}},
	            {R"(<video src=video.mp4 autoplay>)",
	             {"video", 168, 198},
	             {
	                 {R"(autoplay)", "autoplay", "", 189, 197, 197},
	                 {R"(src=video.mp4)", "src", "video.mp4", 175, 179, 188},
	             }},
	            {R"(</video>)", {"/video", 198, 206}, {}},
	            {R"(<Script type=none>)",
	             {"Script", 208, 226},
	             {
	                 {R"(type=none)", "type", "none", 216, 221, 225},
	             }},
	            {R"(</script>)", {"/script", 281, 290}, {}},
	            {R"(</body>)", {"/body", 291, 298}, {}},
	            {R"(</html>)", {"/html", 299, 306}, {}},
	        },
	    },

	    {
	        R"(<html><body>
<form method="post"
      enctype="application/x-www-form-urlencoded"
      action="https://pizza.example.com/order.cgi">
 <p><label>Customer name: <input name="custname"></label></p>
 <p><label>Telephone: <input type=tel name="custtel"></label></p>
 <p><label>Email address: <input type=email name="custemail"></label></p>
 <fieldset>
  <!-- comments -->
  <legend> Pizza Size </legend>
  <p><label> <input type=radio name=size value="small"> Small </label></p>
  <p><label> <input type=radio name=size value="medium"> Medium </label></p>
  <p><label> <input type=radio name=size value="large"> Large </label></p>
 </fieldset>
 <fieldset>
  <legend> Pizza Toppings </legend>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; bacon"> Bacon </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo cheese"> Extra Cheese </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; onion"> Onion </label></p>
  <p><label> <input type=checkbox name="topping" value="topping &raquo; mushroom"> Mushroom </label></p>
 </fieldset>
 <p><label>Preferred delivery time: <input type=time min="11:00" max="21:00" step="900" name="delivery"></label></p>
 <p><label>Delivery instructions: <TextArea name="comments">    a
text inside

</nope

textarea: <b>bold</b></textarea></label></p>
 <p><button>Submit order</button></p>
</form></body></html>)"sv,
	        {
	            {R"(<html>)", {"html", 0, 6}, {}},
	            {R"(<body>)", {"body", 6, 12}, {}},
	            {R"(<form method="post"
      enctype="application/x-www-form-urlencoded"
      action="https://pizza.example.com/order.cgi">)",
	             {"form", 13, 134},
	             {
	                 {R"(action="https://pizza.example.com/order.cgi")",
	                  "action", "https://pizza.example.com/order.cgi", 89, 96,
	                  133},
	                 {R"(enctype="application/x-www-form-urlencoded")",
	                  "enctype", "application/x-www-form-urlencoded", 39, 47,
	                  82},
	                 {R"(method="post")", "method", "post", 19, 26, 32},
	             }},
	            {R"(<p>)", {"p", 136, 139}, {}},
	            {R"(<label>)", {"label", 139, 146}, {}},
	            {R"(<input name="custname">)",
	             {"input", 161, 184, true},
	             {
	                 {R"(name="custname")", "name", "custname", 168, 173, 183},
	             }},
	            {R"(</label>)", {"/label", 184, 192}, {}},
	            {R"(</p>)", {"/p", 192, 196}, {}},
	            {R"(<p>)", {"p", 198, 201}, {}},
	            {R"(<label>)", {"label", 201, 208}, {}},
	            {R"(<input type=tel name="custtel">)",
	             {"input", 219, 250, true},
	             {
	                 {R"(name="custtel")", "name", "custtel", 235, 240, 249},
	                 {R"(type=tel)", "type", "tel", 226, 231, 234},
	             }},
	            {R"(</label>)", {"/label", 250, 258}, {}},
	            {R"(</p>)", {"/p", 258, 262}, {}},
	            {R"(<p>)", {"p", 264, 267}, {}},
	            {R"(<label>)", {"label", 267, 274}, {}},
	            {R"(<input type=email name="custemail">)",
	             {"input", 289, 324, true},
	             {
	                 {R"(name="custemail")", "name", "custemail", 307, 312,
	                  323},
	                 {R"(type=email)", "type", "email", 296, 301, 306},
	             }},
	            {R"(</label>)", {"/label", 324, 332}, {}},
	            {R"(</p>)", {"/p", 332, 336}, {}},
	            {R"(<fieldset>)", {"fieldset", 338, 348}, {}},
	            {R"(<!-- comments -->)", {"!--", 351, 368, true}, {}},
	            {R"(<legend>)", {"legend", 371, 379}, {}},
	            {R"(</legend>)", {"/legend", 391, 400}, {}},
	            {R"(<p>)", {"p", 403, 406}, {}},
	            {R"(<label>)", {"label", 406, 413}, {}},
	            {R"(<input type=radio name=size value="small">)",
	             {"input", 414, 456, true},
	             {
	                 {R"(value="small")", "value", "small", 442, 448, 455},
	                 {R"(name=size)", "name", "size", 432, 437, 441},
	                 {R"(type=radio)", "type", "radio", 421, 426, 431},
	             }},
	            {R"(</label>)", {"/label", 463, 471}, {}},
	            {R"(</p>)", {"/p", 471, 475}, {}},
	            {R"(<p>)", {"p", 478, 481}, {}},
	            {R"(<label>)", {"label", 481, 488}, {}},
	            {R"(<input type=radio name=size value="medium">)",
	             {"input", 489, 532, true},
	             {
	                 {R"(value="medium")", "value", "medium", 517, 523, 531},
	                 {R"(name=size)", "name", "size", 507, 512, 516},
	                 {R"(type=radio)", "type", "radio", 496, 501, 506},
	             }},
	            {R"(</label>)", {"/label", 540, 548}, {}},
	            {R"(</p>)", {"/p", 548, 552}, {}},
	            {R"(<p>)", {"p", 555, 558}, {}},
	            {R"(<label>)", {"label", 558, 565}, {}},
	            {R"(<input type=radio name=size value="large">)",
	             {"input", 566, 608, true},
	             {
	                 {R"(value="large")", "value", "large", 594, 600, 607},
	                 {R"(name=size)", "name", "iz", 584, 589, 593},
	                 {R"(type=radio)", "type", "adi", 573, 578, 583},
	             }},
	            {R"(</label>)", {"/label", 615, 623}, {}},
	            {R"(</p>)", {"/p", 623, 627}, {}},
	            {R"(</fieldset>)", {"/fieldset", 629, 640}, {}},
	            {R"(<fieldset>)", {"fieldset", 642, 652}, {}},
	            {R"(<legend>)", {"legend", 655, 663}, {}},
	            {R"(</legend>)", {"/legend", 679, 688}, {}},
	            {R"(<p>)", {"p", 691, 694}, {}},
	            {R"(<label>)", {"label", 694, 701}, {}},
	            {R"(<input type=checkbox name="topping" value="topping &raquo; bacon">)",
	             {"input", 702, 768, true},
	             {
	                 {R"(value="topping &raquo; bacon")", "value",
	                  "topping &raquo; bacon", 738, 744, 767},
	                 {R"(name="topping")", "name", "topping", 723, 728, 737},
	                 {R"(type=checkbox)", "type", "checkbox", 709, 714, 722},
	             }},
	            {R"(</label>)", {"/label", 775, 783}, {}},
	            {R"(</p>)", {"/p", 783, 787}, {}},
	            {R"(<p>)", {"p", 790, 793}, {}},
	            {R"(<label>)", {"label", 793, 800}, {}},
	            {R"(<input type=checkbox name="topping" value="topping &raquo cheese">)",
	             {"input", 801, 867, true},
	             {
	                 {R"(value="topping &raquo cheese")", "value",
	                  "topping &raquo cheese", 837, 843, 866},
	                 {R"(name="topping")", "name", "topping", 822, 827, 836},
	                 {R"(type=checkbox)", "type", "checkbox", 808, 813, 821},
	             }},
	            {R"(</label>)", {"/label", 881, 889}, {}},
	            {R"(</p>)", {"/p", 889, 893}, {}},
	            {R"(<p>)", {"p", 896, 899}, {}},
	            {R"(<label>)", {"label", 899, 906}, {}},
	            {R"(<input type=checkbox name="topping" value="topping &raquo; onion">)",
	             {"input", 907, 973, true},
	             {
	                 {R"(value="topping &raquo; onion")", "value",
	                  "topping &raquo; onion", 943, 949, 972},
	                 {R"(name="topping")", "name", "topping", 928, 933, 942},
	                 {R"(type=checkbox)", "type", "checkbox", 914, 919, 927},
	             }},
	            {R"(</label>)", {"/label", 980, 988}, {}},
	            {R"(</p>)", {"/p", 988, 992}, {}},
	            {R"(<p>)", {"p", 995, 998}, {}},
	            {R"(<label>)", {"label", 998, 1005}, {}},
	            {R"(<input type=checkbox name="topping" value="topping &raquo; mushroom">)",
	             {"input", 1006, 1075, true},
	             {
	                 {R"(value="topping &raquo; mushroom")", "value",
	                  "topping &raquo; mushroom", 1042, 1048, 1074},
	                 {R"(name="topping")", "name", "topping", 1027, 1032, 1041},
	                 {R"(type=checkbox)", "type", "checkbox", 1013, 1018, 1026},
	             }},
	            {R"(</label>)", {"/label", 1085, 1093}, {}},
	            {R"(</p>)", {"/p", 1093, 1097}, {}},
	            {R"(</fieldset>)", {"/fieldset", 1099, 1110}, {}},
	            {R"(<p>)", {"p", 1112, 1115}, {}},
	            {R"(<label>)", {"label", 1115, 1122}, {}},
	            {R"(<input type=time min="11:00" max="21:00" step="900" name="delivery">)",
	             {"input", 1147, 1215, true},
	             {
	                 {R"(step="900")", "step", "900", 1188, 1193, 1198},
	                 {R"(max="21:00")", "max", "21:00", 1176, 1180, 1187},
	                 {R"(min="11:00")", "min", "11:00", 1164, 1168, 1175},
	                 {R"(name="delivery")", "name", "delivery", 1199, 1204,
	                  1214},
	                 {R"(type=time)", "type", "time", 1154, 1159, 1163},
	             }},
	            {R"(</label>)", {"/label", 1215, 1223}, {}},
	            {R"(</p>)", {"/p", 1223, 1227}, {}},
	            {R"(<p>)", {"p", 1229, 1232}, {}},
	            {R"(<label>)", {"label", 1232, 1239}, {}},
	            {R"(<TextArea name="comments">)",
	             {"TextArea", 1262, 1288},
	             {
	                 {R"(name="comments")", "name", "comments", 1272, 1277,
	                  1287},
	             }},
	            {R"(</textarea>)", {"/textarea", 1336, 1347}, {}},
	            {R"(</label>)", {"/label", 1347, 1355}, {}},
	            {R"(</p>)", {"/p", 1355, 1359}, {}},
	            {R"(<p>)", {"p", 1361, 1364}, {}},
	            {R"(<button>)", {"button", 1364, 1372}, {}},
	            {R"(</button>)", {"/button", 1384, 1393}, {}},
	            {R"(</p>)", {"/p", 1393, 1397}, {}},
	            {R"(</form>)", {"/form", 1398, 1405}, {}},
	            {R"(</body>)", {"/body", 1405, 1412}, {}},
	            {R"(</html>)", {"/html", 1412, 1419}, {}},
	        },
	    },
	    {
	        R"(<title>Test for is_text_elem</title>
<span>?</span>
<sTYle>
/*neverending elem*/)",
	        {
	            {R"(<title>)", {"title", 0, 7}, {}},
	            {R"(</title>)", {"/title", 28, 36}, {}},
	            {R"(<span>)", {"span", 37, 43}, {}},
	            {R"(</span>)", {"/span", 44, 51}, {}},
	            {R"(<sTYle>)", {"sTYle", 52, 59}, {}},
	        },
	    },
	    {
	        R"(<title>Test for is_text_elem</title>
<ol></ol>
<option><ruby></ruby></option>
<rp></rp><rt></rt>
<table></table>)",
	        {
	            {R"(<title>)", {"title", 0, 7}, {}},
	            {R"(</title>)", {"/title", 28, 36}, {}},
	            {R"(<ol>)", {"ol", 37, 41}, {}},
	            {R"(</ol>)", {"/ol", 41, 46}, {}},
	            {R"(<option>)", {"option", 47, 55}, {}},
	            {R"(</option>)", {"/option", 68, 77}, {}},
	            {R"(<rp>)", {"rp", 78, 82}, {}},
	            {R"(</rp>)", {"/rp", 82, 87}, {}},
	            {R"(<rt>)", {"rt", 87, 91}, {}},
	            {R"(</rt>)", {"/rt", 91, 96}, {}},
	            {R"(<table>)", {"table", 97, 104}, {}},
	            {R"(</table>)", {"/table", 104, 112}, {}},
	        },
	    },
	    {
	        R"(<html>
<head>
<title>Test for infinite comment</title>
<!-- there is no end to this comment:
</head>
<body>
<p>This is not showing up</p>
</body>
</html>)",
	        {
	            {R"(<html>)", {"html", 0, 6}, {}},
	            {R"(<head>)", {"head", 7, 13}, {}},
	            {R"(<title>)", {"title", 14, 21}, {}},
	            {R"(</title>)", {"/title", 46, 54}, {}},
	            {R"(<!-- there is no end to this comment:
</head>
<body>
<p>This is not showing up</p>
</body>
</html>)",
	             {"!--", 55, 153, true},
	             {}},
	        },
	    },
	    {
	        R"(<!DOCTYPE html>
<!SOMETHING-ELSE wibbly-wobbly timey-wimey "stuff">
<html>
<head>
<title>Test for <! which is not a comment</title>
</head>
<body>
<p>This is showing up</p>
</body>
</html>)",
	        {
	            {R"(<!DOCTYPE html>)", {"!DOCTYPE", 0, 15}, {}},
	            {
	                R"(<!SOMETHING-ELSE wibbly-wobbly timey-wimey "stuff">)",
	                {"!SOMETHING-ELSE", 16, 67},
	                {},
	            },
	            {R"(<html>)", {"html", 68, 74}, {}},
	            {R"(<head>)", {"head", 75, 81}, {}},
	            {R"(<title>)", {"title", 82, 89}, {}},
	            {R"(</title>)", {"/title", 123, 131}, {}},
	            {R"(</head>)", {"/head", 132, 139}, {}},
	            {R"(<body>)", {"body", 140, 146}, {}},
	            {R"(<p>)", {"p", 147, 150}, {}},
	            {R"(</p>)", {"/p", 168, 172}, {}},
	            {R"(</body>)", {"/body", 173, 180}, {}},
	            {R"(</html>)", {"/html", 181, 188}, {}},
	        },
	    },
	    {
	        R"(<title>Test for void element</title>
<area href=#>
<base target=elsewhere>
<br>
<col span=3>
<embed id=object>
<hr>
<img src=flower.png>
<input type=radio>
<keygen keytype="rsa">
<link rel=stylesheet href=style.css>
<meta charset="UTF-8">
<param name=name value=None>
<source type=video/mp4>
<track kind=chapters src=movie.vtt>
<wbr>)",
	        {
	            {R"(<title>)", {"title", 0, 7}, {}},
	            {R"(</title>)", {"/title", 28, 36}, {}},
	            {R"(<area href=#>)",
	             {"area", 37, 50, true},
	             {
	                 {R"(href=#)", "href", "#", 43, 48, 49},
	             }},
	            {R"(<base target=elsewhere>)",
	             {"base", 51, 74, true},
	             {
	                 {R"(target=elsewhere)", "target", "elsewhere", 57, 64, 73},
	             }},
	            {R"(<br>)", {"br", 75, 79, true}, {}},
	            {R"(<col span=3>)",
	             {"col", 80, 92, true},
	             {
	                 {R"(span=3)", "span", "3", 85, 90, 91},
	             }},
	            {R"(<embed id=object>)",
	             {"embed", 93, 110, true},
	             {
	                 {R"(id=object)", "id", "object", 100, 103, 109},
	             }},
	            {R"(<hr>)", {"hr", 111, 115, true}, {}},
	            {R"(<img src=flower.png>)",
	             {"img", 116, 136, true},
	             {
	                 {R"(src=flower.png)", "src", "flower.png", 121, 125, 135},
	             }},
	            {R"(<input type=radio>)",
	             {"input", 137, 155, true},
	             {
	                 {R"(type=radio)", "type", "radio", 144, 149, 154},
	             }},
	            {R"(<keygen keytype="rsa">)",
	             {"keygen", 156, 178, true},
	             {
	                 {R"(keytype="rsa")", "keytype", "rsa", 164, 172, 177},
	             }},
	            {R"(<link rel=stylesheet href=style.css>)",
	             {"link", 179, 215, true},
	             {
	                 {R"(href=style.css)", "href", "style.css", 200, 205, 214},
	                 {R"(rel=stylesheet)", "rel", "stylesheet", 185, 189, 199},
	             }},
	            {R"(<meta charset="UTF-8">)",
	             {"meta", 216, 238, true},
	             {
	                 {R"(charset="UTF-8")", "charset", "UTF-8", 222, 230, 237},
	             }},
	            {R"(<param name=name value=None>)",
	             {"param", 239, 267, true},
	             {
	                 {R"(value=None)", "value", "None", 256, 262, 266},
	                 {R"(name=name)", "name", "name", 246, 251, 255},
	             }},
	            {R"(<source type=video/mp4>)",
	             {"source", 268, 291, true},
	             {
	                 {R"(type=video/mp4)", "type", "video/mp4", 276, 281, 290},
	             }},
	            {R"(<track kind=chapters src=movie.vtt>)",
	             {"track", 292, 327, true},
	             {
	                 {R"(src=movie.vtt)", "src", "movie.vtt", 313, 317, 326},
	                 {R"(kind=chapters)", "kind", "chapters", 299, 304, 312},
	             }},
	            {R"(<wbr>)", {"wbr", 328, 333, true}, {}},
	        },
	    },
	    {
	        R"(<title>Test for almost-void element</title>
<areas href=#>
<baze target=elsewhere>
<brake>
<coll span=3>
<commands type=radio>
<cat>
<embedded id=object>
<hrule>
<image src=flower.png>
<input.z type=radio>
<ident>
<i>
<K.gen keytype="rsa">
<links rel=stylesheet href=style.css>
<meta-data charset="UTF-8">
<parameter name=name value=None>
<sources type=video/mp4>
<trac kind=chapters src=movie.vtt>
<w-br>)",
	        {
	            {R"(<title>)", {"title", 0, 7}, {}},
	            {R"(</title>)", {"/title", 35, 43}, {}},
	            {R"(<areas href=#>)",
	             {"areas", 44, 58},
	             {
	                 {R"(href=#)", "href", "#", 51, 56, 57},
	             }},
	            {R"(<baze target=elsewhere>)",
	             {"baze", 59, 82},
	             {
	                 {R"(target=elsewhere)", "target", "elsewhere", 65, 72, 81},
	             }},
	            {R"(<brake>)", {"brake", 83, 90}, {}},
	            {R"(<coll span=3>)",
	             {"coll", 91, 104},
	             {
	                 {R"(span=3)", "span", "3", 97, 102, 103},
	             }},
	            {R"(<commands type=radio>)",
	             {"commands", 105, 126},
	             {
	                 {R"(type=radio)", "type", "radio", 115, 120, 125},
	             }},
	            {R"(<cat>)", {"cat", 127, 132}, {}},
	            {R"(<embedded id=object>)",
	             {"embedded", 133, 153},
	             {
	                 {R"(id=object)", "id", "object", 143, 146, 152},
	             }},
	            {R"(<hrule>)", {"hrule", 154, 161}, {}},
	            {R"(<image src=flower.png>)",
	             {"image", 162, 184},
	             {
	                 {R"(src=flower.png)", "src", "flower.png", 169, 173, 183},
	             }},
	            {R"(<input.z type=radio>)",
	             {"input.z", 185, 205},
	             {
	                 {R"(type=radio)", "type", "radio", 194, 199, 204},
	             }},
	            {R"(<ident>)", {"ident", 206, 213}, {}},
	            {R"(<i>)", {"i", 214, 217}, {}},
	            {R"(<K.gen keytype="rsa">)",
	             {"K.gen", 218, 239},
	             {
	                 {R"(keytype="rsa")", "keytype", "rsa", 225, 233, 238},
	             }},
	            {R"(<links rel=stylesheet href=style.css>)",
	             {"links", 240, 277},
	             {
	                 {R"(href=style.css)", "href", "style.css", 262, 267, 276},
	                 {R"(rel=stylesheet)", "rel", "stylesheet", 247, 251, 261},
	             }},
	            {R"(<meta-data charset="UTF-8">)",
	             {"meta-data", 278, 305},
	             {
	                 {R"(charset="UTF-8")", "charset", "UTF-8", 289, 297, 304},
	             }},
	            {R"(<parameter name=name value=None>)",
	             {"parameter", 306, 338},
	             {
	                 {R"(value=None)", "value", "None", 327, 333, 337},
	                 {R"(name=name)", "name", "name", 317, 322, 326},
	             }},
	            {R"(<sources type=video/mp4>)",
	             {"sources", 339, 363},
	             {
	                 {R"(type=video/mp4)", "type", "video/mp4", 348, 353, 362},
	             }},
	            {R"(<trac kind=chapters src=movie.vtt>)",
	             {"trac", 364, 398},
	             {
	                 {R"(src=movie.vtt)", "src", "movie.vtt", 384, 388, 397},
	                 {R"(kind=chapters)", "kind", "chapters", 370, 375, 383},
	             }},
	            {R"(<w-br>)", {"w-br", 399, 405}, {}},
	        },
	    },
	    {
	        R"(<tag attr="value" >)",
	        {
	            {
	                R"(<tag attr="value" >)",
	                {"tag", 0, 19},
	                {
	                    {R"(attr="value")", "attr", "value", 5, 10, 17},
	                },
	            },
	        },
	    },
	    {
	        R"(<tag attr=value >)",
	        {
	            {
	                R"(<tag attr=value >)",
	                {"tag", 0, 17},
	                {
	                    {R"(attr=value)", "attr", "value", 5, 10, 15},
	                },
	            },
	        },
	    },
	    {
	        R"(<tag lazyily= written >)",
	        {
	            {
	                R"(<tag lazyily= written >)",
	                {"tag", 0, 23},
	                {
	                    {R"(lazyily= written)", "lazyily", "written", 5, 14,
	                     21},
	                },
	            },
	        },
	    },
	    {
	        R"(<tag lazyily =written >)",
	        {
	            {
	                R"(<tag lazyily =written >)",
	                {"tag", 0, 23},
	                {
	                    {R"(lazyily =written)", "lazyily", "written", 5, 14,
	                     21},
	                },
	            },
	        },
	    },
	    {
	        R"(<tag lazyily = "written">)",
	        {
	            {
	                R"(<tag lazyily = "written">)",
	                {"tag", 0, 25},
	                {
	                    {R"(lazyily = "written")", "lazyily", "written", 5, 15,
	                     24},
	                },
	            },
	        },
	    },
	    {
	        R"(<?xml-stylesheet type="text/xsl" href="style.xsl"?>)",
	        {
	            {R"(<?xml-stylesheet type="text/xsl" href="style.xsl"?>)",
	             {"?xml-stylesheet", 0, 51},
	             {
	                 {R"(href="style.xsl")", "href", "style.xsl", 33, 38, 49},
	                 {R"(type="text/xsl")", "type", "text/xsl", 17, 22, 32},
	             }},
	        },
	    },
	    {"<!DOCTYPE html><!SOMETHING-ELSE wibbly-wobbly timey-wimey stuff", {}},
	    {
	        R"(<?xml-stylesheet type="text/xsl" href="style.xsl">)",
	        {},
	    },
	    {
	        R"(<?0-xml-stylesheet type="text/xsl" href="style.xsl"?>)",
	        {},
	    },
	    {"<br /", {}},
	    {"<br / >", {}},
	};

	static constexpr attr_text_test_data attributes[] = {
	    {{}, {}, {}},
	    {"plain"sv, "plain"sv, "plain"sv},
	    {"&amp;amp;"sv, "&amp;"sv, "&amp;amp;"sv},
	    {
	        "&quot;quoted &raquo; string&quot;"sv,
	        "\"quoted \xc2\xbb string\""sv,
	        "&quot;quoted \xc2\xbb string&quot;"sv,
	    },
	    {">> title <<"sv, ">> title <<"sv, "&gt;&gt; title &lt;&lt;"sv},
	    {
	        ">> &title <<"sv,
	        ">> &title <<"sv,
	        "&gt;&gt; &amp;title &lt;&lt;"sv,
	    },
	    {
	        ">> &title; <<"sv,
	        ">> &title; <<"sv,
	        "&gt;&gt; &amp;title; &lt;&lt;"sv,
	    },
	    {"&DoubleLongLeftRightArrow", "\xe2\x9f\xba", "\xe2\x9f\xba"},
	    {"&#187; &#x00bB;", "\xc2\xbb \xc2\xbb", "\xc2\xbb \xc2\xbb"},
	    {"&#187; &#x01G;", "\xc2\xbb \001G;", "\xc2\xbb \001G;"},
	    {"&#x2728;", "\xE2\x9C\xA8", "\xE2\x9C\xA8"},
	    {"Sur-high &#xd810;", "Sur-high \xEF\xBF\xBD", "Sur-high \xEF\xBF\xBD"},
	    {"&#x1F600;", "\xF0\x9F\x98\x80", "\xF0\x9F\x98\x80"},
	    {
	        "Above legal UTF32: &#x00110000;",
	        "Above legal UTF32: \xEF\xBF\xBD",
	        "Above legal UTF32: \xEF\xBF\xBD",
	    },
	};

	INSTANTIATE_TEST_SUITE_P(samples, html_split, ::testing::ValuesIn(data));
	INSTANTIATE_TEST_SUITE_P(attributes,
	                         attr_text,
	                         ::testing::ValuesIn(attributes));
}  // namespace tangle::browser::testing
