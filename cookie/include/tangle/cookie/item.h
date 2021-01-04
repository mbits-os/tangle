// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Cookie code.
\file
\author Marcin Zdun <mzdun@midnightbits.com>

This class implements the cookie::item type, a structure containing
the description of a single cookie.
*/

#pragma once

#include <tangle/cookie/chrono.h>
#include <tangle/cookie/scope.h>
#include <string>
#include <string_view>
#include <vector>

/**
 * \namespace tangle::cookie
 * \brief Cookie item and cookie jar code
 */
namespace tangle::cookie {
	/**
	Bit flags for defining type of cookie.
	*/
	enum class flags {
		/**
		value representing "no flag is set".
		*/
		none = 0x00,
		/**
		flag set only for `Expires` and `Max-Age` cookies.
		*/
		persistent = 0x01,
		/**
		set for cookies without `Domain` attribute.
		*/
		host_only = 0x02,
		/**
		set for cookies with `Secure` attribute.
		*/
		secure = 0x04,
		/**
		set for cookies with `HttpOnly` attribute.
		*/
		http_only = 0x08,
	};

#ifndef USING_DOXYGEN
	inline flags operator|(flags lhs, flags rhs) {
		return flags((int)lhs | (int)rhs);
	}
	inline flags operator&(flags lhs, flags rhs) {
		return flags((int)lhs & (int)rhs);
	}
	// for cleaning bits:
	inline flags operator~(flags lhs) { return flags(~(int)lhs); }
#endif  // USING_DOXYGEN

	/**
	Bit flags for item::matches.
	*/
	enum class match {
		/**
		value representing "no flag is set".
		*/
		none = 0x00,
		/**
		when set, denotes that the item::matches is called for HTTPS-type
		communication.
		*/
		secure = 0x01,
		/**
		when set, denotes that the item::matches is called for network
		communication (as opposed to local solution, such as bridging into JS).
		*/
		http = 0x02,
	};

#ifndef USING_DOXYGEN
	// hide the boring details from the doygen
	inline match operator|(match lhs, match rhs) {
		return match((int)lhs | (int)rhs);
	}
	inline match operator&(match lhs, match rhs) {
		return match((int)lhs & (int)rhs);
	}
	inline match& operator|=(match& lhs, match rhs) {
		return (match&)((int&)lhs |= (int)rhs);
	}
	inline match& operator&=(match& lhs, match rhs) {
		return (match&)((int&)lhs &= (int)rhs);
	}
	// for cleaning bits:
	inline match operator~(match lhs) { return match(~(int)lhs); }
#endif  // USING_DOXYGEN

	/**
	The item class defines a single cookie. It contains the scope, name and
	value of a cookie, together with flags and optional `Expires` time point.
	*/
	class item {
	public:
		/**
		Constructs a new item from the smallest amount of information, bigger
		than the default constructor.
		\param name the name of the constructed cookie.
		\param value the value of the constructed cookie.
		\param scope the domain and path the constructed cookie is created for.
		*/
		item(const std::string& name,
		     const std::string& value,
		     const scope_type& scope);

		/**
		Constructs a new item from cookie name, value, scope, flags and optional
		expires. For the expires to be actually used, the flags must contain
		flags::persistent flag.
		\param name the name of the constructed cookie.
		\param value the value of the constructed cookie.
		\param scope the domain and path the constructed cookie is created for.
		\param flags the flags describing the constructed cookie.
		\param expires the point int time, when the persistent cookie should no
		               longer be taken into consideration.
		*/
		item(const std::string& name,
		     const std::string& value,
		     const scope_type& scope,
		     flags flags,
		     time_point expires = clock::now());

		/**
		Constructs a new item from cookie name, value, scope, flags, expires and
		created time points. For the expires to be actually used, the flags must
		contain flags::persistent flag.
		\param name the name of the constructed cookie.
		\param value the value of the constructed cookie.
		\param scope the domain and path the constructed cookie is created for.
		\param flags the flags describing the constructed cookie.
		\param expires the point int time, when the persistent cookie should no
		               longer be taken into consideration.
		\param created the point int time representing the creation of the
		               cookie.
		*/
		item(const std::string& name,
		     const std::string& value,
		     const scope_type& scope,
		     flags flags,
		     time_point expires,
		     time_point created);

		/**
		Constructs a new empty item, with no name, no value, empty scope and
		flags::none flags.
		*/
		item();

		/**
		Copy-constructs an item.
		*/
		item(const item&);

		/**
		Move-constructs an item.
		*/
		item(item&&);

		/**
		Copy-assigns an item to this item.
		*/
		item& operator=(const item&);

		/**
		Move-assigns an item to this item.
		*/
		item& operator=(item&&);

		/**
		Getter for flags property. Will return all flags set.
		\returns the value of flags
		*/
		cookie::flags flags() const { return m_flags; }

		/**
		Setter for flags property. Will set internal state to value, but will
		retain the old value of flags::persistent, as it can only be set from
		constructor or expires(time_point) and cleared from clean_expires().
		\param value a new value to set
		*/
		void flags(cookie::flags value) {
			m_flags = (m_flags & cookie::flags::persistent) |
			          (value & ~cookie::flags::persistent);
		}

		/**
		Immutable getter for the scope property.
		\returns the current scope
		*/
		const scope_type& scope() const { return m_scope; }

		/**
		Setter for the scope property.
		\param value a new value to set
		*/
		void scope(const scope_type& value) { m_scope = value; }

