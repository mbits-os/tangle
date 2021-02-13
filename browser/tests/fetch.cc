// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/browser/fetch.hpp>
#include <tangle/nav/doc_impl.hpp>
#include <tangle/nav/document.hpp>
#include <tangle/nav/protocol.hpp>
#include <unordered_set>

using namespace std::literals;

namespace tangle::browser::testing {
	class fetch_proto_mock : public nav::protocol {
	public:
		nav::document open(const nav::request& req,
		                   nav::navigator& nav) override {
			auto const address = req.address();

			msg::http_response::dict_t headers{};

			auto auth = req.headers().find(nav::header::Authorization);
			if (auth != req.headers().end())
				headers["Authorization"] = auth->second;

			auto doc = std::make_shared<nav::doc_impl>(address, &nav);
			doc->on_headers("OK"s, 200, headers);
			return nav::document::wrap(doc);
		}
	};

	class test_basic_auth_fetch : public basic_auth_fetch {
	public:
		using basic_auth_fetch::basic_auth_fetch;
		nav::request modify(nav::request const& req) final {
			return basic_auth_fetch::modify(req);
		}
	};

	TEST(fetch, basic_auth) {
		auto browser = test_basic_auth_fetch{"user", "secret", {{}}};
		auto req = browser.modify(nav::request{"http://www.example.com/"sv});
		auto auth = req.headers().find_front(nav::header::Authorization);
		ASSERT_NE(nullptr, auth);
		ASSERT_EQ("Basic dXNlcjpzZWNyZXQ=", *auth);
		ASSERT_EQ("user", browser.username());
	}
}  // namespace tangle::browser::testing
