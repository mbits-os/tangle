// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/fetch.hpp>

namespace tangle::browser {
	fetch_interface::~fetch_interface() = default;

	simple_fetch::simple_fetch(nav::navigator const& nav) : nav{nav} {}

	nav::document simple_fetch::fetch(nav::request const& req) {
		return nav.open(modify(req));
	}

	nav::request simple_fetch::modify(nav::request const& req) { return req; }

	basic_auth_fetch::basic_auth_fetch(std::string&& username,
	                                   std::string&& secret,
	                                   nav::navigator const& nav)
	    : simple_fetch{nav}
	    , username_{std::move(username)}
	    , secret_{std::move(secret)} {}

	nav::request basic_auth_fetch::modify(nav::request const& req) {
		return nav::request{req}.basic_auth(username_, secret_);
	}
}  // namespace tangle::browser