		/**
		Getter for the name property.
		\returns the value of name
		*/
		const std::string& name() const { return m_name; }

		/**
		Setter for the name property.
		\param value a new value to set
		*/
		void name(const std::string& value) { m_name = value; }

		/**
		Getter for the value property.
		\returns the value of cookie
		*/
		const std::string& value() const { return m_value; }

		/**
		Setter for the name property.
		\param value a new value to set
		*/
		void value(const std::string& value) { m_value = value; }

		/**
		Getter for the expires property. Will return any value currently kept,
		whether flags::persistent is set or not.
		\returns the value of expires
		*/
		time_point expires() const { return m_expires; }

		/**
		Setter for the expires property. Calling this function will also set the
		flags::persistent flag.
		\param value a new value to set
		*/
		void expires(time_point value) {
			m_expires = value;
			m_flags = m_flags | cookie::flags::persistent;
		}

		/**
		Cleaner for the expires property. Calling this function will also clean
		the flags::persistent flag.
		*/
		void clean_expires() {
			m_expires = time_point();
			m_flags = m_flags & ~cookie::flags::persistent;
		}

		/**
		Shorthand testing both presence of the flags::persistent and the value
		expires() property.
		\param when point in time, against which the test for being expired
		            should be performed.
		\returns true if expires property is both set and in the past
		*/
		bool has_expired(time_point when = clock::now()) const {
			return ((m_flags & cookie::flags::persistent) ==
			        cookie::flags::persistent) &&
			       (m_expires <= when);
		}

		/**
		Getter for the creation_time property. If not explicitly set,
		initialized to clock::now().
		\returns the value of creation_time
		*/
		time_point creation_time() const { return m_creation_time; }

		/**
		Setter for the creation_time property. If not explicitly set,
		initialized to clock::now().
		\param value a new value to set
		*/
		void creation_time(time_point value) { m_creation_time = value; }

		/**
		Getter for the last_access_time property. Set to creation_time on
		original creation and then updated during each jar::str() to indicate
		this cookie was selected for sending back.
		\returns the value of last_access_time
		*/
		time_point last_access_time() const { return m_last_access_time; }

		/**
		Setter for the last_access_time property. Set to creation_time on
		original creation and then updated during each jar::str() to indicate
		this cookie was selected for sending back.
		\param value a new value to set
		*/
		void last_access_time(time_point value) { m_last_access_time = value; }

		/**
		Check if the cookie should be sent with a request. It uses the time
		point to see, if the cookie is expired or not. It uses the upstream
		scope to see, if the cookie's scope matches the request. Finally, it
		uses options to see, if the cookies flags match the request.
		\param upstream the domain-path pair for the new request; the domain
		                should be set to `Host` and the path should be set to
		                directory of the resource in the request.
		\param options The type of request being made. The network-bound
		               requests should set the match::http option, the secured
		               request should have the match::secure options set. _This
		               means HTTPS network request should have __both__ of the
		               options set._
		\param when time point to use, when determining if the persistent cookie
		            has expired or not.
		\returns true, if the cookie should be sent
		\see RFC6265, section 5.1.3, Domain Matching
		\see RFC6265, section 5.1.4, Paths and Path-Match
		\see scope_type::matches
		*/
		bool matches(const scope_type& upstream,
		             match options = match::none,
		             time_point when = clock::now()) const;

		/**
		Produces string compatible with `Set-Cookie` header. If the
		`prefer_maxage` is set and the cookie is persistent, it will use `when`
		as a time point to calculate time difference to expires time point.

		* The `Domain` attribute is added, if the scope's domain is not empty
		and the flags::host_only is not set.
		* The `Path` attribute is added, if the scope's path is not empty.
		* The `Expires` or `MaxAge` attribute is added, if the flags::persistent
		is set, depending on prefer_maxage.
		* The `Secure` and/or `HttpOnly` attributes are added, if the
		flags::secure and/or flags::http_only is set, respectively.

		\param prefer_maxage Allows to decide, whether `MaxAge` or `Expires`
		                     should be used.
		\param when If the cookie is persistent, and the
		            `MaxAge` is preferred, what is the "now" age should be
		            calculated against.
		\returns a string representation of the cookie for a
		         `Set-Cookie` header
		*/
		std::string server_string(bool prefer_maxage = false,
		                          time_point when = clock::now()) const;

	private:
		cookie::flags m_flags = cookie::flags::none;
		scope_type m_scope;
		std::string m_name;
		std::string m_value;
		time_point m_expires;
		time_point m_creation_time{clock::now()};
		time_point m_last_access_time{m_creation_time};
	};

	/**
	Will parse a `Cookie` header extracting name/value pairs.
	\param origin a domain name to be used for the `{origin, "/"}` scope for new
	              cookies.
	\param header a character span representing a `Cookie` header.
	\returns a vector of all cookie name/value pairs found in the header.
	*/
	std::vector<item> from_client(const std::string& origin,
	                              std::string_view header);

	/**
	Will parse a `Set-Cookie` header extracting a cookie definition.
	\param origin a domain name to be used together with the flags::host_only
	              when the `Domain` attribute is missing.
	\param header a character span representing a `Set-Cookie` header.
	\param created a point in time used for `MaxAge` calculations and for the
	               creation_time and last_access_time properties.
	\returns a cookie as defined by the header.
	*/
	item from_server(const std::string& origin,
	                 std::string_view header,
	                 time_point created = clock::now());
}  // namespace tangle::cookie
