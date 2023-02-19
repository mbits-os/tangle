// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/uri.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::testing {
	using ::testing::TestWithParam;
	using ::testing::ValuesIn;

	struct UriCannonicalHrefTest {
		std::string_view base;
		std::string_view href;
		std::string_view expected;
	};

	struct UriCannonicalHexTest {
		std::string_view url;
		std::string_view expected;
		uri::server_quirks quirks = uri::no_quirks;
	};

	std::ostream& operator<<(std::ostream& o,
	                         const UriCannonicalHrefTest& param) {
		return o << "\"" << param.base << "\" + \"" << param.href << "\"";
	}

	std::ostream& operator<<(std::ostream& o,
	                         const UriCannonicalHexTest& param) {
		return o << "\"" << param.url << "\" -> \"" << param.expected << "\"";
	}
}  // namespace tangle::testing

namespace tangle::testing {
	class UriCannonicalHref : public TestWithParam<UriCannonicalHrefTest> {};
	class UriCannonicalHex : public TestWithParam<UriCannonicalHexTest> {};
	class UriCannonicalBase : public TestWithParam<UriCannonicalHexTest> {};

	TEST_P(UriCannonicalHref, HrefAttr) {
		auto const& param = GetParam();
		auto result = uri::canonical(param.href, param.base);
		ASSERT_EQ(param.expected, result);
	}

	TEST_P(UriCannonicalHex, URLDecode) {
		auto const& param = GetParam();
		auto result = uri::canonical(param.url, {}, uri::ui_safe, param.quirks);
		uri copy{};
		copy = result;
		ASSERT_EQ(param.expected, result);
		ASSERT_EQ(result, copy);
	}

	TEST_P(UriCannonicalBase, MakeBase) {
		auto const& param = GetParam();
		auto result = uri::make_base(param.url);
		auto stream = std::ostringstream{};
		stream << result;
		ASSERT_EQ(param.expected, result);
		ASSERT_EQ(result.string(), stream.str());
	}

	// clang-format off
	static const UriCannonicalHrefTest uri_canonical_empties[] = {
		{ "http://example.com",               "",                         "http://example.com/" },
		{ "http://example.com/",              "",                         "http://example.com/" },
		{ "http://example.com",               "/",                        "http://example.com/" },
		{ "http://example.com/",              "/",                        "http://example.com/" },
		{ "http://example.com",               "http://example.net",       "http://example.net/" },
		{ "http://example.com/A/B/some.file", "",                         "http://example.com/A/B/some.file/" },
		{ "http://example.com/A/B/",          "",                         "http://example.com/A/B/" },
	};

	static const UriCannonicalHrefTest uri_canonical_roots[] = {
		{ "http://example.com/A/B/some.file", "/C/other.file",            "http://example.com/C/other.file" },
		{ "http://example.com/A/B/some.file?field=value",
		                                      "/C/other.file",            "http://example.com/C/other.file" },
		{ "http://example.com/A/B/some.file?field=value#anchor",
		                                      "/C/other.file",            "http://example.com/C/other.file" },
		{ "http://example.com/A/B/some.file#anchor",
		                                      "/C/other.file",            "http://example.com/C/other.file" },
		{ "http://example.com/A/B/",          "/C/other.file",            "http://example.com/C/other.file" },
		{ "http://example.com",               "//example.net",            "http://example.net/" },
		{ "https://example.com",              "//example.net",            "https://example.net/" },
	};

	static const UriCannonicalHrefTest uri_canonical_here[] = {
		{ "http://example.com/A/B/some.file", "C/other.file",                  "http://example.com/A/B/some.file/C/other.file" },
		{ "http://example.com/A/B/some.file?field=value",
		                                      "C/other.file",                  "http://example.com/A/B/some.file/C/other.file" },
		{ "http://example.com/A/B/some.file?field=value#anchor",
		                                      "C/other.file",                  "http://example.com/A/B/some.file/C/other.file" },
		{ "http://example.com/A/B/some.file#anchor",
		                                      "C/other.file",                  "http://example.com/A/B/some.file/C/other.file" },
		{ "http://example.com/A/B/",          "C/other.file",                  "http://example.com/A/B/C/other.file" },
		{ "http://example.com/A/B/some.file", "C/other.file?fld=value",        "http://example.com/A/B/some.file/C/other.file?fld=value" },
		{ "http://example.com/A/B/",          "C/other.file?fld=value",        "http://example.com/A/B/C/other.file?fld=value" },
		{ "http://example.com/A/B/some.file", "C/other.file?fld=value#anchor", "http://example.com/A/B/some.file/C/other.file?fld=value#anchor" },
		{ "http://example.com/A/B/",          "C/other.file?fld=value#anchor", "http://example.com/A/B/C/other.file?fld=value#anchor" },
		{ "http://example.com/A/B/some.file", "C/other.file#anchor",           "http://example.com/A/B/some.file/C/other.file#anchor" },
		{ "http://example.com/A/B/",          "C/other.file#anchor",           "http://example.com/A/B/C/other.file#anchor" },
	};

