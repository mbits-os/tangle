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
 *
 */

/**
Cookie jar I/O code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
\version 1.0

This file contains code for I/O for cookie jars.
Top level functions, tangle::cookie::io::store_raw and
tangle::cookie::io::store_raw, allow to write binary
files for long-term persistent cookies for any given
I/O.
*/

#pragma once

#include <tangle/cookie/item.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

namespace tangle { namespace cookie {

	/**
	Cookie jar I/O code.
	*/
	namespace io {
		/// Version-independent mnemonics
		enum {
			TAG_FILE = 0x52414A43, ///< First four bytes of stored files: CJAR
		};

		/**
		Version 1.0 of cookie I/O.

		The file starts with `CJAR` tag, followed by version
		tag for I/O v1.0.

		The data header record contains count of version 1 item
		structures, followed by size of item structure itself.
		This allows for any version 1.x to be backwards-compatible
		with 1.0.

		The count and size is followed by offsets to item list block
		and to the string buffer.
		 */
		inline namespace v1 {
			/// Mnemonics for version 1.0
			enum {
				VERSION = 0x00010000 ///< Version tag for v1.0 I/O structure
			};

			/**
			Single cookie item as described in CJAR file.

			The strings are offsets into string buffer, the time stamps are
			64 bit integers broken into high and lo 32 words.
			 */
			struct item {
				std::uint32_t name;       ///< When decoded, represents cookie's name
				std::uint32_t value;      ///< When decoded, represents cookie's value
				std::uint32_t domain;     ///< When decoded, represents cookie scope's domain
				std::uint32_t path;       ///< When decoded, represents cookie scope's path
				std::uint32_t flags;      ///< Numeric representation of cookie's flags
				std::uint32_t expires_lo; ///< Low half of expires time-stamp
				std::uint32_t expires_hi; ///< High half of expires time-stamp
				std::uint32_t created_lo; ///< Low half of created time-stamp
				std::uint32_t created_hi; ///< High half of created time-stamp
				std::uint32_t access_lo;  ///< Low half of access time-stamp
				std::uint32_t access_hi;  ///< High half of access time-stamp
			};

			/**
			Version 1.0 data header.

			Immediately follows file header, describing any 1.x
			basic information: how many items there are in the jar
			file, what is the size of single item, where the items are,
			where the strings used by the items are in the file.
			 */
			struct data_header {
				/**
				Number of cookie items in CJAR file.
				*/
				std::uint32_t count = 0;

				/**
				Size of single item in the file.

				For version 1.0 defaults to size of the v1::item, but
				can be anything larger for other versions.
				 */
				std::uint32_t item = sizeof(v1::item);

				/**
				Offset of the items block, counting from beginning of the data header.

				For version 1.0 it defaults to size of the v1::data_header itself,
				but can be something larger for later version.
				 */
				std::uint32_t items = sizeof(data_header);

				/**
				Offset of strings block, counting from beginning of the data header.

				For version 1.0 it defaults to size of the v1::data_header itself,
				plus count times size of v1::item (or, to be exact, whatever item should hold)
				 */
				std::uint32_t strings = 0;
			};
		};

		/**
		File header for CJAR file

		Defines the magic tag for the file and version of binary syntax used by the file.
		 */
		struct file_header {
			/**
			The magic number for the file.

			Contains bytes for "CJAR".
			\see TAG_FILE
			 */
			std::uint32_t tag = TAG_FILE;
			/**
			The version fo the file structure.

			Currently set to v1.0, but due to the current version being inline namespace,
			will use whatever version is latest at the moment.
			\see v1::VERSION
			 */
			std::uint32_t version = VERSION;
		};

		/**
		Meta-type helper for all I/O specifications.

		\tparam IO the type of the I/O class. Should be in form of a pointer (e.g. `FILE*`) or reference (e.g. `iostream&`)

		To be sure all APIs needed by the store_raw and load_raw are
		provided, inherit from io_base. The APIs needed by the io_base
		are: writeraw for output and readraw with skipraw for input.
		\see v1::store_raw
		\see v1::load_raw
		\see io_impl<FILE*>
		 */
		template <typename IO>
		struct io_impl;

		/**
		Meta-type for I/O code independent from any actual I/O.

		\tparam Final a curiously recurring template providing writeraw et al.
		\tparam IO the type of the I/O class. Should be in form of a pointer (e.g. `FILE*`) or reference (e.g. `iostream&`)
		 */
		template <typename Final, typename IO>
		struct io_base {

			/**
			Writes a POD or a simple type into writable.
			\tparam T the type of the POD/simple argument; should be deduced from the call
			\param writable the I/O object
			\param obj the POD/simple type data to be written into the writable
			\return true, if entire obj was written
			 */
			template <typename T>
			static bool write(IO writable, const T& obj)
			{
				return Final::writeraw(writable, &obj, sizeof(T)) == sizeof(T);
			}

