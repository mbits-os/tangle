// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <fstream>

#include "dtd_database.hh"
#include "file.hh"

using namespace std::literals;
static constexpr auto autogen_hdr = R"(// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-dtd
// build/bin/html-dtd tools/lookup/html.dtd browser

)"sv;

template <typename Pred>
int write(fs::path const& filename, Pred pred) {
	std::ofstream of{filename};
	if (!of.is_open()) {
		auto u8 = filename.generic_u8string();
#ifdef __cpp_lib_char8_t
		auto u8ptr = reinterpret_cast<char const*>(u8.c_str());
#else
		auto u8ptr = u8.c_str();
#endif
		std::fprintf(stderr, "html-dtd: error: cannot open %s\n", u8ptr);
		return 1;
	}

	pred(of);
	return 0;
}

#define store(filename, op)                                                    \
	if (int result =                                                           \
	        write(filename, [&](std::ostream& out) { op(out, autogen_hdr); })) \
	return result

int main(int argc, char** argv) {
	if (argc < 3) {
		std::fprintf(stderr, "html-dtd <dtd-file> <project-dir>\n");
		return 1;
	}

	auto input = buffered_file::open(argv[1]);
	if (!input.is_open()) {
		std::fprintf(stderr, "html-dtd: error: cannot open %s\n", argv[1]);
		return 1;
	}

	database db;
	db.load(input);

	auto out_dir = fs::path{argv[2]};

	store(out_dir / "src" / "html_dtd.cpp", db.print_code);
	store(out_dir / "include" / "tangle" / "browser" / "html_dtd_flags.hpp",
	      db.print_header);
	store(out_dir / "tests" / "html_dtd.cc", db.print_tests);
}
