// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/cookie/jar.hpp>

namespace tangle::cookie::io {
	struct data_span {
		size_t length{};
		uint8_t const* bytes{};
		constexpr data_span() = default;
		template <size_t Length>
		constexpr data_span(uint8_t const (&arr)[Length])
		    : length{Length}, bytes{arr} {}
		constexpr data_span(uint8_t const* data, size_t length)
		    : length{length}, bytes{data} {}
		constexpr data_span(data_span const& span) = default;
		constexpr data_span(data_span&& span) = default;
		data_span(std::vector<uint8_t> const& span)
		    : length{span.size()}, bytes{span.data()} {}
		constexpr data_span& operator=(data_span const& span) = default;
		constexpr data_span& operator=(data_span&& span) = default;
		constexpr data_span subspan(size_t offset, size_t new_length) const {
			if (offset > length) offset = length;
			auto const rest = length - offset;
			if (new_length > rest) new_length = rest;
			return {bytes + offset, new_length};
		}
		constexpr bool operator==(data_span const& rhs) const noexcept {
			return (length == rhs.length) &&
			       (std::memcmp(bytes, rhs.bytes, length) == 0);
		};
	};

	struct data : data_span {
		size_t ptr{};
		using data_span::data_span;
		data(data_span const& span) : data_span(span) {}
	};

	struct limited_vector {
		size_t max_size;
		std::vector<uint8_t> buffer{};
	};

	template <>
	struct io_impl<std::vector<uint8_t>&>
	    : io_base<io_impl<std::vector<uint8_t>&>, std::vector<uint8_t>&> {
		static size_t writeraw(std::vector<uint8_t>& writable,
		                       const void* buffer,
		                       size_t length) {
			if (!buffer) return 0;

			auto ptr = (const uint8_t*)buffer;
			writable.insert(std::end(writable), ptr, ptr + length);
			return length;
		}
	};

	template <>
	struct io_impl<limited_vector&>
	    : io_base<io_impl<limited_vector&>, limited_vector&> {
		static size_t writeraw(limited_vector& writable,
		                       const void* buffer,
		                       size_t length) {
			if (!buffer) return 0;

			auto ptr = (const uint8_t*)buffer;
			auto const rest = writable.max_size - writable.buffer.size();
			auto const to_write = length > rest ? rest : length;

			writable.buffer.insert(std::end(writable.buffer), ptr,
			                       ptr + to_write);
			return to_write;
		}
	};

	template <>
	struct io_impl<data*> : io_base<io_impl<data*>, data*> {
		static size_t readraw(data* readable, void* buffer, size_t length) {
			if (!buffer) return 0;

			auto rest = readable->length - readable->ptr;
			if (rest < length) length = rest;
			auto src = readable->bytes + readable->ptr;
			readable->ptr += length;
			if (length) memcpy(buffer, src, length);
			return length;
		}

		static size_t skipraw(data* readable, size_t length) {
			auto rest = readable->length - readable->ptr;
			if (rest < length) length = rest;
			readable->ptr += length;
			return length;
		}
	};

	enum {
		sizeof_item = 44,
		data_offset = 16,
	};

}  // namespace tangle::cookie::io

#define U32(VAL)                                    \
	static_cast<uint8_t>((VAL)&0xFF),               \
	    static_cast<uint8_t>(((VAL) >> 8) & 0xFF),  \
	    static_cast<uint8_t>(((VAL) >> 16) & 0xFF), \
	    static_cast<uint8_t>(((VAL) >> 24) & 0xFF)

#define NULL_BYTES 0, 0, 0, 0

#define FILE_HEADER(MAJOR, MINOR)                          \
	U32(io::TAG_FILE), static_cast<uint8_t>((MINOR)&0xFF), \
	    static_cast<uint8_t>(((MINOR) >> 8) & 0xFF),       \
	    static_cast<uint8_t>((MAJOR)&0xFF),                \
	    static_cast<uint8_t>(((MAJOR) >> 8) & 0xFF)

#define V1_HEADER(ITEMS, SIZEOF_ITEM, DATA_OFFSET, STRINGS_PADDING) \
	U32(ITEMS), U32(SIZEOF_ITEM), U32(DATA_OFFSET),                 \
	    U32((DATA_OFFSET) + (ITEMS) * (SIZEOF_ITEM) + (STRINGS_PADDING))

#define V1_ITEM_STR(NAME, VALUE, DOMAIN, PATH) \
	U32(NAME), U32(VALUE), U32(DOMAIN), U32(PATH)
#define V1_ITEM_FLAGS U32
#define V1_ITEM_TIMESTAMP(HI, LO) U32(LO), U32(HI)