	static const UriCannonicalHrefTest uri_canonical_up[] = {
		{ "http://example.com/A/B/some.file", "../C/other.file",          "http://example.com/A/B/C/other.file" },
		{ "http://example.com/A/B/some.file", "../../C/other.file",       "http://example.com/A/C/other.file" },
		{ "http://example.com/A/B/some.file", "../../../C/other.file",    "http://example.com/C/other.file" },
		{ "http://example.com/A/B/some.file", "../../../../C/other.file", "http://example.com/C/other.file" },
		{ "http://example.com/A/B/",          "../C/other.file",          "http://example.com/A/C/other.file" },
		{ "http://example.com/A/B/",          "../../C/other.file",       "http://example.com/C/other.file" },
		{ "http://example.com/A/B/",          "../../../C/other.file",    "http://example.com/C/other.file" },
	};

	static const UriCannonicalHrefTest uri_canonical_opaque[] = {
		{ "http://example.com/A/B/",          "http:C/other.file",        "http://example.com/A/B/C/other.file" },
		{ "http://example.com/A/B/",          "http:/C/other.file",       "http://example.com/C/other.file" },
		{ "http://example.com/A/B/",          "https:C/other.file",       "https:C/other.file" },
		{ "http://example.com/A/B/",          "https:/C/other.file",      "https:/C/other.file" },
		{ "http://example.com/A/B/",          "mailto:user@server",       "mailto:user@server" },
		{ "http://example.com/A/B/",          "ftp:C/other.file",         "ftp:C/other.file" },
		{ "https://example.com/A/B/",         "http:C/other.file",        "http:C/other.file" },
		{ "https://example.com/A/B/",         "http:/C/other.file",       "http:/C/other.file" },
		{ "https://example.com/A/B/",         "https:C/other.file",       "https://example.com/A/B/C/other.file" },
		{ "https://example.com/A/B/",         "https:/C/other.file",      "https://example.com/C/other.file" },
		{ "https://example.com/A/B/",         "mailto:user@server",       "mailto:user@server" },
		{ "https://example.com/A/B/",         "ftp:C/other.file",         "ftp:C/other.file" },
		{ "//example.com/A/B/",              "ftp:C/other.file",         "ftp:C/other.file" },
		// here, both uris are opaque, so nothing is done...
		{ "ftp:A/B",                          "ftp:../C/other.file",      "ftp:../C/other.file" },
		{ "mailto:userA@serverA",             "mailto:userB@serverB",     "mailto:userB@serverB" },
	};

	static const UriCannonicalHexTest uri_canonical_decode_auth[] = {
		{ "http://example.com/",                               "http://example.com/" },
		{ "http://%45xample.com/",                             "http://example.com/" },
		{ "http://example%2Fslash.com/",                       "" },
		{ "HTTP://EXAMPLE.COM/",                               "http://example.com/" },
		{ "//EXAMPLE.COM/",                                    "//example.com/" },
		{ "http://127.0.0.1/",                                 "http://127.0.0.1/" },
		{ "http://[::1]/",                                     "http://[::1]/" },
		{ "http://[::1]:/",                                    "http://[::1]/" },
		{ "http://[::1]:8080/",                                "http://[::1]:8080/" },
		{ "http://[::1],80/",                                  "" },
		{ "http://example.com:/",                              "http://example.com/" },
		{ "http://example.com:80/",                            "http://example.com/" },
		{ "https://example.com:443/",                          "https://example.com/" },
		{ "ftp://example.com:21/",                             "ftp://example.com/" },
		{ "ssh://example.com:22/",                             "ssh://example.com/" },
		{ "ssh://example.com:95/",                             "ssh://example.com:95/" },
		{ "telnet://example.com:23/",                          "telnet://example.com/" },
		{ "smb://example.com:445/",                            "smb://example.com:445/" },
		{ "http://example.com:8080/",                          "http://example.com:8080/" },
		{ "http://example.com:blah/",                          "" },
		{ "http://example.com:b2/",                            "" },
		{ "http://example.com:2b/",                            "" },
	};

