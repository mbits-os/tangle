// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "paths.hh"
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::testing {
	struct pages_parse_test_data {
		std::string_view input;
		std::unordered_map<std::string, std::string> expected;

		friend std::ostream& operator<<(std::ostream& out,
		                                pages_parse_test_data const& test) {
			return out << cxx_string{test.input, 160};
		}
	};

	struct pages_parse_test : ::testing::TestWithParam<pages_parse_test_data> {
	};

	TEST_P(pages_parse_test, parse) {
		auto& param = GetParam();
		auto actual = pages::parse_definition(param.input);

		ASSERT_EQ(param.expected.size(), actual.defs.size());

		for (auto& [key, paths] : actual.defs) {
			auto it = param.expected.find(key);
			ASSERT_NE(it, param.expected.end()) << "Key: " << key;
			auto& exp = it->second;

			std::ostringstream out;
			out << paths_print{paths.tracks};
			ASSERT_EQ(exp, out.str());
		}
	}

	static pages_parse_test_data const parse[] = {
	    {{}, {}},
	    {"{ anything }", {}},
	    {"anything token", {}},
	    {"anything }", {}},
	    {
	        "anything {}",
	        {{"anything", "{}"}},
	    },
	    {
	        "page::1 { div { #id { !text } } } page::2 {}",
	        {
	            {"page::1", "{ div { #id { !text } } }"},
	            {"page::2", "{}"},
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(items,
	                         pages_parse_test,
	                         ::testing::ValuesIn(parse));

	struct pages_visit_test : ::testing::TestWithParam<visit_data> {};

	TEST_P(pages_visit_test, walk) {
		auto& param = GetParam();
		root_cursor cur{param.hdr.markup};
		auto defs = pages::parse_definition(param.hdr.definition);

		std::vector<std::string> actual{};
		auto callbacks = setup(&actual);
		ASSERT_EQ(param.hdr.result_during_walk,
		          defs.visit(cur.root(), param.hdr.page_id, callbacks));

		ASSERT_EQ(param.expected.size(), actual.size());

		auto it = actual.begin();
		for (auto& exp : param.expected) {
			std::string xpctd{};
			xpctd.assign(exp);
			auto& act = *it++;
			ASSERT_EQ(xpctd, act);
		}
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

	static constexpr auto page_paths = R"(
head-title {head{title{!innerText}}}
title-only {title{!innerText}}
content-markup {.content{!innerHTML}}
content-p-text {.content{p{!innerText}}}
img-src {img{!imgSrc}}
img-innerHTML {img{!innerHTML}}
poster {#poster{!imgSrc}}
menu-text {.menu{.link{!hrefText}}}
menu-links {.menu{.link{!aHref}}}
after-mark {.for-mark{!textAfterMark}}
endless-inner {#endless-span{span{!innerHTML}}}
endless-outer {#endless-span{span{!outerHTML}}}
)"sv;

	static visit_data const lookup[] = {
	    {{html, page_paths, "no-such", false}, {}},
	    {{html, page_paths, "head-title"}, {"Title of the page"}},
	    {{html, page_paths, "title-only"}, {"Title of the page"}},
	    {
	        {html, page_paths, "content-markup"},
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
	    },
	    {
	        {html, page_paths, "content-p-text"},
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
	    },
	    {
	        {html, page_paths, "img-src"},
	        {"image1.png", "image2.png", "image3.png", "image4.png"},
	    },
	    {
	        {html, page_paths, "img-innerHTML"},
	        {{}, {}, {}, {}},
	    },
	    {
	        {html, page_paths, "poster"},
	        {{"image2.png"}},
	    },
	    {
	        {html, page_paths, "menu-text"},
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
	    },
	    {
	        {html, page_paths, "menu-links"},
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
	    },
	    {
	        {html, page_paths, "after-mark"},
	        {"Text between. Third Fourth"},
	    },
	    {
	        {html, page_paths, "endless-inner"},
	        {"span, which has <b>no</b> end tag\n"},
	    },
	    {
	        {html, page_paths, "endless-outer"},
	        {"<span>span, which has <b>no</b> end tag\n"},
	    },
	};

	INSTANTIATE_TEST_SUITE_P(lookup,
	                         pages_visit_test,
	                         ::testing::ValuesIn(lookup));
}  // namespace tangle::browser::walk::testing
