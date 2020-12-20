// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/http/client.hh>

namespace tangle::http::client {
	static constexpr char base64_chars[] =
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	    "abcdefghijklmnopqrstuvwxyz"
	    "0123456789+/";

	static std::string base64_encode(std::string const& in) {
		std::string out;
		out.reserve(((in.size() + 3) << 2) >> 2);

		int val = 0, valb = -6;
		for (auto c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back(base64_chars[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6)
			out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4)
			out.push_back('=');
		return out;
	}

	request& request::set_basic_auth(std::string const& username,
	                                 std::string const& secret) {
		std::string auth;
		auth.reserve(username.size() + secret.size() + 1);
		auth.append(username);
		auth.push_back(':');
		auth.append(secret);
		auth = base64_encode(auth);

		return set(header::Authorization, "Basic " + auth);
	}
}