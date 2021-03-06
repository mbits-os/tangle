// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

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

#include <tangle/cookie/item.hpp>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>

/**
Cookie jar I/O code.
*/
namespace tangle::cookie::io {
	/// Version-independent mnemonics
	enum {
		TAG_FILE = 0x52414A43,  ///< First four bytes of stored files: CJAR
	};

	/**
	Version 1.0 of cookie I/O.

	The file starts with `CJAR` tag, followed by version tag for I/O v1.0.

	The data header record contains count of version 1 item structures, followed
	by size of item structure itself. This allows for any version 1.x to be
	backwards-compatible with 1.0.

	The count and size is followed by offsets to item list block and to the
	string buffer.
	 */
	inline namespace v1 {
		/// Mnemonics for version 1.0
		enum {
			VERSION = 0x00010000  ///< Version tag for v1.0 I/O structure
		};

		/**
		Single cookie item as described in CJAR file.

		The strings are offsets into string buffer, the time stamps are 64 bit
		integers broken into high and lo 32-bit words.
		 */
		struct item {
			std::uint32_t name;   ///< When decoded, represents cookie's name
			std::uint32_t value;  ///< When decoded, represents cookie's value
			std::uint32_t
			    domain;  ///< When decoded, represents cookie scope's domain
			std::uint32_t
			    path;  ///< When decoded, represents cookie scope's path
			std::uint32_t flags;  ///< Numeric representation of cookie's flags
			std::uint32_t expires_lo;  ///< Low half of expires time-stamp
			std::uint32_t expires_hi;  ///< High half of expires time-stamp
			std::uint32_t created_lo;  ///< Low half of created time-stamp
			std::uint32_t created_hi;  ///< High half of created time-stamp
			std::uint32_t access_lo;   ///< Low half of access time-stamp
			std::uint32_t access_hi;   ///< High half of access time-stamp
		};

		/**
		Version 1.0 data header.

		Immediately follows file header, describing any 1.x basic information:
		how many items there are in the jar file, what is the size of single
		item, where the items are, where the strings used by the items are in
		the file.
		 */
		struct data_header {
			/**
			Number of cookie items in CJAR file.
			*/
			std::uint32_t count = 0;

			/**
			Size of single item in the file.

			For version 1.0 defaults to size of the v1::item, but can be
			anything larger for other versions.
			 */
			std::uint32_t item = sizeof(v1::item);

			/**
			Offset of the items block, counting from beginning of the data
			header.

			For version 1.0 it defaults to size of the v1::data_header itself,
			but can be something larger for later version.
			 */
			std::uint32_t items = sizeof(data_header);

			/**
			Offset of strings block, counting from beginning of the data header.

			For version 1.0 it defaults to size of the v1::data_header itself,
			plus count times size of v1::item (or, to be exact, whatever item
			should hold)
			 */
			std::uint32_t strings = 0;
		};
	}  // namespace v1

