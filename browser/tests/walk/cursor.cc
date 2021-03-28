// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/cursor.hpp>
#include <tangle/browser/walk/selector.hpp>
#include "cxx_string.hh"
#include <sstream>
#include <stack>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::walk::testing {
	struct cursor_test_data {
		struct header {
			std::string_view html;
			std::vector<std::string_view> path;
			std::string (*modifier)(cursor& scope);
		} hdr;
		std::vector<std::string_view> expected;

		friend std::ostream& operator<<(std::ostream& out,
		                                cursor_test_data const& test) {
			if (test.hdr.path.empty()) return out << "[empty]";
			auto first = true;
			for (auto& selector : test.hdr.path) {
				if (first)
					first = false;
				else
					out << " -> ";
				out << selector;
			}
			return out;
		}

		void query(cursor& scope,
		           size_t depth,
		           std::vector<std::string>& output) const {
			if (depth == hdr.path.size()) {
				output.push_back(hdr.modifier(scope));
				// std::cout << ">>> apply " << cxx_string{output.back(), 120}
				//           << "\n\n";
				return;
			}

			auto sel = selector::parse(hdr.path[depth]);

			if (!scope.eof()) ++scope;

			while (!scope.eof()) {
				auto& element = *scope;

				if (!sel.matches(element)) {
					++scope;
					continue;
				}

				// std::cout << ">>> sel " << sel << "\n";
				auto sub = scope.forCurrentElement();
				query(sub, depth + 1, output);
			}
		}
	};

	class cursor_test : public ::testing::TestWithParam<cursor_test_data> {};

	TEST(cursor_test, empty) {
		cursor cur{};
		ASSERT_TRUE(cur.eof());
	}

	TEST_P(cursor_test, navigate) {
		auto& param = GetParam();

		root_cursor env{param.hdr.html};
		std::vector<std::string> actual;
		param.query(env.root(), 0, actual);

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

	std::string innerText(cursor& scope) { return scope.innerText(); }

	std::string innerHTML(cursor& scope) {
		std::string out{};
		out.assign(scope.innerHTML().text);
		return out;
	}

	std::string outerHTML(cursor& scope) {
		std::string out{};
		out.assign(scope.text);
		return out;
	}

	std::string imgSrc(cursor& scope) { return scope.attr("img"sv, "src"sv); }
	std::string aHref(cursor& scope) { return scope.attr("a"sv, "href"sv); }

	std::string hrefText(cursor& scope) {
		auto ht = scope.aHrefText();
		return ht.href + "|"s + ht.text;
	}

	std::string textAfterMark(cursor& scope) {
		auto sel = selector::parse("#mark");
		while (!scope.eof()) {
			if (!sel.matches(*scope)) {
				++scope;
				continue;
			}

			[[maybe_unused]] auto _ = scope.forCurrentElement();
			auto real = scope.copyFromHere();
			return real.innerText();
		}
		return {};
	}

	static cursor_test_data const lookup[] = {
	    {{html, {"head", "title"}, innerText}, {"Title of the page"}},
	    {{html, {"title"}, innerText}, {"Title of the page"}},
	    {
	        {html, {".content"}, innerHTML},
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
	        {html, {".content", "p"}, innerText},
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
	        {html, {"img"}, imgSrc},
	        {"image1.png", "image2.png", "image3.png", "image4.png"},
	    },
	    {
	        {html, {"img"}, innerHTML},
	        {{}, {}, {}, {}},
	    },
	    {{html, {"#poster"}, imgSrc}, {{"image2.png"}}},
	    {
	        {html, {".menu", ".link"}, hrefText},
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
	        {html, {".menu", ".link"}, aHref},
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
	    {{html, {".for-mark"}, textAfterMark}, {"Text between. Third Fourth"}},
	    {
	        {html, {"#endless-span", "span"}, innerHTML},
	        {"span, which has <b>no</b> end tag\n"},
	    },
	    {
	        {html, {"#endless-span", "span"}, outerHTML},
	        {"<span>span, which has <b>no</b> end tag\n"},
	    },
	    {
	        {
	            R"html(<html>
<head>
    <title>Title of the page</title>
</head>
<body>
<header><div>First <p>(with a paragraph)</p> and some more text</header>
<div>Second</div>
<footer>
<div>Third
</footer>
<div>Fourth)html",
	            {"div"},
	            innerHTML,
	        },
	        {
	            "First <p>(with a paragraph)</p> and some more text",
	            "Second",
	            "Third\n",
	            "Fourth",
	        },
	    },
	    {
	        {html, {"#mixed-phrasing"}, innerText},
	        {"Normal text, bold and italic and underline, just italic, just "
	         "underline"},
	    },
	    {
	        {html, {"#mixed-phrasing-ii"}, innerText},
	        {"Normal text, bold and italic and underline, just italic, just "
	         "underline"},
	    },
	    {
	        {html, {"#mixed-phrasing-iii"}, innerText},
	        {"Normal text, bold and italic and underline, just italic, just "
	         "underline"},
	    },
	};

	INSTANTIATE_TEST_SUITE_P(lookup, cursor_test, ::testing::ValuesIn(lookup));
}  // namespace tangle::browser::walk::testing