			/**
			Writes a C-style string into writable.
			\param writable the I/O object
			\param s pointer to a C-style string
			\return true, if s in nullptr or entire string was written, including terminating NIL
			*/
			static bool writestr(IO writable, const char* s)
			{
				if (!s) {
					// not writing non-string is, well, exactly the side-effect we would expect
					return true;
				}

				auto len = std::strlen(s) + 1;
				return Final::writeraw(writable, s, len) == len;
			}

			/**
			Writes a string into writable.
			\param writable the I/O object
			\param s string to write
			\return true, if entire string was written, including terminating NIL
			*/
			static bool writestr(IO writable, const std::string& s)
			{
				if (s.empty()) {
					// empty string is not a non-string, but a ""
					char zero = 0;
					return Final::write(writable, zero);
				}

				return Final::writeraw(writable, s.c_str(), s.length() + 1) == (s.length() + 1);
			}

			/**
			Reads a POD or a simple type from readable.
			\tparam T the type of the POD/simple argument; should be deduced from the call
			\param readable the I/O object
			\param obj the POD/simple type data to be read from readable
			\return true, if entire obj was read
			*/
			template <typename T>
			static bool read(IO readable, T& obj)
			{
				return Final::readraw(readable, &obj, sizeof(T)) == sizeof(T);
			}

			/**
			Skips some bytes in readable.
			\param readable the I/O object
			\param length the number of bytes to skip
			\return true, if entire chunk was skipped
			*/
			static bool skip(IO readable, size_t length)
			{
				if (!length)
					return true;
				return Final::skipraw(readable, length) == length;
			}
		};

		/**
		Specification of io_impl for stdio's FILE handle.

		When using, `writable` should be opened with "wb", while
		`readable` should be opened with "rb".
		\see io_impl
		 */
		template <> struct io_impl<FILE*> : io_base<io_impl<FILE*>, FILE*> {

			/**
			Performs actual writing into the file.

			Called by io_base::write and io_base::writestr, forwards the call to `fwrite`.
			\param writable the FILE* object opened with "wb", or an analogue.
			\param buffer pointer to a memory required to be read
			\param length number of bytes to be written from the buffer
			\return number of bytes actually written into the FILE*
			 */
			static size_t writeraw(FILE* writable, const void* buffer, size_t length)
			{
				return fwrite(buffer, 1, length, writable);
			}

			/**
			Performs actual reading from the file.

			Called by io_base::read, forwards the call to `fread`.
			\param readable the FILE* object opened with "rb", or an analogue.
			\param buffer pointer to a memory used as output
			\param length number of bytes to be written into the buffer
			\return number of bytes actually read from the FILE*
			 */
			static size_t readraw(FILE* readable, void* buffer, size_t length)
			{
				return fread(buffer, 1, length, readable);
			}

			/**
			Performs actual skipping in the file.

			Called by io_base::skip, forwards the call to `fseek`.
			\param readable the FILE* object opened with "rb", or an analogue.
			\param length number of bytes to be skipped
			\return number of bytes skipped, as reported by calls to `ftell` before and after the `fseek`.
			 */
			static size_t skipraw(FILE* readable, size_t length)
			{
				auto told = ftell(readable);
				fseek(readable, length, SEEK_CUR);
				auto newpos = ftell(readable);
				return newpos - told;
			}
		};

		inline static void timestamp(uint32_t& lo, uint32_t& hi, uint64_t time)
		{
			hi = (time >> 32) & 0xFFFF'FFFFul;
			lo = (time) & 0xFFFF'FFFFul;
		}

		inline static clock::time_point make_timepoint(uint64_t lo, uint64_t hi)
		{
			lo &= 0xFFFF'FFFFul;
			hi &= 0xFFFF'FFFFul;
			hi <<= 32;
			hi |= lo;
			clock::duration from_epoch { hi };
			return clock::time_point { from_epoch };
		}

