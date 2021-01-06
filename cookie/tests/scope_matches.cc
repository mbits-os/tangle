// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/cookie/scope.hpp>

struct scope_info {
	tangle::cookie::scope_type in, out;
	bool host_only;
	bool expected;
	std::string explain;
};

std::ostream& operator<<(std::ostream& o, const scope_info& nfo) {
	o << "(" << nfo.out.domain << nfo.out.path << " =~ " << nfo.in.domain
	  << nfo.in.path;
	if (nfo.host_only) o << " [host-only]";
	return o << ")";
}

class scope : public ::testing::TestWithParam<scope_info> {};

TEST_P(scope, matches) {
	auto& par = GetParam();
	auto& in = par.in;
	auto& out = par.out;
	auto host_only = par.host_only;
	auto expected = par.expected;
	ASSERT_EQ(expected, in.matches(out, host_only)) << "Why: " << par.explain;
}

scope_info domains[] = {
    // MATCH-SCOPE
    // MATCHED-SCOPE
    // HOST-ONLY
    // EXPECTED
    {
        {"example.com", "/"},
        {"www.example.com", "/"},
        false,
        true,
        "domain-suffix matches, when host-only is FALSE",
    },
    {
        {"example.com", "/"},
        {"www.example.com", "/"},
        true,
        false,
        "domain-suffix does not match, when host-only is TRUE",
    },
    {
        {"example.com", "/"},
        {"example.com", "/"},
        true,
        true,
        "same matches always",
    },
    {
        {"example.com", "/"},
        {"example.com", "/"},
        false,
        true,
        "same matches always",
    },
    {
        {"ns.example.com", "/"},
        {"www.example.com", "/"},
        true,
        false,
        "both have common suffix, but neither is suffix for the other one",
    },
    {
        {"ns.example.com", "/"},
        {"www.example.com", "/"},
        false,
        false,
        "both have common suffix, but neither is suffix for the other one",
    },
    {
        {"ns.example.com", "/"},
        {"example.com", "/"},
        true,
        false,
        "mached is shorter than match, therefore match is not its suffix",
    },
    {
        {"ns.example.com", "/"},
        {"example.com", "/"},
        false,
        false,
        "mached is shorter than match, therefore match is not its suffix",
    },
    {
        {"ns.example.com", "/"},
        {"dns.example.com", "/"},
        false,
        false,
        "suffix, but not a domain-suffix",
    },
};

constexpr char example[] = "www.example.com";
scope_info paths[] = {
    // MATCH-SCOPE
    // MATCHED-SCOPE
    // HOST-ONLY
    // EXPECTED
    {
        {example, "/"},
        {example, "/dir/sub"},
        false,
        true,
        "root matches anything",
    },
    {
        {example, ""},
        {example, "/dir/sub"},
        false,
        true,
        "empty is as-if root (matches anything)",
    },
    {{example, "/dir/sub"},
     {example, "/dir/sub"},
     false,
     true,
     "same matches always"},
    {
        {example, "/dir/sub"},
        {example, "/dir/sub/"},
        false,
        true,
        "non-slashed \"self\" matches slashed",
    },
    {
        {example, "/dir/sub/"},
        {example, "/dir/sub"},
        false,
        false,
        "slashed \"self\" does not match non-slashed (it's not a prefix)",
    },
    {
        {example, "/dir/sub"},
        {example, "/dir/sub/a/b/c"},
        false,
        true,
        "prefix is path-prefix, when first non-same is slash",
    },
    {
        {example, "/dir/sub/"},
        {example, "/dir/sub/a/b/c"},
        false,
        true,
        "prefix is path-prefix, when last same is slash",
    },
    {
        {example, "/dir/a"},
        {example, "/dir/b"},
        false,
        false,
        "this match is not a prefix for matched",
    },
    {
        {example, "/dir/a/b/c"},
        {example, "/dir/a/b"},
        false,
        false,
        "common prefix, but match is not prefix for matched",
    },
    {
        {example, "/dir/a/b/c"},
        {example, "/dir/a/b/cd"},
        false,
        false,
        "prefix, but not path-prefix",
    },
};

INSTANTIATE_TEST_CASE_P(domains, scope, ::testing::ValuesIn(domains));
INSTANTIATE_TEST_CASE_P(paths, scope, ::testing::ValuesIn(paths));
