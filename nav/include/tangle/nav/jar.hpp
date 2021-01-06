// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

/**
Navigation cookie jar.
\file
\author Marcin Zdun <mzdun@midnightbits.com>
*/

#pragma once
#include <tangle/cookie/jar.hpp>
#include <tangle/uri.hpp>

namespace tangle::nav {

	/**
	Navigation cookie jar.

	Helper class ensuring a jar is stored and loaded.
	*/
	class jar {
	public:
		void path(const std::string& path);
		const std::string& path() const { return m_path; }
		void store(cookie::time_point when = cookie::clock::now()) const;
		bool append(const uri& address,
		            std::string_view header,
		            cookie::time_point created = cookie::clock::now());
		std::string get(const uri& address,
		                bool https,
		                cookie::time_point when = cookie::clock::now());

	private:
		std::string m_path;
		cookie::jar m_cookies;
	};
}  // namespace tangle::nav
