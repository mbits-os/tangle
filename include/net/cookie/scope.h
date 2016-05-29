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

/**
Cookie scope code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

The key used to identify any given cookie is its name
together with a domain-path pair. This file defines the
this pair as scope_type.
*/

#pragma once

#include <string>

namespace net { namespace cookie {

	/**
	The string pair for domain and path. This type is extracted from cookie::item
	to keep responsibility for `domain-match` and `path-match`
	algorithms.
	*/
	class scope_type {
	public:
		/**
		Constructs a new scope_type from a given domain and path.
		\param domain the name of the domain
		\param path the name of the path
		*/
		scope_type(const std::string& domain, const std::string& path);

		/**
		Constructs a new scope_type with empty domain and path.
		*/
		scope_type();

		/**
		Copy-constructs a new scope_type from a given scope.
		\param other already existing scope to use as base of construction
		*/
		scope_type(const scope_type& other);

		/**
		Move-constructs a new scope_type from a given scope.
		\param other expiring scope to use as base of construction
		*/
		scope_type(scope_type&& other);

		/**
		Copy-assigns given scope to this scope_type.
		\param other already existing scope to use as source of data
		*/
		scope_type& operator=(const scope_type& other);

		/**
		Move-assigns given scope to this scope_type.
		\param other expiring scope to use as source of data
		*/
		scope_type& operator=(scope_type&& other);

		/**
		Constructs a new scope_type from a given path, with empty domain.
		*/
		static scope_type from_path(const std::string& path) { return { {}, path }; }

		/**
		The equal-comparison operator.
		
		Two scopes are equal, if both domain and path are equal, string-wise.
		No canonization is performed at this point.
		*/
		bool operator == (const scope_type& rhs) const
		{
			return
				m_domain == rhs.m_domain &&
				m_path == rhs.m_path;
		}

		/**
		The less-than operator.

		One scope is less than the other, if the domains are not equal and
		it's domain is less than other's domain, string-wise. If the domains
		are equal string-wise, it is less than the other, if it's path is less
		then other's path. No canonization is performed at this point.
		*/
		bool operator < (const scope_type& rhs) const
		{
			auto result = m_domain.compare(rhs.m_domain);
			if (!result)
				return m_path < rhs.m_path;
			return result < 0;
		}

		/**
		Getter for the domain property. The domain should either be contents
		of `Domain` cookie attribute, or the domain from which the cookie
		originated.
		*/
		const std::string& domain() const { return m_domain; }

		/**
		Setter for the domain property. The domain should either be contents
		of `Domain` cookie attribute, or the domain from which the cookie
		originated.
		*/
		void domain(const std::string& value) { m_domain = value; }

		/**
		Getter for the path property. The path represents the resource
		directory this cookie belongs to and should either be contents
		of `Path` cookie attribute, or the root path (`/`).
		*/
		const std::string& path() const { return m_path; }

		/**
		Setter for the path property. The path represents the resource
		directory this cookie belongs to and should either be contents
		of `Path` cookie attribute, or the root path (`/`).
		*/
		void path(const std::string& value) { m_path = value; }

		/**
		Implementation of `domain-match` and `path-match`.
		It decides whether or not the cookie should be sent with a request.
		\param upstream the domain-path pair for the new request; the domain
		                should be set to `Host` and the path should be set
						to directory of the resource in the request.
		\param host_only If set, this scope represent a `host-only` cookie.
		\see RFC6265, section 5.1.3, Domain Matching
		\see RFC6265, section 5.1.4, Paths and Path-Match
		*/
		bool matches(const scope_type& upstream, bool host_only) const
		{
			return matches_domain(upstream, host_only)
				&& matches_path(upstream);
		}
	private:
		std::string m_domain;
		std::string m_path;

		bool matches_domain(const scope_type& upstream, bool host_only) const;
		bool matches_path(const scope_type& upstream) const;
	};
}}
