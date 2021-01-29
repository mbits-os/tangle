// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <filesystem>
#include <tangle/http/curl.hpp>
#include <tangle/http/proto.hpp>
#include <tangle/nav/protocol.hpp>
#include "flask-app.hh"

using namespace std::literals;
namespace fs = std::filesystem;

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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.conn_status(), 0)
		    << "  Connection status: " << doc.conn_status()
		    << "\n  Connection status text: " << doc.conn_status_text();
		EXPECT_TRUE(doc.conn_status_text().empty());

		EXPECT_EQ(doc.text(), "Hello world!");
		EXPECT_FALSE(doc.is_link());
		EXPECT_TRUE(doc.exists());
	}

	TEST_F(curl_test, copy_and_move) {
		nav::request req{"http://127.0.0.1:5000/"sv};
		nav::document doc;
		{
			nav::document doc1;
			{
				auto doc2 = browser.open(req);
				doc1 = doc2;
			}
			doc = std::move(doc1);
		}

		{
			auto copy = browser;
			auto moved = std::move(copy);
			nav::navigator copy2, moved2;
			copy2 = moved;
			moved2 = std::move(moved);
		}

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.conn_status(), 0)
		    << "  Connection status: " << doc.conn_status()
		    << "\n  Connection status text: " << doc.conn_status_text();
		EXPECT_TRUE(doc.conn_status_text().empty());

		EXPECT_EQ(doc.text(), "Hello world!");
		EXPECT_FALSE(doc.is_link());
		EXPECT_TRUE(doc.exists());
	}

	TEST_F(curl_test, hello_world_header) {
		using nav::header;
		nav::request req{"http://127.0.0.1:5000/"sv};
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.conn_status(), 0)
		    << "  Connection status: " << doc.conn_status()
		    << "\n  Connection status text: " << doc.conn_status_text();
		EXPECT_TRUE(doc.conn_status_text().empty());

		EXPECT_EQ(doc.text(), "Hello world!");
		EXPECT_FALSE(doc.is_link());
		EXPECT_TRUE(doc.exists());

		using headers_item =
		    std::pair<nav::header_key, std::vector<std::string>>;
		std::vector<headers_item> headers;
		std::copy(doc.headers().begin(), doc.headers().end(),
		          std::back_inserter(headers));
		std::sort(headers.begin(), headers.end());
		std::string actual;
		static constexpr auto npos = std::string_view::npos;
		for (auto const& [name, values] : headers) {
			if (name == header::Date) {
				actual += "Date: <today>\n";
				continue;
			}
			if (name == header::Server) {
				for (auto const& value : values) {
					std::string server;
					std::string_view value_view{value};
					std::string_view::size_type pos = 0;
					std::string_view::size_type prev = 0;
					while (pos != npos) {
						pos = value_view.find('/', prev);
						if (pos == npos) {
							server.append(value_view.substr(prev));
							break;
						}
						server.append(value_view.substr(prev, pos + 1 - prev));
						server.append("<version>"sv);
						prev = pos + 1;
						prev = pos = value_view.find(' ', prev);
					}
					actual += "Server: ";
					actual += server;
					actual.push_back('\n');
				}
				continue;
			}
			if (name == header::Set_Cookie) {
				static constexpr auto expires = "Expires="sv;
				for (auto const& value : values) {
					auto pos = value.find(expires);
					if (pos != npos) {
						std::string cookie =
						    value.substr(0, pos + expires.size());
						cookie.append("<date>"sv);
						pos = value.find(';', pos + expires.size());
						if (pos != npos) cookie.append(value.substr(pos));

						actual += "Set-Cookie: ";
						actual += cookie;
						actual.push_back('\n');
					} else {
						actual += "Set-Cookie: ";
						actual += value;
						actual.push_back('\n');
					}
				}
				continue;
			}

			for (auto const& value : values) {
				actual += name.name();
				actual += ": ";
				actual += value;
				actual.push_back('\n');
			}
		}
		std::string expected = R"(x-user-defined: user defined special value