		inline namespace v1 {
			/**
			Stores qualified cookies from the jar into the writable I/O object.

			Enumerates the persistent cookies from the jar vector and if the cookie
			has not yet expired (as compared to `when` parameter), it is stored into
			the writable.

			\tparam IO the type of the I/O class. Should be in form of a pointer (e.g. `FILE*`) or reference (e.g. `iostream&`)

			\param writable the I/O object
			\param jar a list of cookies to be scanned and stored into the writable
			\param when point in time representing current moment; used to determine expired cookies, which should not be stored

			\return true, if the entire v1.0 structure was written into I/O
			 */
			template <typename IO>
			inline bool store_raw(IO writable, const std::vector<cookie::item>& jar, time_point when = clock::now())
			{
				io::data_header header { };

				for (auto& item : jar) {
					if ((item.flags() & flags::persistent) == flags::persistent && (item.expires() > when))
						++header.count;
				}

				header.strings = header.items + header.count * header.item;

				if (!io::io_impl<IO>::write(writable, io::file_header { })) // CJAR v1.0
					return false;
				if (!io::io_impl<IO>::write(writable, header))
					return false;

				uint32_t strings = 1;
				for (auto& item : jar) {
					if ((item.flags() & flags::persistent) != flags::persistent || (item.expires() <= when))
						continue;

					io::item out { };
					out.name = strings; strings += item.name().length() + 1;
					out.value = strings; strings += item.value().length() + 1;
					out.domain = strings; strings += item.scope().domain().length() + 1;
					out.path = strings; strings += item.scope().path().length() + 1;
					out.flags = (int32_t)item.flags();
					timestamp(out.expires_lo, out.expires_hi, item.expires().time_since_epoch().count());
					timestamp(out.created_lo, out.created_hi, item.creation_time().time_since_epoch().count());
					timestamp(out.access_lo, out.access_hi, item.last_access_time().time_since_epoch().count());
					if (!io::io_impl<IO>::write(writable, out))
						return false;
				}

				if (!io::io_impl<IO>::write(writable, strings))
					return false;

				if (!io::io_impl<IO>::writestr(writable, ""))
					return false;

				for (auto& item : jar) {
					if ((item.flags() & flags::persistent) != flags::persistent || (item.expires() <= when))
						continue;

					if (!io::io_impl<IO>::writestr(writable, item.name()))
						return false;
					if (!io::io_impl<IO>::writestr(writable, item.value()))
						return false;
					if (!io::io_impl<IO>::writestr(writable, item.scope().domain()))
						return false;
					if (!io::io_impl<IO>::writestr(writable, item.scope().path()))
						return false;
				}

				return true;
			}

			/**
			Loads qualified cookies from the readable I/O object into a jar.

			Parses and decodes the contents of persistent cookies and if the cookie
			has not yet expired (as compared to `when` parameter), it is added to
			the jar.

			Should be able to read any 1.x file format, as any extension to version 1
			of syntax should only expand on information already stored.

			\tparam IO the type of the I/O class. Should be in form of a pointer (e.g. `FILE*`) or reference (e.g. `iostream&`)

			\param readable the I/O object
			\param jar a list of cookies to be filled with cookies from the readable
			\param when point in time representing current moment; used to determine expired cookies, which should not be restored

			\return true, if the entire v1.0 structure was read from I/O
			 */
			template <typename IO>
			inline bool load_raw(IO readable, std::vector<cookie::item>& jar, time_point when = clock::now())
			{
				io::file_header file { };
				if (!io::io_impl<IO>::read(readable, file))
					return false;

				if (file.tag != io::TAG_FILE)
					return false;

#ifdef V1_ONLY
				if (file.version != io::v1::VERSION)
					return false;
#else
				if ((file.version & 0xFFFF0000) != io::v1::VERSION)
					return false; // support every 1.x as if 1.0
#endif

				io::v1::data_header header { };
				if (!io::io_impl<IO>::read(readable, header))
					return false;

				if (header.items < sizeof(io::v1::data_header))
					return false;
				if (header.item < sizeof(io::v1::item))
					return false;

				auto strings = header.items + header.count * header.item;
				if (strings > header.strings)
					return false;

				if (!io::io_impl<IO>::skip(readable, header.items - sizeof(io::v1::data_header)))
					return false;

				std::vector<io::v1::item> items;
				items.reserve(header.count);
				for (uint32_t i = 0; i < header.count; ++i) {
					io::v1::item next { };
					if (!io::io_impl<IO>::read(readable, next))
						return false;
					if (!io::io_impl<IO>::skip(readable, header.item - sizeof(io::v1::item)))
						return false;

					items.push_back(next);
				}

				uint32_t str_length = 0;
				if (!io::io_impl<IO>::skip(readable, header.strings - strings))
					return false;
				if (!io::io_impl<IO>::read(readable, str_length))
					return false;

				for (auto& raw : items) {
					if (raw.name >= str_length)
						return false;
					if (raw.value >= str_length)
						return false;
					if (raw.domain >= str_length)
						return false;
					if (raw.path >= str_length)
						return false;
				}

				auto buffer = std::make_unique<char[]>(str_length);
				if (io::io_impl<IO>::readraw(readable, buffer.get(), str_length) != str_length)
					return false;
				if (buffer[str_length - 1] != 0)
					return false;

				std::vector<cookie::item> out;
				out.reserve(items.size());
				for (auto& raw : items) {
					cookie::item item {
						buffer.get() + raw.name,
						buffer.get() + raw.value,
						{ buffer.get() + raw.domain, buffer.get() + raw.path },
						(cookie::flags)raw.flags,
						make_timepoint(raw.expires_lo, raw.expires_hi)
					};
					item.creation_time(make_timepoint(raw.created_lo, raw.created_hi));
					item.last_access_time(make_timepoint(raw.access_lo, raw.access_hi));

					if ((item.flags() & flags::persistent) != flags::persistent) // how did non-persistent cookie find its way here?
						return false;

					if (item.expires() <= when)
						continue; // already expired

					out.push_back(std::move(item));
				}

				std::swap(jar, out);
				return true;
			}
		};
	}
}}
