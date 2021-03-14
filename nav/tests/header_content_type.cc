// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/nav/headers.hpp>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::nav::testing {
	struct ct_info {
		std::vector<std::pair<header_key, std::string>> headers{};
		std::string_view mime{};
		std::string_view type{};
		std::string_view subtype{};
		header_params params{};

		friend std::ostream& operator<<(std::ostream& out,
		                                ct_info const& info) {
			nav::headers hdrs{};
			for (auto const& [key, value] : info.headers) {
				hdrs.add(key, value);
			}

			auto const linesep = hdrs.size() == 1 ? "" : "\n  ";
			for (auto const& [key, values] : hdrs) {
				auto sep = ':';
				out << linesep << key.name();
				for (auto const& value : values) {
					out << sep << ' ' << value;
					sep = ',';
				}
			}

			return out;
		}
	};

	class content_type : public ::testing::TestWithParam<ct_info> {};

	TEST_P(content_type, parse) {
		auto const& param = GetParam();
		nav::headers hdrs{};
		for (auto const& [key, value] : param.headers) {
			hdrs.add(key, value);
		}

		auto const actual = hdrs.get_content_type();
		ASSERT_EQ(param.mime, actual.mime);
		ASSERT_EQ(param.type, actual.mime_type());
		ASSERT_EQ(param.subtype, actual.mime_subtype());

		for (auto const& [key, value] : param.params) {
			auto it = actual.parameteres.find(key);
			ASSERT_NE(actual.parameteres.end(), it)
			    << "Expected param: " << key << "=" << value;
			ASSERT_EQ(value, it->second);
		}

		for (auto const& [key, value] : actual.parameteres) {
			auto it = param.params.find(key);
			ASSERT_NE(param.params.end(), it)
			    << "Unexpected param: " << key << "=" << value;
		}
	}

#define MIME_TYPE(TYPE, SUBTYPE) TYPE "/" SUBTYPE ""sv, TYPE ""sv, SUBTYPE ""sv
#define OCTET_STREAM MIME_TYPE("application", "octet-stream")
#define TEXT_HTML MIME_TYPE("text", "html")

	static ct_info various[] = {
	    {
	        {},
	        OCTET_STREAM,
	    },
	    {
	        {
	            {header::Content_Type, "*/*"},
	        },
	        OCTET_STREAM,
	    },
	    {
	        {
	            {header::Content_Type, "text/html;"},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type, "text/html;charset;    x=y"},
	        },
	        TEXT_HTML,
	        {
	            {"x", "y"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/html;charset,"},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type, "text/html; vl?ue=; charset=utf-8"},
	        },
	        TEXT_HTML,
	        {
	            {"charset", "utf-8"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/plain;charset=\"gbk, text/html"},
	        },
	        MIME_TYPE("text", "plain"),
	    },
	    {
	        {
	            {header::Content_Type, R"(x/y;field="quoted \a \" \	\\")"},
	        },
	        MIME_TYPE("x", "y"),
	        {
	            {"field", "quoted a \" 	\\"},
	        },
	    },
	};

	static ct_info fetch[] = {
	    // https://fetch.spec.whatwg.org/#example-extract-a-mime-type
	    {
	        {
	            {header::Content_Type, "text/plain;charset=gbk, text/html"},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type,
	             "text/html;charset=gbk;a=b, text/html;x=y"},
	        },
	        TEXT_HTML,
	        {
	            {"x", "y"},
	            {"charset", "gbk"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/html;charset=gbk;a=b"},
	            {header::Content_Type, "text/html;x=y"},
	        },
	        TEXT_HTML,
	        {
	            {"x", "y"},
	            {"charset", "gbk"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/html;charset=gbk"},
	            {header::Content_Type, "x/x"},
	            {header::Content_Type, "text/html;x=y"},
	        },
	        TEXT_HTML,
	        {
	            {"x", "y"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/html"},
	            {header::Content_Type, "cannot-parse"},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type, "text/html"},
	            {header::Content_Type, "*/*"},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type, "text/html"},
	            {header::Content_Type, ""},
	        },
	        TEXT_HTML,
	    },
	    {
	        {
	            {header::Content_Type,
	             R"(text/html;charset="shift_jis"iso-2022-jp)"},
	        },
	        TEXT_HTML,
	        {
	            {"charset", "shift_jis"},
	        },
	    },
	};

	static ct_info fetch_vars[] = {
	    // variantions on tests above
	    {
	        {
	            {header::Content_Type, "text/html;charset=gbk"},
	            {header::Content_Type, "x/x"},
	            {header::Content_Type, "text/HTML;Charset=UTF-8"},
	            {header::Content_Type, "Text/Html;x=y"},
	        },
	        TEXT_HTML,
	        {
	            {"x", "y"},
	            {"charset", "utf-8"},
	        },
	    },
	    {
	        {
	            {header::Content_Type, "text/html"},
	            {header::Content_Type, "cannot-parse, text/plain"},
	        },
	        MIME_TYPE("text", "plain"),
	    },
	};

	INSTANTIATE_TEST_SUITE_P(various,
	                         content_type,
	                         ::testing::ValuesIn(various));

	INSTANTIATE_TEST_SUITE_P(fetch_spec,
	                         content_type,
	                         ::testing::ValuesIn(fetch));

	INSTANTIATE_TEST_SUITE_P(fetch_variantions,
	                         content_type,
	                         ::testing::ValuesIn(fetch_vars));
}  // namespace tangle::nav::testing