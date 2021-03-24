// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-dtd
// build/bin/html-dtd tools/lookup/html.dtd browser

#include <tangle/browser/html_dtd.hpp>
#include <cctype>
#include <string>

namespace tangle::browser {
static element_info const& get_info(unsigned index) {
	static element_info const infos[] = {
		{
			// 0: a
			elem_flag::any | elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{},
			{elem_flag::interactive},
		},
		{
			// 1: abbr, b, bdi, bdo, cite, code, data, em, i, kbd, mark, output,
			// q, s, samp, small, span, strong, sub, sup, time, u, var
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing},
			{elem_flag::flow_only},
		},
		{
			// 2: address
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow},
			{elem_flag::sectioning | elem_flag::heading, {"address", "footer", "header"}},
		},
		{
			// 3: area, br, img, keygen, wbr
			elem_flag::empty | elem_flag::flow | elem_flag::phrasing,
		},
		{
			// 4: article, aside, nav
			elem_flag::flow | elem_flag::flow_only | elem_flag::sectioning,
			{elem_flag::flow},
			{elem_flag::none, {"main"}},
		},
		{
			// 5: audio, video
			elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{elem_flag::flow, {"source", "track"}},
			{elem_flag::none, {"media"}},
		},
		{
			// 6: base, meta
			elem_flag::empty | elem_flag::metadata,
		},
		{
			// 7: blockquote, dialog, main
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow},
		},
		{
			// 8: body, dd, figcaption, li, td
			elem_flag::none,
			{elem_flag::flow},
		},
		{
			// 9: button
			elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{elem_flag::phrasing},
			{elem_flag::interactive},
		},
		{
			// 10: canvas, del, ins, math, object, svg
			elem_flag::any | elem_flag::flow | elem_flag::phrasing,
		},
		{
			// 11: caption
			elem_flag::none,
			{elem_flag::flow},
			{elem_flag::none, {"table"}},
		},
		{
			// 12: col, menuitem, param, source, track
			elem_flag::empty,
		},
		{
			// 13: colgroup
			elem_flag::none,
			{elem_flag::none, {"col", "template"}},
		},
		{
			// 14: datalist
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing, {"option", "script"}},
			{elem_flag::flow_only},
		},
		{
			// 15: details
			elem_flag::flow | elem_flag::flow_only | elem_flag::interactive,
			{elem_flag::flow, {"summary"}},
		},
		{
			// 16: dfn
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing},
			{elem_flag::flow_only, {"dfn"}},
		},
		{
			// 17: div
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow, {"dd", "dt"}},
			{elem_flag::none, {"main"}},
		},
		{
			// 18: dl
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::scripting, {"dd", "div", "dt"}},
		},
		{
			// 19: dt, th
			elem_flag::none,
			{elem_flag::flow},
			{elem_flag::sectioning | elem_flag::heading, {"footer", "header"}},
		},
		{
			// 20: embed, input
			elem_flag::empty | elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
		},
		{
			// 21: fieldset
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow, {"legend"}},
		},
		{
			// 22: figure
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow, {"figcaption"}},
		},
		{
			// 23: footer, header
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow},
			{elem_flag::none, {"footer", "header", "main"}},
		},
		{
			// 24: form
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::flow},
			{elem_flag::none, {"form"}},
		},
		{
			// 25: h1, h2, h3, h4, h5, h6
			elem_flag::flow | elem_flag::flow_only | elem_flag::heading,
			{elem_flag::phrasing},
			{elem_flag::flow_only},
		},
		{
			// 26: head
			elem_flag::none,
			{elem_flag::metadata},
		},
		{
			// 27: hr
			elem_flag::empty | elem_flag::flow | elem_flag::flow_only,
		},
		{
			// 28: html
			elem_flag::none,
			{elem_flag::none, {"body", "head", "script"}},
		},
		{
			// 29: iframe
			elem_flag::any | elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{},
			{elem_flag::none, {"body", "head"}},
		},
		{
			// 30: label
			elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{elem_flag::phrasing},
			{elem_flag::flow_only, {"label"}},
		},
		{
			// 31: legend
			elem_flag::none,
			{elem_flag::phrasing, {"h1", "h2", "h3", "h5", "h6"}},
			{elem_flag::none, {"main"}},
		},
		{
			// 32: link
			elem_flag::empty | elem_flag::metadata | elem_flag::flow | elem_flag::phrasing,
		},
		{
			// 33: map
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::flow},
		},
		{
			// 34: menu
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::scripting, {"hr", "menu", "menuitem"}},
		},
		{
			// 35: meter
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing},
			{elem_flag::flow_only, {"meter"}},
		},
		{
			// 36: noscript
			elem_flag::any | elem_flag::metadata | elem_flag::flow | elem_flag::phrasing,
		},
		{
			// 37: ol, ul
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::scripting, {"li"}},
		},
		{
			// 38: optgroup
			elem_flag::none,
			{elem_flag::scripting, {"option"}},
		},
		{
			// 39: option
			elem_flag::text_only,
		},
		{
			// 40: p
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::phrasing},
			{elem_flag::flow_only, {"figcaption"}},
		},
		{
			// 41: picture
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::scripting, {"img", "source"}},
		},
		{
			// 42: pre
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::phrasing},
			{elem_flag::flow_only},
		},
		{
			// 43: progress
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing},
			{elem_flag::flow_only, {"progress"}},
		},
		{
			// 44: rb, rp, rt
			elem_flag::none,
			{elem_flag::phrasing},
			{elem_flag::flow_only},
		},
		{
			// 45: rtc
			elem_flag::none,
			{elem_flag::phrasing, {"rp", "rt"}},
			{elem_flag::flow_only},
		},
		{
			// 46: ruby
			elem_flag::flow | elem_flag::phrasing,
			{elem_flag::phrasing, {"rb", "rp", "rt", "rtc"}},
			{elem_flag::flow_only},
		},
		{
			// 47: script
			elem_flag::text_only | elem_flag::metadata | elem_flag::scripting,
		},
		{
			// 48: section
			elem_flag::flow | elem_flag::flow_only | elem_flag::sectioning,
			{elem_flag::flow},
		},
		{
			// 49: select
			elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
			{elem_flag::scripting, {"optgroup", "option"}},
		},
		{
			// 50: style
			elem_flag::text_only | elem_flag::metadata | elem_flag::flow | elem_flag::flow_only,
		},
		{
			// 51: summary
			elem_flag::none,
			{elem_flag::heading | elem_flag::phrasing},
			{elem_flag::flow_only},
		},
		{
			// 52: table
			elem_flag::flow | elem_flag::flow_only,
			{elem_flag::scripting, {"caption", "colgroup", "tbody", "tfoot", "thead", "tr"}},
		},
		{
			// 53: tbody, tfoot, thead
			elem_flag::none,
			{elem_flag::scripting, {"tr"}},
		},
		{
			// 54: template
			elem_flag::any | elem_flag::metadata | elem_flag::flow | elem_flag::phrasing | elem_flag::scripting,
		},
		{
			// 55: textarea
			elem_flag::text_only | elem_flag::flow | elem_flag::phrasing | elem_flag::interactive,
		},
		{
			// 56: title
			elem_flag::text_only | elem_flag::metadata,
		},
		{
			// 57: tr
			elem_flag::none,
			{elem_flag::scripting, {"td", "th"}},
		},
		{
			elem_flag::any | elem_flag::flow | elem_flag::phrasing
		},
	};

	return infos[index];
}

