// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-dtd
// build/bin/html-dtd tools/lookup/html.dtd browser

#include <tangle/browser/html_dtd.hpp>
#include <gtest/gtest.h>

namespace tangle::browser::testing {
	struct html_element_data {
		std::string_view name;
		unsigned expected{58};
		friend std::ostream& operator<<(std::ostream& out, html_element_data const& data) {
			return out << data.name;
		}
	};

	class html_element: public ::testing::TestWithParam<html_element_data>{};
	TEST_P(html_element, lookup) {
		auto& param = GetParam();
		auto actual = browser::element_index(param.name);

		ASSERT_EQ(param.expected, actual);
	}

	static constexpr html_element_data names[] = {
	    {"a"sv, 0},
	    {"abbr"sv, 1},
	    {"address"sv, 2},
	    {"area"sv, 3},
	    {"article"sv, 4},
	    {"aside"sv, 4},
	    {"audio"sv, 5},
	    {"b"sv, 1},
	    {"base"sv, 6},
	    {"bdi"sv, 1},
	    {"bdo"sv, 1},
	    {"blockquote"sv, 7},
	    {"body"sv, 8},
	    {"br"sv, 3},
	    {"button"sv, 9},
	    {"canvas"sv, 10},
	    {"caption"sv, 11},
	    {"cite"sv, 1},
	    {"code"sv, 1},
	    {"col"sv, 12},
	    {"colgroup"sv, 13},
	    {"data"sv, 1},
	    {"datalist"sv, 14},
	    {"dd"sv, 8},
	    {"del"sv, 10},
	    {"details"sv, 15},
	    {"dfn"sv, 16},
	    {"dialog"sv, 7},
	    {"div"sv, 17},
	    {"dl"sv, 18},
	    {"dt"sv, 19},
	    {"em"sv, 1},
	    {"embed"sv, 20},
	    {"fieldset"sv, 21},
	    {"figcaption"sv, 8},
	    {"figure"sv, 22},
	    {"footer"sv, 23},
	    {"form"sv, 24},
	    {"h1"sv, 25},
	    {"h2"sv, 25},
	    {"h3"sv, 25},
	    {"h4"sv, 25},
	    {"h5"sv, 25},
	    {"h6"sv, 25},
	    {"head"sv, 26},
	    {"header"sv, 23},
	    {"hr"sv, 27},
	    {"html"sv, 28},
	    {"i"sv, 1},
	    {"iframe"sv, 29},
	    {"img"sv, 3},
	    {"input"sv, 20},
	    {"ins"sv, 10},
	    {"kbd"sv, 1},
	    {"keygen"sv, 3},
	    {"label"sv, 30},
	    {"legend"sv, 31},
	    {"li"sv, 8},
	    {"link"sv, 32},
	    {"main"sv, 7},
	    {"map"sv, 33},
	    {"mark"sv, 1},
	    {"math"sv, 10},
	    {"menu"sv, 34},
	    {"menuitem"sv, 12},
	    {"meta"sv, 6},
	    {"meter"sv, 35},
	    {"nav"sv, 4},
	    {"noscript"sv, 36},
	    {"object"sv, 10},
	    {"ol"sv, 37},
	    {"optgroup"sv, 38},
	    {"option"sv, 39},
	    {"output"sv, 1},
	    {"p"sv, 40},
	    {"param"sv, 12},
	    {"picture"sv, 41},
	    {"pre"sv, 42},
	    {"progress"sv, 43},
	    {"q"sv, 1},
	    {"rb"sv, 44},
	    {"rp"sv, 44},
	    {"rt"sv, 44},
	    {"rtc"sv, 45},
	    {"ruby"sv, 46},
	    {"s"sv, 1},
	    {"samp"sv, 1},
	    {"script"sv, 47},
	    {"section"sv, 48},
	    {"select"sv, 49},
	    {"small"sv, 1},
	    {"source"sv, 12},
	    {"span"sv, 1},
	    {"strong"sv, 1},
	    {"style"sv, 50},
	    {"sub"sv, 1},
	    {"summary"sv, 51},
	    {"sup"sv, 1},
	    {"svg"sv, 10},
	    {"table"sv, 52},
	    {"tbody"sv, 53},
	    {"td"sv, 8},
	    {"template"sv, 54},
	    {"textarea"sv, 55},
	    {"tfoot"sv, 53},
	    {"th"sv, 19},
	    {"thead"sv, 53},
	    {"time"sv, 1},
	    {"title"sv, 56},
	    {"tr"sv, 57},
	    {"track"sv, 12},
	    {"u"sv, 1},
	    {"ul"sv, 37},
	    {"var"sv, 1},
	    {"video"sv, 5},
	    {"wbr"sv, 3},
	};

