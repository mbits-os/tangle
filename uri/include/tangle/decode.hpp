// Copyright (c) 2015 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>
#include <string_view>

namespace tangle {
	enum class codec { common, host, path, query };

	/**
	Prepares the input range to be safely used as a part
	of URL.

	Converts all characters to their hex counterparts (%%XX),
	except for the unreserved ones: letters, digits,
	<code>"-"</code>, <code>"."</code>, <code>"_"</code>,
	<code>"~"</code>. This function, together with urldecode,
	can be used for URI normalization.

	\param in address of buffer to encode, out of which
	          <code>in_len</code> must be readable
	\param in_len the length of the buffer
	\param alg encoding algorithm to use
	\return percent-encoded string containing no reserved
	        characters, except for the percent sign.

	\see RFC3986, section 2.1, Percent-Encoding
	\see RFC3986, section 2.3, Unreserved Characters
	\see urldecode(const char*,size_t)
	*/
	std::string urlencode(const char* in,
	                      size_t in_len,
	                      codec alg = codec::common);

	/**
	Removes all percent-encodings from the input range.

	Converts all encoded octets in form of %%XX to their
	raw form.

	\param in address of buffer to decode, out of which
	          <code>in_len</code> must be readable
	\param in_len the length of the buffer
	\param alg decoding algorithm to use
	\return percent-decoded string, which may include some
	        registered characters.

	\see RFC3986, section 2.1, Percent-Encoding
	\see tangle::urlencode(const char*,size_t)
	*/
	std::string urldecode(const char* in,
	                      size_t in_len,
	                      codec alg = codec::common);

	/**
	Prepares the input string to be safely used as a part
	of URL.

	A version of urlencode(const char*, size_t) taking an
	STL string as input range.

	\param in an STL string to encode
	\param alg encoding algorithm to use
	\return percent-encoded string containing no reserved
	        characters, except for the percent sign.

	\see urlencode(const char*, size_t)
	*/
	inline std::string urlencode(const std::string& in,
	                             codec alg = codec::common) {
		return urlencode(in.c_str(), in.length(), alg);
	}

	/**
	Removes all percent-encodings from the input range.

	A version of urldecode(const char*, size_t) taking an
	STL string as input range.

	\param in an STL string to decode
	\param alg decoding algorithm to use
	\return percent-decoded string, which may include some
	        registered characters.

	\see urldecode(const char*, size_t)
	*/
	inline std::string urldecode(const std::string& in,
	                             codec alg = codec::common) {
		return urldecode(in.c_str(), in.length(), alg);
	}

	/**
	Prepares the input string to be safely used as a part
	of URL.

	A version of urlencode(const char*, size_t) taking a
	std::string_view as input range.

	\param in a std::string_view to encode
	\param alg encoding algorithm to use
	\return percent-encoded string containing no reserved
	        characters, except for the percent sign.

	\see urlencode(const char*, size_t)
	*/
	inline std::string urlencode(std::string_view in,
	                             codec alg = codec::common) {
		return urlencode(in.data(), in.length(), alg);
	}

	/**
	Removes all percent-encodings from the input range.

	A version of urldecode(const char*, size_t) taking a
	std::string_view as input range.

	\param in a std::string_view to decode
	\param alg decoding algorithm to use
	\return percent-decoded string, which may include some
	        registered characters.

	\see urldecode(const char*, size_t)
	*/
	inline std::string urldecode(std::string_view in,
	                             codec alg = codec::common) {
		return urldecode(in.data(), in.length(), alg);
	}
}  // namespace tangle
