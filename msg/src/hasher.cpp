// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/msg/hasher.h>
#include <algorithm>

namespace tangle::msg {

	namespace consts {
		template <size_t sizeof_size_t>
		struct fnv_const;
		template <>
		struct fnv_const<4> {
			static constexpr uint32_t offset = 2166136261U;
			static constexpr uint32_t prime = 16777619U;
		};

		template <>
		struct fnv_const<8> {
			static constexpr uint64_t offset = 14695981039346656037ULL;
			static constexpr uint64_t prime = 1099511628211ULL;
		};
	};  // namespace consts

	hasher::hasher() : m_value{consts::fnv_const<sizeof(size_t)>::offset} {}

	hasher& hasher::append(const void* buffer, size_t length) {
		constexpr auto prime = consts::fnv_const<sizeof(size_t)>::prime;

		for (auto data = (const char*)buffer; length; ++data, --length) {
			m_value ^= (size_t)*data;
			m_value *= prime;
		}
		return *this;
	}

	size_t hasher::hash(const char* s, size_t l) {
		return hasher{}.append(s, l).value();
	}
}  // namespace tangle::msg
