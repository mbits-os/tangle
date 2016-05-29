#pragma once

#include <net/cookie/jar.h>

namespace net { namespace cookie { namespace io {
	struct data {
		size_t length;
		size_t ptr;
		const uint8_t* bytes;
	};

	template <> struct io_impl<std::vector<uint8_t>&> : io_base<io_impl<std::vector<uint8_t>&>, std::vector<uint8_t>&> {
		static size_t writeraw(std::vector<uint8_t>& writable, const void* buffer, size_t length)
		{
			if (!buffer)
				return 0;

			auto ptr = (const uint8_t*)buffer;
			writable.insert(std::end(writable), ptr, ptr + length);
			return length;
		}
	};

	template <> struct io_impl<data*> : io_base<io_impl<data*>, data*> {
		static size_t readraw(data* readable, void* buffer, size_t length)
		{
			if (!buffer)
				return 0;

			auto rest = readable->length - readable->ptr;
			if (rest < length)
				length = rest;
			auto src = readable->bytes + readable->ptr;
			readable->ptr += length;
			if (length)
				memcpy(buffer, src, length);
			return length;
		}

		static size_t skipraw(data* readable, size_t length)
		{
			auto rest = readable->length - readable->ptr;
			if (rest < length)
				length = rest;
			readable->ptr += length;
			return length;
		}
	};
}}}