	static const UriCannonicalHexTest uri_canonical_decode_path[] = {
		{ "http://example.com/FileName.ext",                   "http://example.com/FileName.ext" },
		{ "http://EXAMPLE.COM/FileName.ext",                   "http://example.com/FileName.ext" },
		{ "http://example.com/Dir/subdir%2Ffake/FileName.ext", "http://example.com/Dir/subdir%2Ffake/FileName.ext" },
		{ "http://example.com/Dir/subdir%2ffake/FileName.ext", "http://example.com/Dir/subdir%2Ffake/FileName.ext" },
		{ "http://example.com/Dir/subdir/Fil%65Name.ext",      "http://example.com/Dir/subdir/FileName.ext" },
		{ "http://example.com/Dir/subdir/File Name.ext",       "http://example.com/Dir/subdir/File%20Name.ext" },
		{ "http://example.com/Dir/subdir/File+Name.ext",       "http://example.com/Dir/subdir/File+Name.ext" },
		{ "http://example.com/A/B/./some.file",                "http://example.com/A/B/some.file" },
		{ "http://example.com/A/B/./",                         "http://example.com/A/B/" },
		{ "http://example.com/A/B/.",                          "http://example.com/A/B/" },
		{ "http://example.com/A/B/../some.file",               "http://example.com/A/some.file" },
		{ "http://example.com/A/B/../",                        "http://example.com/A/" },
		{ "http://example.com/A/B/..",                         "http://example.com/A/" },
		{ "http://example.com/A/B/../../../some.file",         "http://example.com/some.file" },
		{ "http://example.com/A/B/../../../C/../",             "http://example.com/" },
		{ "http://example.com/A/B/../../.%2e/C/..",            "http://example.com/" },
		{ "http://example.com/%7Euser",                        "http://example.com/~user" },
		{ "http://example.com/one+other",                      "http://example.com/one+other" },
		{ "http://example.com/one+other",                      "http://example.com/one%2Bother", uri::no_plus_in_path },
		{ "http://example.com/%%30%30",                        "http://example.com/%2500" }
	};

	static const UriCannonicalHexTest uri_canonical_decode_relpath[] = {
		{ "A/B/../../../some.file",                             "../some.file" },
		{ "/A/B/../../../some.file",                            "/some.file" },
		{ "A/B/../../some.file",                                "some.file" },
		{ "A/../../../some.file",                               "../../some.file" },
		// nothing is done to opaque uri's path
		{ "ftp:A/B/../C/other.file",                            "ftp:A/B/../C/other.file" },
	};

	static const UriCannonicalHexTest uri_canonical_decode_query[] = {
		{ "?name=value",                                        "?name=value" },
		{ "?name=value with space",                             "?name=value+with+space" },
		{ "?name=value+with+space",                             "?name=value+with+space" },
		{ "?name=value%20with%20space",                         "?name=value+with+space" },
	};

	static const UriCannonicalHexTest uri_make_base[] = {
		{ "",                                                   "" },
		{ "one-only",                                           "http://one-only/" },
		{ "one-only/leaf",                                      "http://one-only/" },
		{ "one-only/dir/",                                      "http://one-only/dir/" },
		{ "one-only/dir/leaf",                                  "http://one-only/dir/" },
		{ "one-only/dir/sub/",                                  "http://one-only/dir/sub/" },
		{ "https://server",                                     "https://server/" },
		{ "https://server/",                                    "https://server/" },
		{ "https://server/leaf",                                "https://server/" },
		{ "https://server/dir/",                                "https://server/dir/" },
		{ "https://server/dir/leaf",                            "https://server/dir/" },
		{ "https://server/dir/sub/",                            "https://server/dir/sub/" },
	};
	// clang-format off

#define URI_CANONICAL(Name, Domain, arr) \
	INSTANTIATE_TEST_SUITE_P(Name, UriCannonical ## Domain, ValuesIn(arr))

	URI_CANONICAL(Empties, Href, uri_canonical_empties);
	URI_CANONICAL(Roots, Href, uri_canonical_roots);
	URI_CANONICAL(Here, Href, uri_canonical_here);
	URI_CANONICAL(Up, Href, uri_canonical_up);
	URI_CANONICAL(Opaque, Href, uri_canonical_opaque);
	URI_CANONICAL(Authority, Hex, uri_canonical_decode_auth);
	URI_CANONICAL(Path, Hex, uri_canonical_decode_path);
	URI_CANONICAL(RelPath, Hex, uri_canonical_decode_relpath);
	URI_CANONICAL(Query, Hex, uri_canonical_decode_query);
	URI_CANONICAL(MakeBase, Base, uri_make_base);
}
