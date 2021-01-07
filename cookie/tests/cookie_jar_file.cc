// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/cookie/jar.hpp>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std::literals;
namespace tangle::cookie::io::testing {
	using ::testing::Test;

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

	struct cookie_jar_file : Test {};

	TEST_F(cookie_jar_file, store_temp) {
		jar cookies{
		    {"https_only", "value", {"example.com", "/"}, flags::secure | flags::persistent, now() + 3600s},
		    {"no_subdoms", "value", {"example.com", "/"}, flags::host_only | flags::persistent, now() + 3600s},
		    {"no_js", "value", {"example.com", "/"}, flags::http_only | flags::persistent, now() + 600s},
		};

		cookies.add({"no_js", "value", {"example.com", "/"}, flags::http_only | flags::persistent, now() - 10s});

        auto filename = fs::temp_directory_path() / "cookies.tmp";

        cookies.store(filename.string(), now() - 20s);

        jar new_cookies{};
        new_cookies.load(filename.string(), now() + 600s);
	}

}  // namespace tangle::cookie::io::testing
