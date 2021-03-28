// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/pages.hpp>
#include <tangle/browser/walk/std_actions.hpp>
#include "cxx_string.hh"
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::actions {
	std::string varname(std::vector<std::string> const& args);
}

namespace tangle::browser::walk::testing {
	using browser::testing::cxx_string;
	std::string textAfterMark(cursor& scope);

	struct std_action_test_data {
		struct header {
			std::string_view markup;
			std::string page_id{};
		} hdr;
		std::vector<std::pair<std::string_view, std::string_view>> expected;
		std::string_view debug_output{};

		friend std::ostream& operator<<(std::ostream& out,
		                                std_action_test_data const& test) {
			return out << test.hdr.page_id << ' '
			           << cxx_string{test.hdr.markup, 60};
		}
	};

	struct test_pages : parsed_pages<test_pages> {
		static std::string_view pages_text() {
			return R"(
head-title {head{title{!text[head]}}}
title-only {title{!text[direct]}}
content-markup {.content{!innerHTML[content]}}
content-p-text {.content{p{!text}}}
img-src {img{!image}}
img-innerHTML {img{!innerHTML}}
poster {#poster{!image !srcset[sized]}}
menu-text {.menu{.link{!linkText}}}
menu-links {.menu{.link{!link}}}
after-mark {.for-mark{!textAfterMark[still works]}}
endless-inner {#endless-span{span{!innerHTML}}}
endless-outer {#endless-span{span{!outerHTML}}}
)"sv;
		}
	};

	template <typename Final>
	struct test_matcher_base
	    : page_matcher<Final,
	                   std::vector<std::pair<std::string, std::string>>,
	                   test_pages> {
		using base_class =
		    page_matcher<Final,
		                 std::vector<std::pair<std::string, std::string>>,
		                 test_pages>;
		static std::string cur_page_name;
		static std::string const& page_name() noexcept { return cur_page_name; }
	};
	template <typename Final>
	std::string test_matcher_base<Final>::cur_page_name{};

	struct test_matcher_false : test_matcher_base<test_matcher_false> {
		void setup_actions(action_callbacks& actions) {
			base_class::setup_actions(actions);
			actions["textAfterMark"] = action_callback::wrap(
			    [](selector_action const&, cursor&) { return false; });
		}
	};

	struct test_matcher : test_matcher_base<test_matcher> {
		void setup_actions(action_callbacks& actions) {
			base_class::setup_actions(actions);

			auto self = this;
			actions["textAfterMark"] = action_callback::wrap(
			    [self](selector_action const& action, cursor& scope) {
				    self->result.push_back(
				        {actions::varname(action.args), textAfterMark(scope)});
				    return true;
			    });
		}

		bool set_string(std::string const& name,
		                std::string_view value) override {
			result.emplace_back(name, std::string{value.data(), value.size()});
			return true;
		}

		bool set_href_text(std::string const& name,
		                   href_text const& value) override {
			result.emplace_back(name, value.href + "|" + value.text);
			return true;
		}
	};

	struct std_action_test : ::testing::TestWithParam<std_action_test_data> {};

	TEST_P(std_action_test, walk) {
		auto& param = GetParam();
		test_matcher::cur_page_name = param.hdr.page_id;
		auto actual = test_matcher{}.visit(param.hdr.markup);
		ASSERT_EQ(param.expected.size(), actual.size());

		auto it = actual.begin();
		for (auto& exp : param.expected) {
			auto& act = *it++;
			std::string xpctd{};

			xpctd.assign(exp.first);
			ASSERT_EQ(xpctd, act.first);

			xpctd.assign(exp.second);
			ASSERT_EQ(xpctd, act.second);
		}
	}

	TEST_P(std_action_test, walk_false) {
		auto& param = GetParam();
		test_matcher_false::cur_page_name = param.hdr.page_id;
		auto actual = test_matcher_false{}.visit(param.hdr.markup);
		ASSERT_EQ(0, actual.size());
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
<img id='poster' src='image2.png' srcset='url1 size1 url2 size2 url3 size3' />
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

	static std_action_test_data const lookup[] = {
	    {{html, "head-title"}, {{"head", "Title of the page"}}},
	    {{html, "title-only"}, {{"direct", "Title of the page"}}},
	    {
	        {html, "content-markup"},
	        {
	            {"content", {}},

	            {"content",
	             "\n<p> <i> Lorem ipsum dolor sit amet, consectetur "
	             "adipiscing "
	             "elit.</i> Nulla ultricies\nnunc rhoncus gravida "
	             "blandit. In "
	             "non erat cursus, feugiat elit id, facilisis\nnibh. "
	             "Nulla "
	             "vehicula neque sit amet urna ornare, vitae aliquet "
	             "arcu "
	             "pulvinar.\nIn hac habitasse platea dictumst. Donec "
	             "tincidunt, "
	             "mauris in viverra posuere,\nsem nisi bibendum eros, "
	             "vel "
	             "rutrum justo justo accumsan mauris. "
	             "Integer\nlobortis vitae "
	             "tortor sit amet dignissim. Cras rutrum lectus ac "
	             "consequat\ndapibus. Phasellus hendrerit magna orci. "
	             "Vivamus "
	             "gravida a turpis sed commodo.\nNullam posuere ut "
	             "mauris "
	             "vitae "
	             "eleifend. Curabitur in ornare orci. "
	             "Nulla\nconsectetur "
	             "commodo tortor sit amet cursus.</p>\n\n<p>Quisque "
	             "nec "
	             "sagittis dui, in dapibus metus. Nam varius pulvinar "
	             "felis, "
	             "nec\nrhoncus orci tincidunt id. Ut luctus, nisl "
	             "scelerisque "
	             "imperdiet dictum, turpis\njusto rhoncus dui, non "
	             "tempor odio "
	             "est vel nibh. Interdum et malesuada fames ac\nante "
	             "ipsum "
	             "primis in faucibus. Nullam sed congue risus. Sed "
	             "molestie "
	             "magna in\nante faucibus, eget porta ante ultricies. "
	             "Aenean "
	             "efficitur finibus tortor.\nNulla eu facilisis nulla. "
	             "Integer "
	             "quis lorem quam.</p>\n\n"},

	            {"content",
	             "\n\n<p>\nInteger facilisis diam at augue convallis "
	             "congue "
	             "vitae eu elit. Vestibulum\nante ipsum primis in "
	             "faucibus "
	             "orci "
	             "luctus et ultrices posuere cubilia curae;\nQuisque "
	             "placerat "
	             "arcu vitae tempor venenatis. Integer commodo arcu a "
	             "tellus\naliquet euismod. Donec eu finibus sapien. "
	             "Vivamus ac "
	             "nisi ac mi dignissim\nconsectetur in vestibulum "
	             "neque. "
	             "Quisque non justo nunc. Nullam ut facilisis\ndiam. "
	             "Vivamus "
	             "eget facilisis odio. Donec at urna quis neque "
	             "eleifend "
	             "blandit\nfinibus a eros. Phasellus non viverra orci. "
	             "Nam "
	             "rutrum ornare nulla, et congue\npurus posuere in. "
	             "Suspendisse "
	             "ac nisi justo. Aliquam nec nisi in urna "
	             "sagittis\nmollis. "
	             "Vestibulum id convallis enim. Maecenas lacinia purus "
	             "non "
	             "massa\nporttitor, at accumsan urna "
	             "sagittis.\n</p>\n\n"},

	            {"content",
	             "\n    <div>First</div>\n    <div "
	             "id=mark>Second</div>\n    "
	             "Text between.\n    <div>\n        Third\n        "
	             "<div><p>Fourth</p></div>\n    </div>\n"},
	        },
	    },
	    {
	        {html, "content-p-text"},
	        {
	            {{},
	             "Lorem ipsum dolor sit amet, consectetur adipiscing "
	             "elit. "
	             "Nulla ultricies nunc rhoncus gravida blandit. In non "
	             "erat "
	             "cursus, feugiat elit id, facilisis nibh. Nulla "
	             "vehicula "
	             "neque "
	             "sit amet urna ornare, vitae aliquet arcu pulvinar. "
	             "In hac "
	             "habitasse platea dictumst. Donec tincidunt, mauris "
	             "in "
	             "viverra "
	             "posuere, sem nisi bibendum eros, vel rutrum justo "
	             "justo "
	             "accumsan mauris. Integer lobortis vitae tortor sit "
	             "amet "
	             "dignissim. Cras rutrum lectus ac consequat dapibus. "
	             "Phasellus "
	             "hendrerit magna orci. Vivamus gravida a turpis sed "
	             "commodo. "
	             "Nullam posuere ut mauris vitae eleifend. Curabitur "
	             "in ornare "
	             "orci. Nulla consectetur commodo tortor sit amet "
	             "cursus."},

	            {{},
	             "Quisque nec sagittis dui, in dapibus metus. Nam "
	             "varius "
	             "pulvinar felis, nec rhoncus orci tincidunt id. Ut "
	             "luctus, "
	             "nisl scelerisque imperdiet dictum, turpis justo "
	             "rhoncus dui, "
	             "non tempor odio est vel nibh. Interdum et malesuada "
	             "fames ac "
	             "ante ipsum primis in faucibus. Nullam sed congue "
	             "risus. Sed "
	             "molestie magna in ante faucibus, eget porta ante "
	             "ultricies. "
	             "Aenean efficitur finibus tortor. Nulla eu facilisis "
	             "nulla. "
	             "Integer quis lorem quam."},

	            {{},
	             "Integer facilisis diam at augue convallis congue "
	             "vitae eu "
	             "elit. Vestibulum ante ipsum primis in faucibus orci "
	             "luctus "
	             "et "
	             "ultrices posuere cubilia curae; Quisque placerat "
	             "arcu vitae "
	             "tempor venenatis. Integer commodo arcu a tellus "
	             "aliquet "
	             "euismod. Donec eu finibus sapien. Vivamus ac nisi ac "
	             "mi "
	             "dignissim consectetur in vestibulum neque. Quisque "
	             "non justo "
	             "nunc. Nullam ut facilisis diam. Vivamus eget "
	             "facilisis odio. "
	             "Donec at urna quis neque eleifend blandit finibus a "
	             "eros. "
	             "Phasellus non viverra orci. Nam rutrum ornare nulla, "
	             "et "
	             "congue purus posuere in. Suspendisse ac nisi justo. "
	             "Aliquam "
	             "nec nisi in urna sagittis mollis. Vestibulum id "
	             "convallis "
	             "enim. Maecenas lacinia purus non massa porttitor, at "
	             "accumsan "
	             "urna sagittis."},

	            {{}, "Fourth"},
	        },
	    },
	    {
	        {html, "img-src"},
	        {{{}, "image1.png"},
	         {{}, "image2.png"},
	         {{}, "image3.png"},
	         {{}, "image4.png"}},
	    },
	    {
	        {html, "img-innerHTML"},
	        {{{}, {}}, {{}, {}}, {{}, {}}, {{}, {}}},
	    },
	    {
	        {html, "poster"},
	        {
	            {{}, "image2.png"},
	            {"sized", "url1 size1 url2 size2 url3 size3"},
	        },
	    },
	    {
	        {html, "menu-text"},
	        {
	            {{}, "#|First"},
	            {{}, "example.com|Second"},
	            {{}, "sub.html|Third"},
	            {{}, "app.example.com|Fourth"},
	            {{}, "|Fifth"},
	            {{}, "|"},
	            {{}, "|"},
	            {{}, "|"},
	            {{}, "|"},
	            {{}, "|"},
	        },
	    },
	    {
	        {html, "menu-links"},
	        {
	            {{}, "#"},
	            {{}, "example.com"},
	            {{}, "sub.html"},
	            {{}, "app.example.com"},
	            {{}, ""},
	            {{}, {}},
	            {{}, {}},
	            {{}, {}},
	            {{}, {}},
	            {{}, {}},
	        },
	    },
	    {
	        {html, "after-mark"},
	        {{"still", "Text between. Third Fourth"}},
	    },
	    {
	        {html, "endless-inner"},
	        {{{}, "span, which has <b>no</b> end tag\n"}},
	    },
	    {
	        {html, "endless-outer"},
	        {{{}, "<span>span, which has <b>no</b> end tag\n"}},
	    },
	};

	INSTANTIATE_TEST_SUITE_P(lookup,
	                         std_action_test,
	                         ::testing::ValuesIn(lookup));
}  // namespace tangle::browser::walk::testing
