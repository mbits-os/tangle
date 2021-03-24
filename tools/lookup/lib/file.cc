// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "file.hh"

file::ptr file::open(char const* path, char const* mode) {
	return ptr{fopen(path, mode)};
}

buffered_file::buffered_file(FILE* buffered_file)
    : file::ptr{buffered_file}, buffer_(10u * 1024 * 1024, '\0') {}

buffered_file buffered_file::open(char const* path, char const* mode) {
	return buffered_file{fopen(path, mode)};
}

bool buffered_file::readline(std::string& out) {
	out.clear();
	if (pos_ == capacity_ && eof_) return false;
	while (true) {
		auto prev = pos_;
		auto ptr = buffer_.data();
		while (pos_ < capacity_ && ptr[pos_] != '\n')
			++pos_;
		if (pos_ != prev) out.append(ptr + prev, pos_ - prev);
		if (pos_ < capacity_) {
			++pos_;
			return true;
		}

		underflow();
		if (eof_) return false;
	}
}

void buffered_file::underflow() {
	if (eof_) return;
	auto len = std::fread(buffer_.data(), 1, buffer_.size(), get());

	pos_ = 0;
	capacity_ = len;

	if (!len) eof_ = true;
}