	/**
	File header for CJAR file

	Defines the magic tag for the file and version of binary syntax used by the
	file.
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

		Currently set to v1.0, but due to the current version being inline
		namespace, will use whatever version is latest at the moment.

		\see v1::VERSION
		 */
		std::uint32_t version = VERSION;
	};

	/**
	Meta-type helper for all I/O specifications.

	\tparam IO the type of the I/O class. Should be in form of a pointer (e.g.
	`FILE*`) or reference (e.g. `iostream&`)

	To be sure all APIs needed by the store_raw and load_raw are provided,
	inherit from io_base. The APIs needed by the io_base are: writeraw for
	output and readraw with skipraw for input.

	\see v1::store_raw
	\see v1::load_raw
	\see io_impl<FILE*>
	 */
	template <typename IO>
	struct io_impl;

	/**
	Meta-type for I/O code independent from any actual I/O.

	\tparam Final a curiously recurring template providing writeraw et al.
	\tparam IO the type of the I/O class. Should be in form of a pointer (e.g.
	`FILE*`) or reference (e.g. `iostream&`)
	 */
	template <typename Final, typename IO>
	struct io_base {
		/**
		Writes a POD or a simple type into writable.
		\tparam T the type of the POD/simple argument; should be deduced from
		the call \param writable the I/O object \param obj the POD/simple type
		data to be written into the writable \return true, if entire obj was
		written
		 */
		template <typename T>
		static bool write(IO writable, const T& obj) {
			return Final::writeraw(writable, &obj, sizeof(T)) == sizeof(T);
		}

		/**
		Writes a string into writable.
		\param writable the I/O object
		\param s string to write
		\return true, if entire string was written, including terminating NIL
		*/
		static bool writestr(IO writable, const std::string& s) {
			if (s.empty()) {
				// empty string is not a non-string, but a ""
				char zero = 0;
				return Final::write(writable, zero);
			}

			return Final::writeraw(writable, s.c_str(), s.length() + 1) ==
			       (s.length() + 1);
		}

		/**
		Reads a POD or a simple type from readable.
		\tparam T the type of the POD/simple argument; should be deduced from
		        the call
		\param readable the I/O object
		\param obj the POD/simple type data to be read from readable
		\return true, if entire obj was read
		*/
		template <typename T>
		static bool read(IO readable, T& obj) {
			return Final::readraw(readable, &obj, sizeof(T)) == sizeof(T);
		}

		/**
		Skips some bytes in readable.
		\param readable the I/O object
		\param length the number of bytes to skip
		\return true, if entire chunk was skipped
		*/
		static bool skip(IO readable, size_t length) {
			if (!length) return true;
			return Final::skipraw(readable, length) == length;
		}
	};

	/**
	Specification of io_impl for stdio's FILE handle.

	When using, `writable` should be opened with "wb", while `readable` should
	be opened with "rb".
	\see io_impl
	 */
	template <>
	struct io_impl<FILE*> : io_base<io_impl<FILE*>, FILE*> {
		/**
		Performs actual writing into the file.

		Called by io_base::write and io_base::writestr, forwards the call to
		`fwrite`.
		\param writable the FILE* object opened with "wb", or an analogue.
		\param buffer pointer to a memory required to be read
		\param length number of bytes to be written from the buffer
		\return number of bytes actually written into the FILE*
		 */
		static size_t writeraw(FILE* writable,
		                       const void* buffer,
		                       size_t length) {
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
		static size_t readraw(FILE* readable, void* buffer, size_t length) {
			return fread(buffer, 1, length, readable);
		}

		/**
		Performs actual skipping in the file.

		Called by io_base::skip, forwards the call to `fseek`.
		\param readable the FILE* object opened with "rb", or an analogue.
		\param length number of bytes to be skipped
		\return number of bytes skipped, as reported by calls to `ftell` before
		and after the `fseek`.
		 */
		static size_t skipraw(FILE* readable, size_t length) {
			auto told = static_cast<size_t>(ftell(readable));
			if constexpr (sizeof(size_t) >= sizeof(long)) {
				static constexpr auto max_long =
				    std::numeric_limits<long>::max();
				static constexpr auto max_long_sz =
				    static_cast<size_t>(max_long);
				while (max_long_sz < length) {
					// GCOV_EXCL_START -- it is rather hard to create a file
					// satisfying this amounts
					fseek(readable, max_long, SEEK_CUR);
					length -= max_long_sz;
				}
				// GCOV_EXCL_STOP -- MSVC also marks closing brace as part of
				// the block
				fseek(readable, static_cast<long>(length), SEEK_CUR);
			} else {
				fseek(readable, static_cast<long>(length), SEEK_CUR);
			}
			auto newpos = static_cast<size_t>(ftell(readable));
			return newpos - told;
		}
	};

	inline static void timestamp(uint32_t& lo, uint32_t& hi, uint64_t time) {
		hi = static_cast<uint32_t>((time >> 32) & 0xFFFF'FFFFul);
		lo = (time)&0xFFFF'FFFFul;
	}

	inline static void timestamp(uint32_t& lo,
	                             uint32_t& hi,
	                             cookie::time_point const& time) {
		timestamp(lo, hi,
		          static_cast<uint64_t>(time.time_since_epoch().count()));
	}

	inline static clock::time_point make_timepoint(uint64_t lo, uint64_t hi) {
		lo &= 0xFFFF'FFFFul;
		hi &= 0xFFFF'FFFFul;
		hi <<= 32;
		hi |= lo;
		clock::duration from_epoch{hi};
		return clock::time_point{from_epoch};
	}

	inline namespace v1 {
		/**
		Keeps all the strings in all the cookies to be written into a writeble.
		*/
		struct string_bag {
			std::map<std::string, uint32_t> strings{{{}, 0}};
			uint32_t length = 1;

			/**
			Adds a single string to this string bag. If the string is missing
			from this object, it is addded and the offset of that string is
			returned. Otherwise, offset of previous string is returned.

			\param value the string to add to this bag
			\return offset of the string
			*/
			uint32_t add(std::string const& value);
			/**
			Adds all the strings from a cookie and returns offsets to them.

			\param item the cookie object, whose strings should be added to this
			            bag.
			\return a new I/O cookie with strings set to offsets from this
			        bag.
			*/
			io::item add(cookie::item const& item);

			/**
			Stores all the strings from this bag into a writeable.

			\tparam IO the type of the I/O class. Should be in form of a pointer
			(e.g. `FILE*`) or reference (e.g. `iostream&`)

			\param writable the I/O object

			\return true, if the strings structure was written into I/O
			*/
			template <typename IO>
			inline bool store_raw(IO writable) {
				if (!io::io_impl<IO>::write(writable, length)) return false;

				for (auto const& [_, value] : sorted_strings()) {
					if (!io::io_impl<IO>::writestr(writable, value))
						return false;
				}
				return true;
			}

		private:
#ifndef USING_DOXYGEN
			using reversed = std::pair<uint32_t, std::string>;
			std::vector<reversed> sorted_strings();
#endif
		};

		/**
		Stores qualified cookies from the jar into the writable I/O object.

		Enumerates the persistent cookies from the jar vector and if the cookie
		has not yet expired (as compared to `when` parameter), it is stored into
		the writable.

		\tparam IO the type of the I/O class. Should be in form of a pointer
		(e.g. `FILE*`) or reference (e.g. `iostream&`)

		\param writable the I/O object
		\param jar a list of cookies to be scanned and stored into the writable
		\param when point in time representing current moment; used to determine
		expired cookies, which should not be stored

		\return true, if the entire v1.0 structure was written into I/O
		 */
		template <typename IO>
		inline bool store_raw(IO writable,
		                      const std::vector<cookie::item>& jar,
		                      time_point when = clock::now()) {
			io::data_header header{};

			for (auto& item : jar) {
				if ((item.flags() & flags::persistent) == flags::persistent &&
				    (item.expires() > when))
					++header.count;
			}

			header.strings = header.items + header.count * header.item;

			if (!io::io_impl<IO>::write(writable,
			                            io::file_header{}))  // CJAR v1.0
				return false;
			if (!io::io_impl<IO>::write(writable, header)) return false;

			string_bag bag{};

			for (auto& item : jar) {
				if ((item.flags() & flags::persistent) != flags::persistent ||
				    (item.expires() <= when))
					continue;

				io::item out = bag.add(item);
				out.flags = static_cast<uint32_t>(item.flags());
				timestamp(out.expires_lo, out.expires_hi, item.expires());
				timestamp(out.created_lo, out.created_hi, item.creation_time());
				timestamp(out.access_lo, out.access_hi,
				          item.last_access_time());
				if (!io::io_impl<IO>::write(writable, out)) return false;
			}

			return bag.store_raw<IO>(writable);
		}

		/**
		Loads qualified cookies from the readable I/O object into a jar.

		Parses and decodes the contents of persistent cookies and if the cookie
		has not yet expired (as compared to `when` parameter), it is added to
		the jar.

		Should be able to read any 1.x file format, as any extension to version
		1 of syntax should only expand on information already stored.

		\tparam IO the type of the I/O class. Should be in form of a pointer
		(e.g. `FILE*`) or reference (e.g. `iostream&`)

		\param readable the I/O object
		\param jar a list of cookies to be filled with cookies from the readable
		\param when point in time representing current moment; used to determine
		expired cookies, which should not be restored

		\return true, if the entire v1.0 structure was read from I/O
		 */
		template <typename IO>
		inline bool load_raw(IO readable,
		                     std::vector<cookie::item>& jar,
		                     time_point when = clock::now()) {
			io::file_header file{};
			if (!io::io_impl<IO>::read(readable, file)) return false;

			if (file.tag != io::TAG_FILE) return false;

#ifdef V1_ONLY
			if (file.version != io::v1::VERSION) return false;
#else
			if ((file.version & 0xFFFF0000) != io::v1::VERSION)
				return false;  // support every 1.x as if 1.0
#endif

			io::v1::data_header header{};
			if (!io::io_impl<IO>::read(readable, header)) return false;

			if (header.items < sizeof(io::v1::data_header)) return false;
			if (header.item < sizeof(io::v1::item)) return false;

			auto strings = header.items + header.count * header.item;
			if (strings > header.strings) return false;

			if (!io::io_impl<IO>::skip(
			        readable, header.items - sizeof(io::v1::data_header)))
				return false;

			std::vector<io::v1::item> items;
			items.reserve(header.count);
			for (uint32_t i = 0; i < header.count; ++i) {
				io::v1::item next{};
				if (!io::io_impl<IO>::read(readable, next)) return false;
				if (!io::io_impl<IO>::skip(readable,
				                           header.item - sizeof(io::v1::item)))
					return false;

				items.push_back(next);
			}

			uint32_t str_length = 0;
			if (!io::io_impl<IO>::skip(readable, header.strings - strings))
				return false;
			if (!io::io_impl<IO>::read(readable, str_length)) return false;

			for (auto& raw : items) {
				if (raw.name >= str_length) return false;
				if (raw.value >= str_length) return false;
				if (raw.domain >= str_length) return false;
				if (raw.path >= str_length) return false;
			}

			auto buffer = std::make_unique<char[]>(str_length);
			if (io::io_impl<IO>::readraw(readable, buffer.get(), str_length) !=
			    str_length)
				return false;
			if (buffer[str_length - 1] != 0) return false;

			std::vector<cookie::item> out;
			out.reserve(items.size());
			for (auto& raw : items) {
				cookie::item item{
				    buffer.get() + raw.name,
				    buffer.get() + raw.value,
				    {buffer.get() + raw.domain, buffer.get() + raw.path},
				    static_cast<cookie::flags>(raw.flags),
				    make_timepoint(raw.expires_lo, raw.expires_hi)};
				item.creation_time(
				    make_timepoint(raw.created_lo, raw.created_hi));
				item.last_access_time(
				    make_timepoint(raw.access_lo, raw.access_hi));

				if ((item.flags() & flags::persistent) !=
				    flags::persistent)  // how did non-persistent cookie find
				                        // its way here?
					return false;

				if (item.expires() <= when) continue;  // already expired

				out.push_back(std::move(item));
			}

			std::swap(jar, out);
			return true;
		}
	}  // namespace v1
}  // namespace tangle::cookie::io