#define CHAR_AT(NO) if (length > NO) { switch (std::tolower(static_cast<unsigned char>(name[NO]))) {
#define CHAR(C) case C:
#define CHAR_END_NOBR() }}
#define CHAR_END() }} break;
#define ON_SUFFIX(NO, suffix, result) \
  if (equal_ignore_case(name.substr(NO), suffix ## sv)) { return result; } \
  break
#define ON_LEAF_NOBR(NO, result) if (length == NO) return result
#define ON_LEAF(NO, result) ON_LEAF_NOBR(NO, result); break

unsigned element_index(std::string_view name) {
	auto const length = name.length();

	CHAR_AT(0)
		CHAR('a')
			ON_LEAF_NOBR(1, 0);  // a
			CHAR_AT(1)
				CHAR('b') ON_SUFFIX(2, "br", 1);  // abbr
				CHAR('d') ON_SUFFIX(2, "dress", 2);  // address
				CHAR('r')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "a", 3);  // area
						CHAR('t') ON_SUFFIX(3, "icle", 4);  // article
					CHAR_END()
				CHAR('s') ON_SUFFIX(2, "ide", 4);  // aside
				CHAR('u') ON_SUFFIX(2, "dio", 5);  // audio
			CHAR_END()
		CHAR('b')
			ON_LEAF_NOBR(1, 1);  // b
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "se", 6);  // base
				CHAR('d')
					CHAR_AT(2)
						CHAR('i') ON_LEAF(3, 1);  // bdi
						CHAR('o') ON_LEAF(3, 1);  // bdo
					CHAR_END()
				CHAR('l') ON_SUFFIX(2, "ockquote", 7);  // blockquote
				CHAR('o') ON_SUFFIX(2, "dy", 8);  // body
				CHAR('r') ON_LEAF(2, 3);  // br
				CHAR('u') ON_SUFFIX(2, "tton", 9);  // button
			CHAR_END()
		CHAR('c')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('n') ON_SUFFIX(3, "vas", 10);  // canvas
						CHAR('p') ON_SUFFIX(3, "tion", 11);  // caption
					CHAR_END()
				CHAR('i') ON_SUFFIX(2, "te", 1);  // cite
				CHAR('o')
					CHAR_AT(2)
						CHAR('d') ON_SUFFIX(3, "e", 1);  // code
						CHAR('l')
							ON_LEAF_NOBR(3, 12);  // col
							ON_SUFFIX(3, "group", 13);  // colgroup
					CHAR_END()
			CHAR_END()
		CHAR('d')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('t')
							CHAR_AT(3)
								CHAR('a')
									ON_LEAF_NOBR(4, 1);  // data
									ON_SUFFIX(4, "list", 14);  // datalist
							CHAR_END()
					CHAR_END()
				CHAR('d') ON_LEAF(2, 8);  // dd
				CHAR('e')
					CHAR_AT(2)
						CHAR('l') ON_LEAF(3, 10);  // del
						CHAR('t') ON_SUFFIX(3, "ails", 15);  // details
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "n", 16);  // dfn
				CHAR('i')
					CHAR_AT(2)
						CHAR('a') ON_SUFFIX(3, "log", 7);  // dialog
						CHAR('v') ON_LEAF(3, 17);  // div
					CHAR_END()
				CHAR('l') ON_LEAF(2, 18);  // dl
				CHAR('t') ON_LEAF(2, 19);  // dt
			CHAR_END()
		CHAR('e')
			CHAR_AT(1)
				CHAR('m')
					ON_LEAF_NOBR(2, 1);  // em
					ON_SUFFIX(2, "bed", 20);  // embed
			CHAR_END()
		CHAR('f')
			CHAR_AT(1)
				CHAR('i')
					CHAR_AT(2)
						CHAR('e') ON_SUFFIX(3, "ldset", 21);  // fieldset
						CHAR('g')
							CHAR_AT(3)
								CHAR('c') ON_SUFFIX(4, "aption", 8);  // figcaption
								CHAR('u') ON_SUFFIX(4, "re", 22);  // figure
							CHAR_END()
					CHAR_END()
				CHAR('o')
					CHAR_AT(2)
						CHAR('o') ON_SUFFIX(3, "ter", 23);  // footer
						CHAR('r') ON_SUFFIX(3, "m", 24);  // form
					CHAR_END()
			CHAR_END()
		CHAR('h')
			CHAR_AT(1)
				CHAR('1') ON_LEAF(2, 25);  // h1
				CHAR('2') ON_LEAF(2, 25);  // h2
				CHAR('3') ON_LEAF(2, 25);  // h3
				CHAR('4') ON_LEAF(2, 25);  // h4
				CHAR('5') ON_LEAF(2, 25);  // h5
				CHAR('6') ON_LEAF(2, 25);  // h6
				CHAR('e')
					CHAR_AT(2)
						CHAR('a')
							CHAR_AT(3)
								CHAR('d')
									ON_LEAF_NOBR(4, 26);  // head
									ON_SUFFIX(4, "er", 23);  // header
							CHAR_END()
					CHAR_END()
				CHAR('r') ON_LEAF(2, 27);  // hr
				CHAR('t') ON_SUFFIX(2, "ml", 28);  // html
			CHAR_END()
		CHAR('i')
			ON_LEAF_NOBR(1, 1);  // i
			CHAR_AT(1)
				CHAR('f') ON_SUFFIX(2, "rame", 29);  // iframe
				CHAR('m') ON_SUFFIX(2, "g", 3);  // img
				CHAR('n')
					CHAR_AT(2)
						CHAR('p') ON_SUFFIX(3, "ut", 20);  // input
						CHAR('s') ON_LEAF(3, 10);  // ins
					CHAR_END()
			CHAR_END()
		CHAR('k')
			CHAR_AT(1)
				CHAR('b') ON_SUFFIX(2, "d", 1);  // kbd
				CHAR('e') ON_SUFFIX(2, "ygen", 3);  // keygen
			CHAR_END()
		CHAR('l')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "bel", 30);  // label
				CHAR('e') ON_SUFFIX(2, "gend", 31);  // legend
				CHAR('i')
					ON_LEAF_NOBR(2, 8);  // li
					ON_SUFFIX(2, "nk", 32);  // link
			CHAR_END()
		CHAR('m')
			CHAR_AT(1)
				CHAR('a')
					CHAR_AT(2)
						CHAR('i') ON_SUFFIX(3, "n", 7);  // main
						CHAR('p') ON_LEAF(3, 33);  // map
						CHAR('r') ON_SUFFIX(3, "k", 1);  // mark
						CHAR('t') ON_SUFFIX(3, "h", 10);  // math
					CHAR_END()
				CHAR('e')
					CHAR_AT(2)
						CHAR('n')
							CHAR_AT(3)
								CHAR('u')
									ON_LEAF_NOBR(4, 34);  // menu
									ON_SUFFIX(4, "item", 12);  // menuitem
							CHAR_END()
						CHAR('t')
							CHAR_AT(3)
								CHAR('a') ON_LEAF(4, 6);  // meta
								CHAR('e') ON_SUFFIX(4, "r", 35);  // meter
							CHAR_END()
					CHAR_END()
			CHAR_END()
		CHAR('n')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "v", 4);  // nav
				CHAR('o') ON_SUFFIX(2, "script", 36);  // noscript
			CHAR_END()
		CHAR('o')
			CHAR_AT(1)
				CHAR('b') ON_SUFFIX(2, "ject", 10);  // object
				CHAR('l') ON_LEAF(2, 37);  // ol
				CHAR('p')
					CHAR_AT(2)
						CHAR('t')
							CHAR_AT(3)
								CHAR('g') ON_SUFFIX(4, "roup", 38);  // optgroup
								CHAR('i') ON_SUFFIX(4, "on", 39);  // option
							CHAR_END()
					CHAR_END()
				CHAR('u') ON_SUFFIX(2, "tput", 1);  // output
			CHAR_END()
		CHAR('p')
			ON_LEAF_NOBR(1, 40);  // p
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "ram", 12);  // param
				CHAR('i') ON_SUFFIX(2, "cture", 41);  // picture
				CHAR('r')
					CHAR_AT(2)
						CHAR('e') ON_LEAF(3, 42);  // pre
						CHAR('o') ON_SUFFIX(3, "gress", 43);  // progress
					CHAR_END()
			CHAR_END()
		CHAR('q') ON_LEAF(1, 1);  // q
		CHAR('r')
			CHAR_AT(1)
				CHAR('b') ON_LEAF(2, 44);  // rb
				CHAR('p') ON_LEAF(2, 44);  // rp
				CHAR('t')
					ON_LEAF_NOBR(2, 44);  // rt
					ON_SUFFIX(2, "c", 45);  // rtc
				CHAR('u') ON_SUFFIX(2, "by", 46);  // ruby
			CHAR_END()
		CHAR('s')
			ON_LEAF_NOBR(1, 1);  // s
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "mp", 1);  // samp
				CHAR('c') ON_SUFFIX(2, "ript", 47);  // script
				CHAR('e')
					CHAR_AT(2)
						CHAR('c') ON_SUFFIX(3, "tion", 48);  // section
						CHAR('l') ON_SUFFIX(3, "ect", 49);  // select
					CHAR_END()
				CHAR('m') ON_SUFFIX(2, "all", 1);  // small
				CHAR('o') ON_SUFFIX(2, "urce", 12);  // source
				CHAR('p') ON_SUFFIX(2, "an", 1);  // span
				CHAR('t')
					CHAR_AT(2)
						CHAR('r') ON_SUFFIX(3, "ong", 1);  // strong
						CHAR('y') ON_SUFFIX(3, "le", 50);  // style
					CHAR_END()
				CHAR('u')
					CHAR_AT(2)
						CHAR('b') ON_LEAF(3, 1);  // sub
						CHAR('m') ON_SUFFIX(3, "mary", 51);  // summary
						CHAR('p') ON_LEAF(3, 1);  // sup
					CHAR_END()
				CHAR('v') ON_SUFFIX(2, "g", 10);  // svg
			CHAR_END()
		CHAR('t')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "ble", 52);  // table
				CHAR('b') ON_SUFFIX(2, "ody", 53);  // tbody
				CHAR('d') ON_LEAF(2, 8);  // td
				CHAR('e')
					CHAR_AT(2)
						CHAR('m') ON_SUFFIX(3, "plate", 54);  // template
						CHAR('x') ON_SUFFIX(3, "tarea", 55);  // textarea
					CHAR_END()
				CHAR('f') ON_SUFFIX(2, "oot", 53);  // tfoot
				CHAR('h')
					ON_LEAF_NOBR(2, 19);  // th
					ON_SUFFIX(2, "ead", 53);  // thead
				CHAR('i')
					CHAR_AT(2)
						CHAR('m') ON_SUFFIX(3, "e", 1);  // time
						CHAR('t') ON_SUFFIX(3, "le", 56);  // title
					CHAR_END()
				CHAR('r')
					ON_LEAF_NOBR(2, 57);  // tr
					ON_SUFFIX(2, "ack", 12);  // track
			CHAR_END()
		CHAR('u')
			ON_LEAF_NOBR(1, 1);  // u
			ON_SUFFIX(1, "l", 37);  // ul
		CHAR('v')
			CHAR_AT(1)
				CHAR('a') ON_SUFFIX(2, "r", 1);  // var
				CHAR('i') ON_SUFFIX(2, "deo", 5);  // video
			CHAR_END()
		CHAR('w') ON_SUFFIX(1, "br", 3);  // wbr
	CHAR_END_NOBR()

	return 58;
}

#undef CHAR_AT
#undef CHAR
#undef CHAR_END_NOBR
#undef CHAR_END
#undef ON_SUFFIX
#undef ON_LEAF_NOBR
#undef ON_LEAF

element_info const& get_info(std::string_view name) noexcept {
	return get_info(element_index(name));
}

} // namespace tangle::browser
// clang-format on
