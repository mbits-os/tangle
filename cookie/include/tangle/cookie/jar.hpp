// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cookie jar code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

This file contains code for cookie jars. It represents the client-side container
for HTTP cookies.
*/

#pragma once

#include <tangle/cookie/io.hpp>
#include <tangle/cookie/item.hpp>

namespace tangle::cookie {

	/**
	 Cookie Jar.

	 This class represents the client-side container for HTTP cookies. It
	 contains list of cookies attached to a browsing session, with ability to
	 add new cookies from `Set-Cookie` and to generate cookie list for
	 `Cookies`.
	 */
	class jar {
	public:
		/**
		The type of the cookie container. Currently, a std::vector of cookies.
		*/
		using container = std::vector<item>;

		/**
		The type of the iterator for container operations.
		*/
		using const_iterator = container::const_iterator;

		/**
		 Constructs an new, empty client-side cookie container.
		 */
		jar() = default;

		/**
		 Constructs an new client-side cookie container with contains of
		 initializer_list.
		 */
		jar(const std::initializer_list<item>& items) : m_items(items) {}

		/**
		 Constructs an new client-side cookie container with contains of
		 container.
		 */
		jar(const container& items) : m_items(items) {}

		/**
		 Constructs an new client-side cookie container with container's
		 contents.
		 */
		jar(container&& items) : m_items(std::move(items)) {}

		/**
		 Returns an iterator to the first cookie in the container.
		 */
		const_iterator begin() const { return m_items.begin(); }

		/**
		 Returns an iterator to the element following the last cookie in the
		 container.
		 */
		const_iterator end() const { return m_items.end(); }

		/**
		 Returns a reference to the cookie at specified location pos, with
		 bounds checking.
		 */
		const item& at(size_t id) const { return m_items.at(id); }

		/**
		 Checks if the container has no cookies.
		 */
		bool empty() const { return m_items.empty(); }

		/**
		 Returns the number of cookies in the container.
		 */
		size_t size() const { return m_items.size(); }

		/**
		 Locates a cookie with given name, within given scope. Finds a cookie
		 identified by the name, domain and path given. Linear.

		 \param name the name of the cookie
		 \param scope the domain and path of the cookie
		 \return Iterator pointing to a searched cookie, or end() if none found.
		 */
		const_iterator find(const std::string& name,
		                    const scope_type& scope) const;

		/**
		Adds new cookie into pre-existing set of cookies.

		If the cookie with the same name and scope does not yet exist, the new
		cookie is added to the list. Otherwise, if the `for_http` is false and
		existing cookie is `HttpOnly`, the new one is ignored. Otherwise, the
		new cookie replaces the existing one.

		The test for expired cookie, which would warrant removal instead of
		replacement, is not performed until either str() or store().

		\param cookie a new cookie to add
		\param for_http if set to true, represents cookie from HTTP, must be
		                false, when representing a cookie created off-wire.
		*/
		void add(const item& cookie, bool for_http = true);

		/**
		 Adds new cookie into pre-existing set of cookies.

		 If the cookie with the same name and scope does not yet exist, the new
		 cookie is added to the list. Otherwise, if the `for_http` is false and
		 existing cookie is `HttpOnly`, the new one is ignored. Otherwise, the
		 new cookie replaces the existing one.

		 The test for expired cookie, which would warrant removal instead of
		 replacement, is not performed until either str() or store().

		 \param cookie a new cookie to add
		 \param for_http if set to true, represents cookie from HTTP, must be
		                 false, when representing a cookie created off-wire.
		 */
		void add(item&& cookie, bool for_http = true);

		/**
		 Shorthand for add(cookie::from_server()). For the description of the
		 parameters, see tangle::cookie::from_server().

		 \param origin a domain name to be used when the `Domain` is missing.
		 \param header a character span representing a `Set-Cookie` header.
		 \param created a point in time used for `MaxAge` calculations.

		 \see tangle::cookie::from_server()
		 \see add()
		 */
		void from_server(const std::string& origin,
		                 std::string_view header,
		                 time_point created = clock::now()) {
			add(tangle::cookie::from_server(origin, header, created));
		}

