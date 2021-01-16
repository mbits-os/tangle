// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gmock/gmock.h>
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
			browser.reg_proto("https", proto);
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

	static void dbghex(const char* prefix, const void* str, size_t count) {
		constexpr size_t length = 32;
		static constexpr char alphabet[] = "0123456789abcdef";
		char line[length * 4 + 2];
		memset(line, ' ', sizeof(line));
		size_t ch = 0;
		auto chars = static_cast<char const*>(str);
		for (size_t j = 0; j < count; ++j) {
			auto c = *chars++;
			line[ch * 3] = alphabet[(c >> 4) & 0xF];
			line[ch * 3 + 1] = alphabet[(c)&0xF];
			line[ch * 3 + 2] = ' ';
			line[length * 3 + ch] =
			    std::isprint(static_cast<unsigned char>(c)) ? c : '.';

			++ch;
			if (ch == length) {
				ch = 0;
				line[length * 4] = '\n';
				line[length * 4 + 1] = 0;
				fputs(prefix, stderr);
				fputs(line, stderr);
			}
		}

		if (ch != 0) {
			for (size_t i = ch; i < length; ++i) {
				line[i * 3] = line[i * 3 + 1] = line[i * 3 + 2] =
				    line[length * 3 + i] = ' ';
			}

			line[length * 4] = '\n';
			line[length * 4 + 1] = 0;
			fputs(prefix, stderr);
			fputs(line, stderr);
		}
	}

	struct TraceMock : public nav::request_trace {
		MOCK_METHOD1(message, void(std::string_view));
		MOCK_METHOD1(req_header, void(std::string_view));
		MOCK_METHOD1(resp_header, void(std::string_view));
		MOCK_METHOD1(req_data, void(std::string_view));
		MOCK_METHOD1(resp_data, void(std::string_view));
		MOCK_METHOD1(ssl_data_in, void(std::string_view));
		MOCK_METHOD1(ssl_data_out, void(std::string_view));
	};

	struct TraceMock2 : public nav::request_trace {
		void message(std::string_view text) { std::cerr << "[CURL] " << text; }
		void req_header(std::string_view text) {
			std::cerr << "[CURL >>>] " << text;
		}
		void resp_header(std::string_view text) {
			std::cerr << "[CURL <<<] " << text;
		}
		void req_data(std::string_view text) {
			dbghex("[CURL -->] ", text.data(), text.size());
		}
		void resp_data(std::string_view text) {
			dbghex("[CURL <--] ", text.data(), text.size());
		}
		void ssl_data_in(std::string_view text) {
			dbghex("[CURL <--][SSL] ", text.data(), text.size());
		}
		void ssl_data_out(std::string_view text) {
			dbghex("[CURL -->][SSL] ", text.data(), text.size());
		}
	};

	using ::testing::_;
	using ::testing::AnyNumber;

	TEST_F(curl_test, hello_world) {
		nav::request req{"http://127.0.0.1:5000/"sv};
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.conn_status(), 0)
		    << "Connection status: " << doc.conn_status()
		    << "\nConnection status text: " << doc.conn_status_text();
		EXPECT_TRUE(doc.conn_status_text().empty());

		EXPECT_EQ(doc.text(), "Hello world!");
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, debug_get) {
		auto trace = std::make_shared<TraceMock>();
		EXPECT_CALL(*trace, message(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, resp_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_data(_)).Times(0);
		EXPECT_CALL(*trace, resp_data(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, ssl_data_in(_)).Times(0);
		EXPECT_CALL(*trace, ssl_data_out(_)).Times(0);

		nav::request req{"http://127.0.0.1:5000/"sv};
		req.trace(trace);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_EQ(doc.text(), "Hello world!");
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, debug_post) {
		auto trace = std::make_shared<TraceMock>();
		EXPECT_CALL(*trace, message(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, resp_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_data(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, resp_data(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, ssl_data_in(_)).Times(0);
		EXPECT_CALL(*trace, ssl_data_out(_)).Times(0);

		nav::request req{nav::method::post, "http://127.0.0.1:5000/upload"sv};
		req.trace(trace);
		req.content_type("application/json; charset=UTF-8");
		req.content(R"({"value": "Test passed"})");
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();

		EXPECT_EQ(doc.text(), "Test passed");
		EXPECT_EQ(doc.location().string(), "http://127.0.0.1:5000/upload"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, debug_ssl) {
		auto trace = std::make_shared<TraceMock>();
		EXPECT_CALL(*trace, message(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, resp_header(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, req_data(_)).Times(0);
		EXPECT_CALL(*trace, resp_data(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, ssl_data_in(_)).Times(AnyNumber());
		EXPECT_CALL(*trace, ssl_data_out(_)).Times(AnyNumber());

		nav::request req{"https://www.google.com"sv};
		req.trace(trace);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();

		EXPECT_EQ(doc.location().string(), "https://www.google.com/"sv);
		EXPECT_FALSE(doc.is_link());
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
Cookie: cookie=gaderypoluki
Host: 127.0.0.1:5000
Referer: http://127.0.0.1:5000/
User-Agent: custom-agent/1.0
X-Hdr: header value
)");
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, loop) {
		nav::request req{"loop"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.max_redir(100);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), "URL: /loop/fourth");
		EXPECT_EQ(doc.location().string(),
		          "http://127.0.0.1:5000/loop/fourth"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, partial_loop) {
		nav::request req{"loop"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.max_redir(2);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 3)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_NE(doc.conn_status(), 0)
		    << "Connection status: " << doc.conn_status()
		    << "\nConnection status text: " << doc.conn_status_text();

		EXPECT_FALSE(doc.conn_status_text().empty());

		EXPECT_EQ(doc.text(), "");
		EXPECT_EQ(doc.location().string(),
		          "http://127.0.0.1:5000/loop/third"sv);
		EXPECT_TRUE(doc.is_link());
	}

	TEST_F(curl_test, loop_from_doc) {
		auto root = browser.open(nav::request{"http://127.0.0.1:5000/"sv});

		auto doc = root.open("loop"sv);
		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), "URL: /loop/fourth");
		EXPECT_EQ(doc.location().string(),
		          "http://127.0.0.1:5000/loop/fourth"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, loop_from_doc_no_ref) {
		auto root = browser.open(nav::request{"http://127.0.0.1:5000/"sv});

		auto doc = root.open(nav::request{"loop"sv});
		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), "URL: /loop/fourth");
		EXPECT_EQ(doc.location().string(),
		          "http://127.0.0.1:5000/loop/fourth"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, post_form) {
		nav::request req{nav::method::post, "form"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.form_fields("A=v1&B=v2");
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), R"(Field A: v1
Field B: v2
)");
		EXPECT_EQ(doc.location().string(), "http://127.0.0.1:5000/form"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, upload) {
		nav::request req{nav::method::post, "upload"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.content_type("application/json; charset=UTF-8");
		req.content(R"({"value": "Test passed"})");
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "Status: " << doc.status()
		    << "\nStatus text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), "Test passed");
		EXPECT_EQ(doc.location().string(), "http://127.0.0.1:5000/upload"sv);
		EXPECT_FALSE(doc.is_link());
	}
}  // namespace tangle::http::curl::testing
