/*
 * Copyright (C) 2016 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>
#include <tangle/msg/hasher.hpp>
#include <initializer_list>

namespace {
	struct hash_info {
		uint32_t hash32;
		uint64_t hash64;
		const char* msg;
	};

	template <size_t sizeof_size_t> struct select;
	template <> struct select<4> {
		static auto info(const hash_info& nfo) { return nfo.hash32; }
	};
	template <> struct select<8> {
		static auto info(const hash_info& nfo) { return nfo.hash64; }
	};

	using selector = select<sizeof(size_t)>;

	std::ostream& operator<<(std::ostream& o, const hash_info& nfo) {
		return o << selector::info(nfo) << ", \"" << nfo.msg << "\"";
	}

	struct partial_info {
		const char* msg;
		std::initializer_list<const char*> parts;
	};

	std::ostream& operator<<(std::ostream& o, const partial_info& nfo) {
		o << "hash(\"" << nfo.msg << "\") vs hash({";
		bool first = true;
		for (auto const& part : nfo.parts) {
			if (first) first = false;
			else o << ", ";
			o << '"' << part << '"';
		}
		return o << "})";
	}

	class hasher : public ::testing::TestWithParam<hash_info> {
	};

	class hasher_parts : public ::testing::TestWithParam<partial_info> {
	};

	TEST_P(hasher, calc)
	{
		auto& par = GetParam();
		auto msg = par.msg;
		auto expected = selector::info(par);

		auto actual = tangle::msg::hasher::hash(msg, strlen(msg));
		ASSERT_EQ(expected, actual);
	}

	TEST_P(hasher_parts, calc)
	{
		using tangle::msg::hasher;
		auto& par = GetParam();
		auto msg = par.msg;
		auto expected = hasher::hash(msg, strlen(msg));

		hasher combined { };
		for (auto part : par.parts)
			combined.append(part, strlen(part));
		ASSERT_EQ(expected, combined.value());
	}

	static hash_info hashing[] = {
		{ 2166136261U, 14695981039346656037ULL, "" },
		{ 3509953955U,  8954484210020690467ULL, "Lorem ipsum dolor sit amet, consectetur adipiscing elit." },
		{ 2904501379U, 13847790973317505091ULL, "lorem ipsum dolor sit amet, consectetur adipiscing elit." },
		{ 4119201121U,   497309184426307681ULL, "larem ipsum dolor sit amet, consectetur adipiscing elit." },
		{ 1735399955U, 16566894959707560787ULL, "larem ipsum dolor sit amet, consectetur adipiscing" }
	};

	static partial_info partial[] = {
		{ "", { } },
		{ "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", { "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "" } },
		{ "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", { "Lorem ", "ipsum ", "dolor ", "sit ", "amet, ", "consectetur ", "adipiscing ", "elit." } },
	};

	INSTANTIATE_TEST_CASE_P(hashing, hasher, ::testing::ValuesIn(hashing));
	INSTANTIATE_TEST_CASE_P(partial, hasher_parts, ::testing::ValuesIn(partial));
}
