// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/http/curl.hpp>
#include <tangle/http/proto.hpp>
#include <tangle/nav/protocol.hpp>
#include "flask-app.hh"

using namespace std::literals;

namespace tangle::http::curl::testing {

	class curl_test : public ::testing::Test {
	public:
		static void SetUpTestSuite() {
			proto = ::tangle::http::proto();
			browser.reg_proto("http", proto);
			app.start();
		}
		static void TearDownTestSuite() { app.stop(); }

	protected:
		static flask::app app;
		static std::shared_ptr<nav::protocol> proto;
		static nav::navigator browser;
	};

	flask::app curl_test::app{};
	std::shared_ptr<nav::protocol> curl_test::proto{};
	nav::navigator curl_test::browser{{}};

	TEST_F(curl_test, hello_world) {
		nav::request req{"http://127.0.0.1:5000/"sv};
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_EQ(doc.text(), "Hello world!");
	}

	TEST_F(curl_test, echo) {
		nav::request::meta_list hdrs{
		    {"X-hdr", "header value"},
		};

		nav::request req{"echo"sv};
		req.max_redir(0);
		req.referrer("http://127.0.0.1:5000/"sv);
        req.custom_agent("custom-agent/1.0");
		req.meta(std::move(hdrs));

		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();

		EXPECT_EQ(doc.text(), R"(Accept: */*
Host: 127.0.0.1:5000
Referer: http://127.0.0.1:5000/
User-Agent: custom-agent/1.0
X-Hdr: header value
)");
	}

	TEST_F(curl_test, loop) {
		nav::request req{"loop"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.max_redir(100);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();

		EXPECT_EQ(doc.text(), "URL: /loop/fourth");
	}
}  // namespace tangle::http::curl::testing
