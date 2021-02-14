// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/jar.hpp>
#include "cookie-dir.hh"
#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

using namespace std::literals;
namespace tangle::cookie::io::testing {
	using ::testing::TestWithParam;

	namespace {
		auto now() {
			static auto when = clock::now();
			return when;
		}

		auto make_time(int year,
		               int month,
		               int day,
		               int hour,
		               int minute,
		               int second) {
			return clock::from_date({year, month, day, hour, minute, second});
		}
	}  // namespace

	struct file_info {
		char const* filename{};
		bool valid{};
		const jar* expected{};
	};

	struct cookie_jar_file : TestWithParam<file_info> {};

	TEST_F(cookie_jar_file, store_temp) {
		jar cookies{
		    {"https_only",
		     "value",
		     {"example.com", "/"},
		     flags::secure | flags::persistent,
		     now() + 3600s},
		    {"no_subdoms",
		     "value",
		     {"example.com", "/"},
		     flags::host_only | flags::persistent,
		     now() + 3600s},
		    {"no_js",
		     "value",
		     {"example.com", "/"},
		     flags::http_only | flags::persistent,
		     now() + 600s},
		};

		cookies.add({"no_js",
		             "value",
		             {"example.com", "/"},
		             flags::http_only | flags::persistent,
		             now() - 10s});

		auto filename = fs::temp_directory_path() / "cookies.tmp";

		cookies.store(filename.string(), now() - 20s);

		jar new_cookies{};
		new_cookies.load(filename.string(), now() + 600s);
	}

	TEST_P(cookie_jar_file, parse) {
		auto& par = GetParam();
		auto& expected = *par.expected;
		jar actual;

		auto filename = fs::path{io::DATA_DIR} / par.filename;
		auto const path = filename.make_preferred().string();
		ASSERT_EQ(par.valid, actual.load(path, now()));
		if (!par.valid) return;

		ASSERT_EQ(expected.size(), actual.size());

		auto pact = std::begin(actual);
		for (auto& ex : expected) {
			auto& act = *pact;
			++pact;

			ASSERT_EQ(ex.name(), act.name());
			ASSERT_EQ(ex.value(), act.value());
			ASSERT_EQ(ex.scope().domain, act.scope().domain);
			ASSERT_EQ(ex.scope().path, act.scope().path);
			ASSERT_EQ(ex.flags(), act.flags());
			ASSERT_EQ(ex.expires(), act.expires());
			ASSERT_EQ(ex.creation_time(), act.creation_time());
			ASSERT_EQ(ex.last_access_time(), act.last_access_time());
		}
	}

	static const jar tested{{"name2",
	                         "value",
	                         {"example.com", "/"},
	                         flags::http_only | flags::persistent,
	                         make_time(2100, 1, 1, 12, 30, 00),
	                         make_time(2016, 5, 29, 12, 30, 00)},
	                        {"name2",
	                         "value2",
	                         {"www.example.com", "/"},
	                         flags::persistent,
	                         make_time(2100, 1, 1, 12, 30, 00),
	                         make_time(2016, 5, 29, 12, 30, 00)}};

	static file_info const files[] = {
	    {"hello.jar", false},
	    {"2.0.jar", false},
	    {"1.0-loose.jar", true, &tested},
	    {"1.0.jar", true, &tested},
	    {"1.1.jar", true, &tested},
	    {"1.0-expired.jar", true, &tested},
	};

	INSTANTIATE_TEST_SUITE_P(files,
	                         cookie_jar_file,
	                         ::testing::ValuesIn(files));
}  // namespace tangle::cookie::io::testing
