// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cookie scope code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

The key used to identify any given cookie is its name together with a
domain-path pair. This file defines the this pair as scope_type.
*/

#pragma once

#include <string>

namespace tangle::cookie {

	/**
	The string pair for domain and path. This type is extracted from
	cookie::item to keep responsibility for `domain-match` and `path-match`
	algorithms.
	*/
	class scope_type {
	public:
		/**
		The domain property, which should either be contents of `Domain` cookie
		attribute, or the domain from which the cookie originated.
		*/
		std::string domain;

		/**
		The path property, wihch represents the resource directory this cookie
		belongs to and should either be contents of `Path` cookie attribute, or
		the root path (`/`).
		*/
		std::string path;

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
		\returns updated reference
		*/
		scope_type& operator=(const scope_type& other);

		/**
		Move-assigns given scope to this scope_type.
		\param other expiring scope to use as source of data
		\returns updated reference
		*/
		scope_type& operator=(scope_type&& other);

		/**
		Constructs a new scope_type from a given path, with empty domain.
		*/
		static scope_type from_path(const std::string& path) {
			return {{}, path};
		}

		/**
		The equal-comparison operator.

		Two scopes are equal, if both domain and path are equal, string-wise. No
		canonization is performed at this point.

		\param rhs a value to compare against
		\returns true, if the both objects are equal
		*/
		bool operator==(const scope_type& rhs) const {
			return domain == rhs.domain && path == rhs.path;
		}

		/**
		The less-than operator.

		One scope is less than the other, if the domains are not equal and it's
		domain is less than other's domain, string-wise. If the domains are
		equal string-wise, it is less than the other, if it's path is less then
		other's path. No canonization is performed at this point.

		\param rhs a value to compare against
		\returns true, if the left-hand side is lower, than the right-hand side
		*/
		bool operator<(const scope_type& rhs) const {
			auto result = domain.compare(rhs.domain);
			if (!result) return path < rhs.path;
			return result < 0;
		}

		/**
		Implementation of `domain-match` and `path-match`. It decides whether or
		not the cookie should be sent with a request.

		\param upstream the domain-path pair for the new request; the domain
		                should be set to `Host` and the path should be set to
		                directory of the resource in the request.
		\param host_only If set, this scope represent a `host-only` cookie.
		\returns true, if the `upstream` both `domain-matches` and
		         `path-matches` this scope
		\see RFC6265, section 5.1.3, Domain Matching
		\see RFC6265, section 5.1.4, Paths and Path-Match
		*/
		bool matches(const scope_type& upstream, bool host_only) const {
			return matches_domain(upstream, host_only) &&
			       matches_path(upstream);
		}

	private:
		bool matches_domain(const scope_type& upstream, bool host_only) const;
		bool matches_path(const scope_type& upstream) const;
	};
}  // namespace tangle::cookie
