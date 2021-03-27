// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "paths.hh"
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::testing {
	struct paths_test_data {
		std::string_view input;
		std::vector<matcher> parsed;
		std::string_view singleline;
		std::string_view multiline;

		friend std::ostream& operator<<(std::ostream& out,
		                                paths_test_data const& test) {
			return out << cxx_string{test.input, 160};
		}
	};

	struct paths_test : ::testing::TestWithParam<paths_test_data> {
	protected:
		void assert_eq(std::vector<matcher> const& expected,
		               std::vector<matcher> const& actual,
		               std::string const& prefix) {
			ASSERT_EQ(expected.size(), actual.size())
			    << "Path: " << prefix << "\nActual: " << paths_print{actual};
			auto it = expected.begin();
			for (auto& actual_sub : actual) {
				auto& expected_sub = *it++;
				ASSERT_EQ(expected_sub.index(), actual_sub.index())
				    << "Path: " << prefix
				    << "\nActual: " << paths_print{actual};

				auto* actual_step_p = std::get_if<step>(&actual_sub);
				auto* actual_action = std::get_if<selector_action>(&actual_sub);

				if (actual_step_p) {
					auto& expected_step = std::get<step>(expected_sub);
					std::ostringstream exp_stream;
					exp_stream << expected_step.current;
					std::ostringstream actual_stream;
					actual_stream << actual_step_p->current;
					ASSERT_EQ(exp_stream.str(), actual_stream.str())
					    << "Path: " << prefix
					    << "\nActual: " << paths_print{actual};

					assert_eq(expected_step.submatchers,
					          actual_step_p->submatchers,
					          prefix + "/" + actual_stream.str());
					if (this->HasFatalFailure()) return;
					continue;
				}

				auto& expected_action = std::get<selector_action>(expected_sub);
				std::ostringstream exp_stream;
				exp_stream << expected_action;
				std::ostringstream actual_stream;
				actual_stream << *actual_action;
				ASSERT_EQ(exp_stream.str(), actual_stream.str())
				    << "Path: " << prefix
				    << "\nActual: " << paths_print{actual};
			}
		}
	};

	TEST_P(paths_test, parse) {
		auto& param = GetParam();
		auto actual = paths::parse_paths(param.input);
		assert_eq(param.parsed, actual.tracks, "test:");
	}

	TEST_P(paths_test, print) {
		auto& param = GetParam();
		std::ostringstream oss;
		oss << paths_print{param.parsed};
		ASSERT_EQ(param.singleline.data(), oss.str());
	}

	TEST_P(paths_test, print_multi) {
		auto& param = GetParam();
		std::ostringstream oss;
		oss << paths_print{param.parsed, true};
		ASSERT_EQ(param.multiline.data(), oss.str());
	}

	inline matcher sel(std::string_view value) {
		return {selector::parse(value), {}};
	}

	inline matcher sel(std::string_view value,
	                   std::initializer_list<matcher> const& subs) {
		return {selector::parse(value), subs};
	}

	inline matcher action(std::string value) {
		return {selector_action::parse(value)};
	}

	static paths_test_data const data[] = {
	    {{}, {}, "{}"sv, "{}"sv},
	    {
	        "{ !action[] }"sv,
	        {action("!action"s)},
	        "{ !action }"sv,
	        "{ !action }"sv,
	    },
	    {
	        "{ .class { div { !action[arg] } } }"sv,
	        {
	            sel(".class", {sel("div", {action("!action[arg]")})}),
	        },
	        "{ .class { div { !action[arg] } } }"sv,
	        "{ .class { div { !action[arg] } } }"sv,
	    },
	    {
	        "{"
	        "    .class{"
	        "        div{!action[arg]#id{h1{!another-action}}}"
	        "        [lang=fr]{!text}"
	        "    }"
	        "    .other.one{!action}"
	        "}"sv,
	        {
	            sel(".class",
	                {
	                    sel("div",
	                        {
	                            action("!action[arg]"),
	                            sel("#id",
	                                {sel("h1", {action("!another-action")})}),
	                        }),
	                    sel("[lang=fr]", {action("!text")}),
	                }),
	            sel(".other.one", {{action("!action")}}),
	        },
	        "{ .class { div { !action[arg] #id { h1 { !another-action } } } [lang=fr] { !text } } .other.one { !action } }"sv,
	        R"({
    .class {
        div {
            !action[arg]
            #id { h1 { !another-action } }
        }
        [lang=fr] { !text }
    }
    .other.one { !action }
})"sv,
	    },
	};

	INSTANTIATE_TEST_SUITE_P(items, paths_test, ::testing::ValuesIn(data));

	struct paths_visit_test : ::testing::TestWithParam<visit_data> {};

	TEST_P(paths_visit_test, walk) {
		auto& param = GetParam();
		root_cursor cur{param.hdr.markup};
		auto defs = paths::parse_paths(param.hdr.definition);

		std::vector<std::string> actual{};
		auto callbacks = setup(&actual);
		ASSERT_EQ(param.hdr.result_during_walk,
		          defs.visit(cur.root(), callbacks));

		ASSERT_EQ(param.expected.size(), actual.size());

		auto it = actual.begin();
		for (auto& exp : param.expected) {
			std::string xpctd{};
			xpctd.assign(exp);
			auto& act = *it++;
			ASSERT_EQ(xpctd, act);
		}
	}

	TEST_P(paths_visit_test, debug) {
		auto& param = GetParam();
		root_cursor cur{param.hdr.markup};
		auto defs = paths::parse_paths(param.hdr.definition);

		std::vector<std::string> _{};
		auto callbacks = setup(&_);

		std::ostringstream actual;
		ASSERT_TRUE(defs.visit(cur.root(), callbacks, nullptr, &actual));

		std::string debug_output{};
		debug_output.assign(param.debug_output);
		ASSERT_EQ(debug_output, actual.str());
	}

	TEST_P(paths_visit_test, exception) {
		auto& param = GetParam();
		root_cursor cur{param.hdr.markup};

		auto defs = paths::parse_paths(param.hdr.definition);
		ASSERT_THROW(defs.visit(cur.root(), {}), std::runtime_error);
	}

	TEST_P(paths_visit_test, return_false) {
		auto& param = GetParam();
		root_cursor cur{param.hdr.markup};

		auto defs = paths::parse_paths(param.hdr.definition);

		always_false purecall;
		ASSERT_FALSE(defs.visit(cur.root(), {}, &purecall));
	}

	static constexpr auto html = R"html(<html>
