// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <vector>

namespace fs = std::filesystem;

struct file_closer {
	void operator()(std::FILE* f) { std::fclose(f); }
};

struct file {
	using ptr = std::unique_ptr<FILE, file_closer>;
	static ptr open(char const* path, char const* mode = "r");
};

class buffered_file : file::ptr {
public:
	static buffered_file open(char const* path, char const* mode = "r");
	FILE* handle() const noexcept { return get(); }
	bool is_open() const { return !!*this; }
	bool readline(std::string& out);

private:
	buffered_file(FILE* file);
	void underflow();

	std::vector<char> buffer_;
	size_t capacity_{};
	size_t pos_{};
	bool eof_{false};
};