	static constexpr html_element_data nosuch[] = {
	    {""sv},
	    {"0P0z"sv},  // fails tail of {}
	    {"ajbtQ"sv},  // fails leaf and tail of "a"sv
	    {"abHLMV"sv},  // fails tail of "ab"sv
	    {"abbrcsax"sv},  // fails leaf of "abbr"sv
	    {"adYPtB"sv},  // fails tail of "ad"sv
	    {"address0b7Z"sv},  // fails leaf of "address"sv
	    {"ar4FPk"sv},  // fails tail of "ar"sv
	    {"are5WJE"sv},  // fails tail of "are"sv
	    {"areanlYf"sv},  // fails leaf of "area"sv
	    {"artjXtR"sv},  // fails tail of "art"sv
	    {"articlew7pA"sv},  // fails leaf of "article"sv
	    {"asn7ZW"sv},  // fails tail of "as"sv
	    {"aside7xmg"sv},  // fails leaf of "aside"sv
	    {"auIA84"sv},  // fails tail of "au"sv
	    {"audioB1iH"sv},  // fails leaf of "audio"sv
	    {"bXqh7"sv},  // fails leaf and tail of "b"sv
	    {"baz19R"sv},  // fails tail of "ba"sv
	    {"baseEXP9"sv},  // fails leaf of "base"sv
	    {"bdHPNy"sv},  // fails tail of "bd"sv
	    {"bdivwqk"sv},  // fails leaf of "bdi"sv
	    {"bdo2cmf"sv},  // fails leaf of "bdo"sv
	    {"bl12Gj"sv},  // fails tail of "bl"sv
	    {"blockquoteLJvv"sv},  // fails leaf of "blockquote"sv
	    {"boiGRF"sv},  // fails tail of "bo"sv
	    {"bodya6pJ"sv},  // fails leaf of "body"sv
	    {"brUvM5"sv},  // fails leaf of "br"sv
	    {"bu7QIT"sv},  // fails tail of "bu"sv
	    {"buttonUKPw"sv},  // fails leaf of "button"sv
	    {"cjpJI"sv},  // fails tail of "c"sv
	    {"caB3ry"sv},  // fails tail of "ca"sv
	    {"canij84"sv},  // fails tail of "can"sv
	    {"canvasuSgC"sv},  // fails leaf of "canvas"sv
	    {"capwVgy"sv},  // fails tail of "cap"sv
	    {"captionqmjW"sv},  // fails leaf of "caption"sv
	    {"ciJawn"sv},  // fails tail of "ci"sv
	    {"citeoaJK"sv},  // fails leaf of "cite"sv
	    {"co6qgf"sv},  // fails tail of "co"sv
	    {"codBAsQ"sv},  // fails tail of "cod"sv
	    {"coderlUT"sv},  // fails leaf of "code"sv
	    {"colSi0R"sv},  // fails leaf and tail of "col"sv
	    {"colgroupoIh7"sv},  // fails leaf of "colgroup"sv
	    {"dSoXU"sv},  // fails tail of "d"sv
	    {"daUeaP"sv},  // fails tail of "da"sv
	    {"dat4BXV"sv},  // fails tail of "dat"sv
	    {"dataVKY6"sv},  // fails leaf and tail of "data"sv
	    {"datalistNdTS"sv},  // fails leaf of "datalist"sv
	    {"ddNilM"sv},  // fails leaf of "dd"sv
	    {"deTkqe"sv},  // fails tail of "de"sv
	    {"deltgcn"sv},  // fails leaf of "del"sv
	    {"det4fVV"sv},  // fails tail of "det"sv
	    {"detailsAlZx"sv},  // fails leaf of "details"sv
	    {"dfkzBc"sv},  // fails tail of "df"sv
	    {"dfntOxJ"sv},  // fails leaf of "dfn"sv
	    {"diGCSA"sv},  // fails tail of "di"sv
	    {"diatV0W"sv},  // fails tail of "dia"sv
	    {"dialogKs2a"sv},  // fails leaf of "dialog"sv
	    {"divAFGw"sv},  // fails leaf of "div"sv
	    {"dl6Oqs"sv},  // fails leaf of "dl"sv
	    {"dttQcD"sv},  // fails leaf of "dt"sv
	    {"eZy2S"sv},  // fails tail of "e"sv
	    {"emZadk"sv},  // fails leaf and tail of "em"sv
	    {"embednnZ2"sv},  // fails leaf of "embed"sv
	    {"f9GRy"sv},  // fails tail of "f"sv
	    {"fivK9Y"sv},  // fails tail of "fi"sv
	    {"fiem1LH"sv},  // fails tail of "fie"sv
	    {"fieldsetEoZi"sv},  // fails leaf of "fieldset"sv
	    {"figPLbz"sv},  // fails tail of "fig"sv
	    {"figcWO9a"sv},  // fails tail of "figc"sv
	    {"figcaptionZKSR"sv},  // fails leaf of "figcaption"sv
	    {"figuloPG"sv},  // fails tail of "figu"sv
	    {"figureOEzw"sv},  // fails leaf of "figure"sv
	    {"fomN3B"sv},  // fails tail of "fo"sv
	    {"fooDXeF"sv},  // fails tail of "foo"sv
	    {"footerfkLE"sv},  // fails leaf of "footer"sv
	    {"forFMNJ"sv},  // fails tail of "for"sv
	    {"form2xg0"sv},  // fails leaf of "form"sv
	    {"hianh"sv},  // fails tail of "h"sv
	    {"h1QOlW"sv},  // fails leaf of "h1"sv
	    {"h2NKEe"sv},  // fails leaf of "h2"sv
	    {"h3fajy"sv},  // fails leaf of "h3"sv
	    {"h4khTo"sv},  // fails leaf of "h4"sv
	    {"h5Xznh"sv},  // fails leaf of "h5"sv
	    {"h6ICOG"sv},  // fails leaf of "h6"sv
	    {"hedT97"sv},  // fails tail of "he"sv
	    {"heavARp"sv},  // fails tail of "hea"sv
	    {"headSPj4"sv},  // fails leaf and tail of "head"sv
	    {"headerQLTZ"sv},  // fails leaf of "header"sv
	    {"hraw20"sv},  // fails leaf of "hr"sv
	    {"htX7yV"sv},  // fails tail of "ht"sv
	    {"htmlzZNl"sv},  // fails leaf of "html"sv
	    {"iqWIJ"sv},  // fails leaf and tail of "i"sv
	    {"if53iQ"sv},  // fails tail of "if"sv
	    {"iframeKBDO"sv},  // fails leaf of "iframe"sv
	    {"imy3Yt"sv},  // fails tail of "im"sv
	    {"imgcJvh"sv},  // fails leaf of "img"sv
	    {"inbXUV"sv},  // fails tail of "in"sv
	    {"inpBUs7"sv},  // fails tail of "inp"sv
	    {"inputeYne"sv},  // fails leaf of "input"sv
	    {"insSVvA"sv},  // fails leaf of "ins"sv
	    {"kzq0u"sv},  // fails tail of "k"sv
	    {"kbDV3b"sv},  // fails tail of "kb"sv
	    {"kbdwOjd"sv},  // fails leaf of "kbd"sv
	    {"keHcui"sv},  // fails tail of "ke"sv
	    {"keygenP3GS"sv},  // fails leaf of "keygen"sv
	    {"lhkJY"sv},  // fails tail of "l"sv
	    {"lajHm6"sv},  // fails tail of "la"sv
	    {"labelxgLh"sv},  // fails leaf of "label"sv
	    {"leegko"sv},  // fails tail of "le"sv
	    {"legendusyn"sv},  // fails leaf of "legend"sv
	    {"liS9uM"sv},  // fails leaf and tail of "li"sv
	    {"linkR7WS"sv},  // fails leaf of "link"sv
	    {"mmpGj"sv},  // fails tail of "m"sv
	    {"maly9a"sv},  // fails tail of "ma"sv
	    {"maiU8BA"sv},  // fails tail of "mai"sv
	    {"mainholj"sv},  // fails leaf of "main"sv
	    {"mapo5YO"sv},  // fails leaf of "map"sv
	    {"mareuFP"sv},  // fails tail of "mar"sv
	    {"mark1uqL"sv},  // fails leaf of "mark"sv
	    {"matyF9P"sv},  // fails tail of "mat"sv
	    {"mathVcLA"sv},  // fails leaf of "math"sv
	    {"meXJhN"sv},  // fails tail of "me"sv
	    {"menvZpk"sv},  // fails tail of "men"sv
	    {"menuxmUV"sv},  // fails leaf and tail of "menu"sv
	    {"menuitemXRtN"sv},  // fails leaf of "menuitem"sv
	    {"met3Una"sv},  // fails tail of "met"sv
	    {"meta0sEn"sv},  // fails leaf of "meta"sv
	    {"meteAPvt"sv},  // fails tail of "mete"sv
	    {"meterKBG4"sv},  // fails leaf of "meter"sv
	    {"npeOg"sv},  // fails tail of "n"sv
	    {"nagPgE"sv},  // fails tail of "na"sv
	    {"navAgky"sv},  // fails leaf of "nav"sv
	    {"no9CnM"sv},  // fails tail of "no"sv
	    {"noscriptJPR1"sv},  // fails leaf of "noscript"sv
	    {"oYLYW"sv},  // fails tail of "o"sv
	    {"obVIgU"sv},  // fails tail of "ob"sv
	    {"objectsbm3"sv},  // fails leaf of "object"sv
	    {"oli0I2"sv},  // fails leaf of "ol"sv
	    {"opAbTa"sv},  // fails tail of "op"sv
	    {"optdaNZ"sv},  // fails tail of "opt"sv
	    {"optgoQDT"sv},  // fails tail of "optg"sv
	    {"optgroupH8yT"sv},  // fails leaf of "optgroup"sv
	    {"optiDrA7"sv},  // fails tail of "opti"sv
	    {"optionWSKH"sv},  // fails leaf of "option"sv
	    {"ouhZqk"sv},  // fails tail of "ou"sv
	    {"outputtZLN"sv},  // fails leaf of "output"sv
	    {"pFe7r"sv},  // fails leaf and tail of "p"sv
	    {"pavgHJ"sv},  // fails tail of "pa"sv
	    {"paramXQ6A"sv},  // fails leaf of "param"sv
	    {"piPhGs"sv},  // fails tail of "pi"sv
	    {"pictureLeF5"sv},  // fails leaf of "picture"sv
	    {"prdoG7"sv},  // fails tail of "pr"sv
	    {"preQrLR"sv},  // fails leaf of "pre"sv
	    {"protweX"sv},  // fails tail of "pro"sv
	    {"progressQ9Ac"sv},  // fails leaf of "progress"sv
	    {"q65d5"sv},  // fails leaf of "q"sv
	    {"rKHyJ"sv},  // fails tail of "r"sv
	    {"rbjnVN"sv},  // fails leaf of "rb"sv
	    {"rpag6A"sv},  // fails leaf of "rp"sv
	    {"rtuddI"sv},  // fails leaf and tail of "rt"sv
	    {"rtcUwa0"sv},  // fails leaf of "rtc"sv
	    {"ruPZSR"sv},  // fails tail of "ru"sv
	    {"rubyDrUa"sv},  // fails leaf of "ruby"sv
	    {"sEEoM"sv},  // fails leaf and tail of "s"sv
	    {"saxx2X"sv},  // fails tail of "sa"sv
	    {"sampxJA1"sv},  // fails leaf of "samp"sv
	    {"scmiIy"sv},  // fails tail of "sc"sv
	    {"scriptjGg7"sv},  // fails leaf of "script"sv
	    {"seEcg9"sv},  // fails tail of "se"sv
	    {"secucLL"sv},  // fails tail of "sec"sv
	    {"sectionv2g6"sv},  // fails leaf of "section"sv
	    {"seluyMR"sv},  // fails tail of "sel"sv
	    {"selectuUnT"sv},  // fails leaf of "select"sv
	    {"smw2R4"sv},  // fails tail of "sm"sv
	    {"small0Whd"sv},  // fails leaf of "small"sv
	    {"soFbQq"sv},  // fails tail of "so"sv
	    {"sourcef5Ld"sv},  // fails leaf of "source"sv
	    {"spQp5M"sv},  // fails tail of "sp"sv
	    {"spanz0ok"sv},  // fails leaf of "span"sv
	    {"stROBt"sv},  // fails tail of "st"sv
	    {"strMSic"sv},  // fails tail of "str"sv
	    {"strongfzYi"sv},  // fails leaf of "strong"sv
	    {"sty4CXB"sv},  // fails tail of "sty"sv
	    {"style4TG8"sv},  // fails leaf of "style"sv
	    {"suHD70"sv},  // fails tail of "su"sv
	    {"sub9CQG"sv},  // fails leaf of "sub"sv
	    {"sumdxF4"sv},  // fails tail of "sum"sv
	    {"summarytzmi"sv},  // fails leaf of "summary"sv
	    {"supGCss"sv},  // fails leaf of "sup"sv
	    {"svUFBm"sv},  // fails tail of "sv"sv
	    {"svgxo8f"sv},  // fails leaf of "svg"sv
	    {"t0336"sv},  // fails tail of "t"sv
	    {"tahr0g"sv},  // fails tail of "ta"sv
	    {"tableT1Hu"sv},  // fails leaf of "table"sv
	    {"tbDP47"sv},  // fails tail of "tb"sv
	    {"tbodyVc97"sv},  // fails leaf of "tbody"sv
	    {"tdIZMD"sv},  // fails leaf of "td"sv
	    {"teIeHB"sv},  // fails tail of "te"sv
	    {"temTOKH"sv},  // fails tail of "tem"sv
	    {"templateQRyW"sv},  // fails leaf of "template"sv
	    {"texjHuS"sv},  // fails tail of "tex"sv
	    {"textareaswhP"sv},  // fails leaf of "textarea"sv
	    {"tfWsRa"sv},  // fails tail of "tf"sv
	    {"tfootCL8q"sv},  // fails leaf of "tfoot"sv
	    {"thvCSl"sv},  // fails leaf and tail of "th"sv
	    {"theadYkwS"sv},  // fails leaf of "thead"sv
	    {"ti2Jex"sv},  // fails tail of "ti"sv
	    {"timbCzN"sv},  // fails tail of "tim"sv
	    {"timelFno"sv},  // fails leaf of "time"sv
	    {"titYGy1"sv},  // fails tail of "tit"sv
	    {"titlemzoV"sv},  // fails leaf of "title"sv
	    {"trobTZ"sv},  // fails leaf and tail of "tr"sv
	    {"trackNnf7"sv},  // fails leaf of "track"sv
	    {"unUrd"sv},  // fails leaf and tail of "u"sv
	    {"ulnf1e"sv},  // fails leaf of "ul"sv
	    {"vpZVl"sv},  // fails tail of "v"sv
	    {"vacalS"sv},  // fails tail of "va"sv
	    {"varLVto"sv},  // fails leaf of "var"sv
	    {"viSqoz"sv},  // fails tail of "vi"sv
	    {"videoKQ1Q"sv},  // fails leaf of "video"sv
	    {"wlY7j"sv},  // fails tail of "w"sv
	    {"wbrQbjz"sv},  // fails leaf of "wbr"sv
	};

	INSTANTIATE_TEST_SUITE_P(names,
	                         html_element,
	                         ::testing::ValuesIn(names));
	INSTANTIATE_TEST_SUITE_P(errors,
	                         html_element,
	                         ::testing::ValuesIn(nosuch));
}  // namespace tangle::browser::testing
// clang-format on