<head>
    <title>Title of the page</title>
</head>
<body>
<div class="content"></div>
<div class="content">
<p> <i> Lorem ipsum dolor sit amet, consectetur adipiscing elit.</i> Nulla ultricies
nunc rhoncus gravida blandit. In non erat cursus, feugiat elit id, facilisis
nibh. Nulla vehicula neque sit amet urna ornare, vitae aliquet arcu pulvinar.
In hac habitasse platea dictumst. Donec tincidunt, mauris in viverra posuere,
sem nisi bibendum eros, vel rutrum justo justo accumsan mauris. Integer
lobortis vitae tortor sit amet dignissim. Cras rutrum lectus ac consequat
dapibus. Phasellus hendrerit magna orci. Vivamus gravida a turpis sed commodo.
Nullam posuere ut mauris vitae eleifend. Curabitur in ornare orci. Nulla
consectetur commodo tortor sit amet cursus.</p>

<p>Quisque nec sagittis dui, in dapibus metus. Nam varius pulvinar felis, nec
rhoncus orci tincidunt id. Ut luctus, nisl scelerisque imperdiet dictum, turpis
justo rhoncus dui, non tempor odio est vel nibh. Interdum et malesuada fames ac
ante ipsum primis in faucibus. Nullam sed congue risus. Sed molestie magna in
ante faucibus, eget porta ante ultricies. Aenean efficitur finibus tortor.
Nulla eu facilisis nulla. Integer quis lorem quam.</p>

</div>
<div class="content">

<p>
Integer facilisis diam at augue convallis congue vitae eu elit. Vestibulum
ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae;
Quisque placerat arcu vitae tempor venenatis. Integer commodo arcu a tellus
aliquet euismod. Donec eu finibus sapien. Vivamus ac nisi ac mi dignissim
consectetur in vestibulum neque. Quisque non justo nunc. Nullam ut facilisis
diam. Vivamus eget facilisis odio. Donec at urna quis neque eleifend blandit
finibus a eros. Phasellus non viverra orci. Nam rutrum ornare nulla, et congue
purus posuere in. Suspendisse ac nisi justo. Aliquam nec nisi in urna sagittis
mollis. Vestibulum id convallis enim. Maecenas lacinia purus non massa
porttitor, at accumsan urna sagittis.
</p>

