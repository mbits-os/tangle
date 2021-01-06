// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <initializer_list>
#include <tangle/msg/hasher.hpp>

namespace {
	struct hash_info {
		uint32_t hash32;
		uint64_t hash64;
		const char* msg;
	};

	template <size_t sizeof_size_t>
	struct select;
	template <>
	struct select<4> {
		static auto info(const hash_info& nfo) { return nfo.hash32; }
	};
	template <>
	struct select<8> {
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
			if (first)
				first = false;
			else
				o << ", ";
			o << '"' << part << '"';
		}
		return o << "})";
	}

	class hasher : public ::testing::TestWithParam<hash_info> {};

	class hasher_parts : public ::testing::TestWithParam<partial_info> {};

	TEST_P(hasher, calc) {
		auto& par = GetParam();
		auto msg = par.msg;
		auto expected = selector::info(par);

		auto actual = tangle::msg::hasher::hash(msg, strlen(msg));
		ASSERT_EQ(expected, actual);
	}

	TEST_P(hasher_parts, calc) {
		using tangle::msg::hasher;
		auto& par = GetParam();
		auto msg = par.msg;
		auto expected = hasher::hash(msg, strlen(msg));

		hasher combined{};
		for (auto part : par.parts)
			combined.append(part, strlen(part));
		ASSERT_EQ(expected, combined.value());
	}

	static hash_info hashing[] = {
	    {2166136261U, 14695981039346656037ULL, ""},
	    {
	        3509953955U,
	        8954484210020690467ULL,
	        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	    },
	    {
	        2904501379U,
	        13847790973317505091ULL,
	        "lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	    },
	    {
	        4119201121U,
	        497309184426307681ULL,
	        "larem ipsum dolor sit amet, consectetur adipiscing elit.",
	    },
	    {
	        1735399955U,
	        16566894959707560787ULL,
	        "larem ipsum dolor sit amet, consectetur adipiscing",
	    },
	};

	static partial_info partial[] = {
	    {
	        "",
	        {},
	    },
	    {
	        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	        {
	            "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	            "",
	        },
	    },
	    {
	        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	        {
	            "Lorem ",
	            "ipsum ",
	            "dolor ",
	            "sit ",
	            "amet, ",
	            "consectetur ",
	            "adipiscing ",
	            "elit.",
	        },
	    },
	};

	INSTANTIATE_TEST_CASE_P(hashing, hasher, ::testing::ValuesIn(hashing));
	INSTANTIATE_TEST_CASE_P(partial,
	                        hasher_parts,
	                        ::testing::ValuesIn(partial));
}  // namespace
