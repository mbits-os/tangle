// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "file.hh"
#include "json.hh"
#include "lookup.hh"
#include <cstring>
#include <fstream>

using namespace std::literals;
static constexpr auto autogen_hdr = R"(// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)
//
// clang-format off
//
// This file is generated, to recreate, call
// ninja -C build html-entities
// curl https://html.spec.whatwg.org/entities.json | build/bin/html-entities - browser/src/entities.cpp browser/tests/html_entities.cc

)"sv;

std::string contents_from(FILE* file) {
	std::string contents;

	char buffer[8192];
	size_t size = sizeof(buffer);
	size_t read;
	while ((read = fread(buffer, 1, size, file)) > 0) {
		contents.append(buffer, buffer + read);
	}

	return contents;
}

json::value read(FILE* file) {
	auto contents = contents_from(file);
	return json::read_json(contents);
}

json::value read(char const* path) {
	if (!std::strcmp(path, "-")) return read(stdin);

	auto input = file::open(path);
	if (!input) {
		std::fprintf(stderr, "html-dtd: error: cannot open %s\n", path);
		std::exit(1);
	}

	return read(input.get());
}

std::string printable(std::string_view input) {
	size_t length = input.size() + 2;
	for (auto c : input) {
		if (!std::isprint(static_cast<unsigned char>(c)))
			length += 3;
		else if (c == '\\' || c == '\"')
			++length;
	}

	std::string result{};
	result.reserve(length);
	result.push_back('"');
	for (auto c : input) {
		if (!std::isprint(static_cast<unsigned char>(c))) {
			char buffer[10] = "";
			sprintf(buffer, "\\x%02x", static_cast<unsigned char>(c));
			result.append(buffer);
			continue;
		}
		if (c == '\\' || c == '\"') result.push_back('\\');
		result.push_back(c);
	}
	result.push_back('"');
	return result;
}

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
		std::fprintf(stderr, "html-entities: error: cannot open %s\n", u8ptr);
		return 1;
	}

	pred(of);
	return 0;
}

int main(int argc, char const** argv) {
	if (argc < 3) {
		std::fprintf(stderr, "entities <json-file> <cpp-dir>\n");
		return 1;
	}

	auto entities = read(argv[1]);
	auto items = json::cast<json::object>(entities);
	if (!strcmp(argv[1], "-")) argv[1] = "standard input";
	if (!items) {
		std::fprintf(stderr,
		             "html-entities: error: expecting %s to describe a jason "
		             "dictionary.\n",
		             argv[1]);
		return 1;
	}

	lookup_tree db;
	for (auto& [name, item_jsn] : *items) {
		auto characters =
		    json::cast_from_json<std::string>(item_jsn, "characters");
		if (!characters) {
			std::fprintf(stderr,
			             "html-entities: error: expecting items to be "
			             "dictionary with a string named \"characters\".\n");
			return 1;
		}

		if (name.empty() || name.back() != ';') continue;

		auto view = std::string_view{name};
		view = view.substr(1);
		if (!view.empty() && view.back() == ';')
			view = view.substr(0, view.size() - 1);

		db.insert(view, name, printable(*characters));
	}

	db = db.gather_endings();

	auto result = write(argv[2], [&](std::ostream& out) {
		out << autogen_hdr << R"(#include <tangle/browser/entities.hpp>

using namespace std::literals;

namespace tangle::browser {
)";

		db.function(out, "char const*", "entity", "nullptr", sensitivity::on);

		out << R"(
}  // namespace tangle
// clang-format on
)";
	});
	if (result) return result;

	return write(argv[3], [&](std::ostream& out) {
		out << autogen_hdr << R"(#include <tangle/browser/entities.hpp>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tangle::browser::testing {
)";

		db.tests(out, "html_named_entities", "char const*", "entity", "nullptr",
		         true);

		out << R"(}  // namespace tangle::browser::testing
// clang-format on
)";
	});
}