</div>

<div>Sed consequat viverra sem id porta. Sed nibh felis, eleifend et mattis
vitae, pellentesque nec odio. Duis dictum efficitur felis ut dictum. Mauris
sodales, elit at posuere convallis, massa tellus consectetur turpis, non
aliquet dui magna sed quam. Vivamus lacinia nunc non semper euismod. Maecenas
consequat ornare elit, at aliquam turpis porta sit amet. Curabitur urna elit,
mattis in accumsan vitae, consectetur id risus. Nulla nec faucibus nisl, vel
accumsan lectus. Etiam non magna non ante interdum tempor.</div>

<div class="content for-mark">
    <div>First</div>
    <div id=mark>Second</div>
    Text between.
    <div>
        Third
        <div><p>Fourth</p></div>
    </div>
</div>

<img src='image1.png' />
<img id='poster' src='image2.png' />
<img src='image3.png' />
<img src='image4.png' />

<ul class="menu">
<li class="link"><a href="#">First</a></li>
<li class="link"><a href=" example.com ">Second</a></li>
<li class="link"><a href="sub.html  ">Third</a></li>
<li class="link"><a href="app.example.com">Fourth</a></li>
<li class="link"><a>Fifth</a></li>
</ul>

<ul class="menu">
<li class="link">First</li>
<li class="link">Second</li>
<li class="link">Third</li>
<li class="link">Fourth</li>
<li class="link">Fifth</li>
</ul>

<p id=endless-span>
This text has a <span>span, which has <b>no</b> end tag
</p>

<p id=mixed-phrasing>
Normal text, <b>bold <i>and italic <u>and underline</b>, just italic</u>, just underline
</p>

<div><p id=mixed-phrasing-ii>
Normal text, <b>bold <i>and italic <u>and underline</b>, just italic</u>, just underline
</div>

<p id=mixed-phrasing-iii>
Normal text, <b>bold <i>and italic <u>and underline</b>, just italic</u>, just underline

<p>Should auto-close #mixed-phrasing-iii</p>