		/**
		 Creates a string ready to be used as contents of Cookie header. Builds
		 a `name=value` list for given scope. The cookies expired as of `when`
		 are not taken into account. `Options` argument is used to tweak the
		 visibility of the cookies. For more information, see item::matches.

		 \param uri the domain-path pair for the new request
		 \param options the type of request being made
		 \param when time point to use, when determining if the persistent
		             cookie has expired or not
		 \return a string representing all matching cookies in the jar
		 \see item::matches
		 */
		std::string str(const scope_type& uri,
		                match options,
		                time_point when = clock::now());

		/**
		Removes all cookies, which has expired as of `when`.

		\param when point in time representing current moment.
		*/
		void prune(time_point when = clock::now());

		/**
		Stores qualified cookies from the jar into the writable I/O object.

		Enumerates the persistent cookies from the jar vector and if the cookie
		has not yet expired (as compared to `when` parameter), it is stored into
		the writable.

		\tparam IO the type of the I/O class, in reference form
		        (e.g. `iostream&`)

		\param writable the I/O object
		\param when point in time representing current moment; used to determine
		            expired cookies, which should not be stored
		\return true, if the entire v1.0 structure was written into I/O

		\see io::v1::store_raw
		*/
		template <typename IO>
		bool store_raw(IO& writable, time_point when = clock::now()) const {
			return io::store_raw<IO&>(writable, m_items, when);
		}

		/**
		Stores qualified cookies from the jar into the writable I/O object.

		Enumerates the persistent cookies from the jar vector and if the cookie
		has not yet expired (as compared to `when` parameter), it is stored into
		the writable.

		\tparam IO the type of the I/O class, in pointer form (e.g. `FILE*`)

		\param writable the I/O object
		\param when point in time representing current moment; used to determine
		            expired cookies, which should not be stored
		\return true, if the entire v1.0 structure was written into I/O

		\see io::v1::store_raw
		*/
		template <typename IO>
		bool store_raw(IO* writable, time_point when = clock::now()) const {
			return io::store_raw<IO*>(writable, m_items, when);
		}

		/**
		Loads qualified cookies from the readable I/O object into a jar.

		Parses and decodes the contents of persistent cookies and if the cookie
		has not yet expired (as compared to `when` parameter), it is added to
		the jar.

		\tparam IO the type of the I/O class, in reference form
		       (e.g. `iostream&`)

		\param readable the I/O object
		\param when point in time representing current moment; used to determine
		            expired cookies, which should not be restored
		\return true, if the entire v1.0 structure was read from I/O

		\see io::v1::load_raw
		*/
		template <typename IO>
		bool load_raw(IO& readable, time_point when = clock::now()) {
			return io::load_raw<IO&>(readable, m_items, when);
		}

		/**
		Loads qualified cookies from the readable I/O object into a jar.

		Parses and decodes the contents of persistent cookies and if the cookie
		has not yet expired (as compared to `when` parameter), it is added to
		the jar.

		\tparam IO the type of the I/O class, in pointer form (e.g. `FILE*`)

		\param readable the I/O object
		\param when point in time representing current moment; used to determine
		            expired cookies, which should not be restored
		\return true, if the entire v1.0 structure was read from I/O

		\see io::v1::load_raw
		*/
		template <typename IO>
		bool load_raw(IO* readable, time_point when = clock::now()) {
			return io::load_raw<IO*>(readable, m_items, when);
		}

		/**
		Stores qualified cookies from the jar in the file pointed by the path.

		\param path the path of the file cookies should be stored to
		\param when point in time representing current moment
		\return true, if the file could be open for writing and if underlying
		        store_raw() returned true.

		\see store_raw()
		*/
		bool store(const std::string& path,
		           time_point when = clock::now()) const;

		/**
		Loads qualified cookies from the file pointed by the path.

		\param path the path of the file cookies should be loaded from
		\param when point in time representing current moment
		\return true, if the file could be open for reading and if underlying
		        load_raw() returned true.

		\see load_raw()
		*/
		bool load(const std::string& path, time_point when = clock::now());

	private:
		container m_items;

		template <typename Forwarded>
		void add_impl(Forwarded&& cookie, bool for_http);
	};
}  // namespace tangle::cookie