x-user-ii: user defined special value
Server: Werkzeug/<version> Python/<version>
Content-Length: 12
Content-Type: text/plain; charset=UTF-8
Date: <today>
Set-Cookie: cookie=gaderypoluki; Expires=<date>; Max-Age=36000; Path=/
)";
		EXPECT_EQ(expected, actual);
	}

	TEST_F(curl_test, non_existent) {
		nav::request req{"http://127.0.0.1:5000/404"sv};
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 4)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.conn_status(), 0)
		    << "  Connection status: " << doc.conn_status()
		    << "\n  Connection status text: " << doc.conn_status_text();
		EXPECT_TRUE(doc.conn_status_text().empty());

		EXPECT_FALSE(doc.is_link());
		EXPECT_FALSE(doc.exists());
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();

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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();

		EXPECT_EQ(doc.location().string(), "https://www.google.com/"sv);
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, echo) {
		nav::headers hdrs{
		    {nav::header_key::make("x-hdr"), "header value"},
		};

		nav::request req{"echo"sv};
		req.max_redir(0);
		req.referrer("http://127.0.0.1:5000/"sv);
		req.custom_agent("custom-agent/1.0");
		req.headers(std::move(hdrs));

		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();

		EXPECT_EQ(doc.text(), R"(Accept: */*
Cookie: cookie=gaderypoluki
Host: 127.0.0.1:5000
Referer: http://127.0.0.1:5000/
User-Agent: custom-agent/1.0
X-Hdr: header value
)");
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, hello_world_null_header) {
		nav::headers hdrs{
		    {nav::header::empty, "Should not be used by backend"},
		    {nav::header_key::make("x-hdr"), "header value"},
		};

		nav::request req{"echo"sv};
		req.max_redir(0);
		req.referrer("http://127.0.0.1:5000/"sv);
		req.custom_agent("custom-agent/1.0");
		req.headers(std::move(hdrs));

		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();

		EXPECT_EQ(doc.text(), R"(Accept: */*
Cookie: cookie=gaderypoluki
Host: 127.0.0.1:5000
Referer: http://127.0.0.1:5000/
User-Agent: custom-agent/1.0
X-Hdr: header value
)");
		EXPECT_FALSE(doc.is_link());
	}

	TEST_F(curl_test, echo_configured) {
		std::error_code ec;
		auto temp = fs::temp_directory_path(ec);
		ASSERT_FALSE(ec);

		struct deleter {
			fs::path const path;
			~deleter() {
				std::error_code ec;
				fs::remove(path, ec);
			}
		};

		deleter clean{temp / "tests.jar"};
		nav::navigator local{{
		    "test/1.2.3",
		    clean.path.string(),
		    {"pl-PL", "en-US", "en-GB"},
		}};
		local.reg_proto("http", proto);
		local.reg_proto("https", proto);

		auto root = local.open(nav::request{"http://127.0.0.1:5000/"sv});
		EXPECT_EQ(root.status() / 100, 2)
		    << "  Status: " << root.status()
		    << "\n  Status text: " << root.status_text();
		EXPECT_FALSE(root.status_text().empty());

		EXPECT_EQ(root.conn_status(), 0)
		    << "  Connection status: " << root.conn_status()
		    << "\n  Connection status text: " << root.conn_status_text();
		EXPECT_TRUE(root.conn_status_text().empty());

		EXPECT_EQ(root.text(), "Hello world!");
		EXPECT_FALSE(root.is_link());
		EXPECT_TRUE(root.exists());

		nav::headers hdrs{
		    {nav::header_key::make("X-hdr"), "header value"},
		};

		nav::request req{"echo"sv};
		req.headers(std::move(hdrs));
		auto doc = root.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();

		EXPECT_EQ(doc.text(), R"(Accept: */*
Cookie: cookie=gaderypoluki
Host: 127.0.0.1:5000
Referer: http://127.0.0.1:5000/
User-Agent: test/1.2.3
X-Hdr: header value
)");
		EXPECT_FALSE(doc.is_link());
		EXPECT_TRUE(doc.exists());
	}

	TEST_F(curl_test, loop) {
		nav::request req{"loop"sv};
		req.referrer("http://127.0.0.1:5000/"sv);
		req.max_redir(100);
		auto doc = browser.open(req);

		EXPECT_EQ(doc.status() / 100, 2)
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_NE(doc.conn_status(), 0)
		    << "  Connection status: " << doc.conn_status()
		    << "\n  Connection status text: " << doc.conn_status_text();

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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
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
		    << "  Status: " << doc.status()
		    << "\n  Status text: " << doc.status_text();
		EXPECT_FALSE(doc.status_text().empty());

		EXPECT_EQ(doc.text(), "Test passed");
		EXPECT_EQ(doc.location().string(), "http://127.0.0.1:5000/upload"sv);
		EXPECT_FALSE(doc.is_link());
	}
}  // namespace tangle::http::curl::testing