</body>
</html>)html"sv;

	static visit_data const lookup[] = {
	    {
	        {html, "{head{title{!innerText}}}"},
	        {"Title of the page"},
	        "!!! head\n"
	        "??? head\n"
	        ">>> head -> head\n"
	        "    !!! title\n"
	        "    ??? title\n"
	        "    >>> title -> title\n",
	    },
	    {
	        {html, "{title{!innerText}}"},
	        {"Title of the page"},
	        "!!! title\n"
	        "??? title\n"
	        ">>> title -> title\n",
	    },
	    {
	        {html, "{.content{!innerHTML}}"},
	        {
	            {},

	            "\n<p> <i> Lorem ipsum dolor sit amet, consectetur adipiscing "
	            "elit.</i> Nulla ultricies\nnunc rhoncus gravida blandit. In "
	            "non erat cursus, feugiat elit id, facilisis\nnibh. Nulla "
	            "vehicula neque sit amet urna ornare, vitae aliquet arcu "
	            "pulvinar.\nIn hac habitasse platea dictumst. Donec tincidunt, "
	            "mauris in viverra posuere,\nsem nisi bibendum eros, vel "
	            "rutrum justo justo accumsan mauris. Integer\nlobortis vitae "
	            "tortor sit amet dignissim. Cras rutrum lectus ac "
	            "consequat\ndapibus. Phasellus hendrerit magna orci. Vivamus "
	            "gravida a turpis sed commodo.\nNullam posuere ut mauris vitae "
	            "eleifend. Curabitur in ornare orci. Nulla\nconsectetur "
	            "commodo tortor sit amet cursus.</p>\n\n<p>Quisque nec "
	            "sagittis dui, in dapibus metus. Nam varius pulvinar felis, "
	            "nec\nrhoncus orci tincidunt id. Ut luctus, nisl scelerisque "
	            "imperdiet dictum, turpis\njusto rhoncus dui, non tempor odio "
	            "est vel nibh. Interdum et malesuada fames ac\nante ipsum "
	            "primis in faucibus. Nullam sed congue risus. Sed molestie "
	            "magna in\nante faucibus, eget porta ante ultricies. Aenean "
	            "efficitur finibus tortor.\nNulla eu facilisis nulla. Integer "
	            "quis lorem quam.</p>\n\n",

	            "\n\n<p>\nInteger facilisis diam at augue convallis congue "
	            "vitae eu elit. Vestibulum\nante ipsum primis in faucibus orci "
	            "luctus et ultrices posuere cubilia curae;\nQuisque placerat "
	            "arcu vitae tempor venenatis. Integer commodo arcu a "
	            "tellus\naliquet euismod. Donec eu finibus sapien. Vivamus ac "
	            "nisi ac mi dignissim\nconsectetur in vestibulum neque. "
	            "Quisque non justo nunc. Nullam ut facilisis\ndiam. Vivamus "
	            "eget facilisis odio. Donec at urna quis neque eleifend "
	            "blandit\nfinibus a eros. Phasellus non viverra orci. Nam "
	            "rutrum ornare nulla, et congue\npurus posuere in. Suspendisse "
	            "ac nisi justo. Aliquam nec nisi in urna sagittis\nmollis. "
	            "Vestibulum id convallis enim. Maecenas lacinia purus non "
	            "massa\nporttitor, at accumsan urna sagittis.\n</p>\n\n",

	            "\n    <div>First</div>\n    <div id=mark>Second</div>\n    "
	            "Text between.\n    <div>\n        Third\n        "
	            "<div><p>Fourth</p></div>\n    </div>\n",
	        },
	        "!!! .content\n"
	        ">>> .content -> div.content\n"
	        ">>> .content -> div.content\n"
	        ">>> .content -> div.content\n"
	        ">>> .content -> div.content.for-mark\n",
	    },
	    {
	        {html, "{.content{p{!innerText}}}"},
	        {
	            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
	            "Nulla ultricies nunc rhoncus gravida blandit. In non erat "
	            "cursus, feugiat elit id, facilisis nibh. Nulla vehicula neque "
	            "sit amet urna ornare, vitae aliquet arcu pulvinar. In hac "
	            "habitasse platea dictumst. Donec tincidunt, mauris in viverra "
	            "posuere, sem nisi bibendum eros, vel rutrum justo justo "
	            "accumsan mauris. Integer lobortis vitae tortor sit amet "
	            "dignissim. Cras rutrum lectus ac consequat dapibus. Phasellus "
	            "hendrerit magna orci. Vivamus gravida a turpis sed commodo. "
	            "Nullam posuere ut mauris vitae eleifend. Curabitur in ornare "
	            "orci. Nulla consectetur commodo tortor sit amet cursus.",

	            "Quisque nec sagittis dui, in dapibus metus. Nam varius "
	            "pulvinar felis, nec rhoncus orci tincidunt id. Ut luctus, "
	            "nisl scelerisque imperdiet dictum, turpis justo rhoncus dui, "
	            "non tempor odio est vel nibh. Interdum et malesuada fames ac "
	            "ante ipsum primis in faucibus. Nullam sed congue risus. Sed "
	            "molestie magna in ante faucibus, eget porta ante ultricies. "
	            "Aenean efficitur finibus tortor. Nulla eu facilisis nulla. "
	            "Integer quis lorem quam.",

	            "Integer facilisis diam at augue convallis congue vitae eu "
	            "elit. Vestibulum ante ipsum primis in faucibus orci luctus et "
	            "ultrices posuere cubilia curae; Quisque placerat arcu vitae "
	            "tempor venenatis. Integer commodo arcu a tellus aliquet "
	            "euismod. Donec eu finibus sapien. Vivamus ac nisi ac mi "
	            "dignissim consectetur in vestibulum neque. Quisque non justo "
	            "nunc. Nullam ut facilisis diam. Vivamus eget facilisis odio. "
	            "Donec at urna quis neque eleifend blandit finibus a eros. "
	            "Phasellus non viverra orci. Nam rutrum ornare nulla, et "
	            "congue purus posuere in. Suspendisse ac nisi justo. Aliquam "
	            "nec nisi in urna sagittis mollis. Vestibulum id convallis "
	            "enim. Maecenas lacinia purus non massa porttitor, at accumsan "
	            "urna sagittis.",

	            "Fourth",
	        },
	        "!!! .content\n"
	        ">>> .content -> div.content\n"
	        "    !!! p\n"
	        ">>> .content -> div.content\n"
	        "    !!! p\n"
	        "    ??? p\n"
	        "    >>> p -> p\n"
	        "    ??? p\n"
	        "    >>> p -> p\n"
	        ">>> .content -> div.content\n"
	        "    !!! p\n"
	        "    ??? p\n"
	        "    >>> p -> p\n"
	        ">>> .content -> div.content.for-mark\n"
	        "    !!! p\n"
	        "    ??? p\n"
	        "    >>> p -> p\n",
	    },
	    {
	        {html, "{img{!imgSrc}}"},
	        {"image1.png", "image2.png", "image3.png", "image4.png"},
	        "!!! img\n"
	        "??? img src=\"image1.png\"\n"
	        ">>> img -> img\n"
	        "??? img src=\"image2.png\" id=\"poster\"\n"
	        ">>> img -> img#poster\n"
	        "??? img src=\"image3.png\"\n"
	        ">>> img -> img\n"
	        "??? img src=\"image4.png\"\n"
	        ">>> img -> img\n",
	    },
	    {
	        {html, "{img{!innerHTML}}"},
	        {{}, {}, {}, {}},
	        "!!! img\n"
	        "??? img src=\"image1.png\"\n"
	        ">>> img -> img\n"
	        "??? img src=\"image2.png\" id=\"poster\"\n"
	        ">>> img -> img#poster\n"
	        "??? img src=\"image3.png\"\n"
	        ">>> img -> img\n"
	        "??? img src=\"image4.png\"\n"
	        ">>> img -> img\n",
	    },
	    {
	        {html, "{#poster{!imgSrc}}"},
	        {{"image2.png"}},
	        "!!! #poster\n"
	        ">>> #poster -> img#poster\n",
	    },
	    {
	        {html, "{.menu{.link{!hrefText}}}"},
	        {
	            "#|First",
	            "example.com|Second",
	            "sub.html|Third",
	            "app.example.com|Fourth",
	            "|Fifth",
	            "|",
	            "|",
	            "|",
	            "|",
	            "|",
	        },
	        "!!! .menu\n"
	        ">>> .menu -> ul.menu\n"
	        "    !!! .link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        ">>> .menu -> ul.menu\n"
	        "    !!! .link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n",
	    },
	    {
	        {html, "{.menu{.link{!aHref}}}"},
	        {
	            "#",
	            "example.com",
	            "sub.html",
	            "app.example.com",
	            "",
	            {},
	            {},
	            {},
	            {},
	            {},
	        },
	        "!!! .menu\n"
	        ">>> .menu -> ul.menu\n"
	        "    !!! .link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        ">>> .menu -> ul.menu\n"
	        "    !!! .link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n"
	        "    >>> .link -> li.link\n",
	    },
	    {
	        {html, "{.for-mark{!textAfterMark}}"},
	        {"Text between. Third Fourth"},
	        "!!! .for-mark\n"
	        ">>> .for-mark -> div.content.for-mark\n",
	    },
	    {
	        {html, "{#endless-span{span{!innerHTML}}}"},
	        {"span, which has <b>no</b> end tag\n"},
	        "!!! #endless-span\n"
	        ">>> #endless-span -> p#endless-span\n"
	        "    !!! span\n"
	        "    ??? span\n"
	        "    >>> span -> span\n",
	    },
	    {
	        {html, "{#endless-span{span{!outerHTML}}}"},
	        {"<span>span, which has <b>no</b> end tag\n"},
	        "!!! #endless-span\n"
	        ">>> #endless-span -> p#endless-span\n"
	        "    !!! span\n"
	        "    ??? span\n"
	        "    >>> span -> span\n",
	    },
	};

	INSTANTIATE_TEST_SUITE_P(lookup,
	                         paths_visit_test,
	                         ::testing::ValuesIn(lookup));
}  // namespace tangle::browser::walk::testing