[![Build Status](https://travis-ci.org/mbits-os/tangle.svg?branch=master)](https://travis-ci.org/mbits-os/tangle)
[![Coverage Status](https://coveralls.io/repos/github/mbits-os/tangle/badge.svg?branch=master)](https://coveralls.io/github/mbits-os/tangle?branch=master)

# Lego bricks for C++ programs.
## Low level, simple, loosely coupled libraries

1. **LOCALE**: `str::format` and binary translation files
2. **URI**: parsing and manipulating strings representing resource
3. **COOKIE**: parsing, manipulating cookies with support for session jar and persistent jar
4. **MSG**: HTTP/1.1 request and response message, independent from both URI and COOKIE
5. **CERT**: platform-specific and/or standalone certificate verification; should support both SSL/TLS and code signing verification.
6. **NAV**: browser sessions, building requests based on current address and given uri, cookie jar attached to the session, certificate verification and communication flagging, no actual communication over the network
7. **HTTP**: connection layer for libnav, probably over libcurl.

## Status:

* LOCALE:
	* code: done ([v0.2](https://github.com/mbits-os/locale/tree/v0.2))
	* docs: _missing_
* URI:
	* code: done ([v1.0](https://github.com/mbits-os/tangle/tree/uri-1.0))
	* norm: [RFC 3986:  Uniform Resource Identifier (URI): Generic Syntax](https://tools.ietf.org/html/rfc3986)
* COOKIE:
	* code: done ([v1.0](https://github.com/mbits-os/tangle/tree/cookie-1.0))
	* norm: [RFC 6265: HTTP State Management Mechanism](https://tools.ietf.org/html/rfc6265)
* MSG:
	* code: done ([v0.1](https://github.com/mbits-os/tangle/tree/msg-0.1))
	* docs: _missing_
	* norm: [RFC 2616: Hypertext Transfer Protocol -- HTTP/1.1](https://tools.ietf.org/html/rfc2616)
* CERT
	* design: _missing_ (based on OpenSSL?)
* NAV:
	* design: _missing_
* HTTP:
	* design: _missing_
