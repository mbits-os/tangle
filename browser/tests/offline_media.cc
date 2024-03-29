// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/offline_media.hpp>
#include <tangle/nav/doc_impl.hpp>
#include <tangle/nav/document.hpp>
#include <tangle/nav/protocol.hpp>
#include <unordered_set>
#include <gtest/gtest.h>

using namespace std::literals;
namespace fs = std::filesystem;

namespace tangle::browser::testing {
	struct mock_response {
		std::string_view url{};
		std::string_view content_type{};
		std::string_view content{"DUMMY-CONTENT"sv};
	};

	struct offline_media_test_data {
		std::string_view text;
		std::string_view expected;
		std::string_view base_url;
		std::string_view prefix;
		std::vector<mock_response> responses{};
	};

	class proto_mock : public nav::protocol {
		std::vector<mock_response> const* resps_{};

	public:
		nav::document open(const nav::request& req,
		                   nav::navigator& nav) override {
			auto const address = req.address();
			std::string_view content_type = "text/html"sv;
			std::string_view content = "DUMMY-CONTENT"sv;
			if (resps_) {
				for (auto const& resp : *resps_) {
					if (resp.url != address) continue;
					content_type = resp.content_type;
					content = resp.content;
					break;
				}
			}

			auto doc = std::make_shared<nav::doc_impl>(address, &nav);
			doc->on_headers(
			    "OK"s, 200,
			    {
			        {
			            "Content-Type"s,
			            {{content_type.data(), content_type.size()}},
			        },
			    });
			if (!content.empty()) doc->on_data(content.data(), content.size());
			return nav::document::wrap(doc);
		}

		void set_responses(std::vector<mock_response> const& list) {
			resps_ = &list;
		}
	};

	template <typename... String>
	std::string concat__(String... strings) {
		auto const sizes = (strings.size() + ... + 0);
		std::string result{};
		result.reserve(sizes);
		(result.append(strings), ...);
		return result;
	}

	template <typename... String>
	std::string concat(String&&... strings) {
		return concat__(std::string_view(std::forward<String>(strings))...);
	}

	template <typename BaseClass>
	class test_offline_media_infix : public BaseClass {
	public:
		test_offline_media_infix(fetch_interface& browser,
		                         std::string&& dirname)
		    : BaseClass{browser}, dirname_{dirname} {}

	private:
		std::string download(uri const& address,
		                     uri::server_quirks quirks) override {
			std::cerr << "dl > " << address << '\n';
			return BaseClass::download(address, quirks);
		}

		std::string store(std::string const& filename,
		                  std::string const& ext,
		                  std::string_view) override {
			unsigned counter = 0;
			auto path = concat(dirname_, "/", filename, ".", ext);
			while (true) {
				auto [it, succeeded] = files_.insert(path);
				if (succeeded) break;

				++counter;
				if (!counter) return {};

				path = concat(dirname_, "/", filename, "-",
				              std::to_string(counter), ".", ext);
			};
			return concat("\"", path, "\"");
		}

		std::string dirname_;
		std::unordered_set<std::string> files_{};
	};

	simple_fetch make_browser(std::vector<mock_response> const& list) {
		auto proto = std::make_shared<proto_mock>();
		proto->set_responses(list);
		auto navigator = nav::navigator{{}};
		navigator.reg_proto("http", proto);
		navigator.reg_proto("https", proto);
		return navigator;
	}

	using test_offline_media = test_offline_media_infix<browser::offline_media>;
	using test_intranet_offline_media =
	    test_offline_media_infix<browser::intranet_offline_media>;

	class offline_media
	    : public ::testing::TestWithParam<offline_media_test_data> {};
	class intranet_offline_media
	    : public ::testing::TestWithParam<offline_media_test_data> {};

	TEST_P(offline_media, process) {
		auto& param = GetParam();

		auto browser = make_browser(param.responses);
		auto offline = test_offline_media{
		    browser, {param.prefix.begin(), param.prefix.end()}};

		auto actual = offline.analyze(param.text, param.base_url);
		ASSERT_EQ(param.expected.data(), actual);
	}

	TEST_P(offline_media, process_fs) {
		auto& param = GetParam();

		auto browser = make_browser(param.responses);
		auto offline = fs_offline_media{
		    browser, {param.prefix.begin(), param.prefix.end()}};

		auto pwd = fs::current_path();
		auto dirname = fs::temp_directory_path() / "offline-media-tests";
		fs::create_directories(dirname);
		fs::current_path(dirname);
		auto actual = offline.analyze(param.text, param.base_url);
		fs::current_path(pwd);

		ASSERT_EQ(param.expected.data(), actual);
	}

	TEST_P(intranet_offline_media, process) {
		auto& param = GetParam();

		auto browser = make_browser(param.responses);
		auto offline = test_intranet_offline_media{
		    browser, {param.prefix.begin(), param.prefix.end()}};

		auto actual = offline.analyze(param.text, param.base_url);
		ASSERT_EQ(param.expected.data(), actual);
	}

	static offline_media_test_data snippets[] = {
	    {
	        "<img SRC=http://www.example.com/site/logo.gif>"sv,
	        "<img SRC=\"dir/logo.gif\">"sv,
	        "http://www.example.com/",
	        "dir"sv,
	    },
	    {
	        R"(<video SRC=/media/movie.mp4><A name=href></A></video>
<video src=movie></video>
<video src=movie.mp4></video>)"sv,
	        "<video SRC=\"dir/movie.mp4\"><A name=href></A></video>\n<video src=\"dir/movie-1.mp4\"></video>\n<video src=\"dir/movie.mp4\"></video>"sv,
	        "http://www.example.com/media/",
	        "dir"sv,
	        {
	            {"http://www.example.com/media/movie", "video/mp4"},
	        },
	    },
	    {
	        "<img src=/media/>"sv,
	        "<img src=\"dir/index.svg\">"sv,
	        "http://www.example.com/",
	        "dir"sv,
	        {
	            {"http://www.example.com/media/",
	             "image/svg+xml; charset=utf-8"},
	        },
	    },
	    {
	        R"(<img src=/data/img.gif>
<video>
<source src="/data/track.mov" />
</video>
<A HREF="img.gif">The image</A>)"sv,
	        R"(<img src="files/img.gif">
<video>
<source src="files/track.mov" />
</video>
<A HREF="files/img.gif">The image</A>)"sv,
	        "http://www.example.com/data/",
	        "files"sv,
	    },
	};

	static offline_media_test_data intranet_snippets[] = {
	    {
	        "<img SRC=http://www.example.com/site/logo.gif>"sv,
	        "<img SRC=\"dir/logo.gif\">"sv,
	        "http://www.example.com/",
	        "dir"sv,
	    },
	    {
	        R"(<video SRC=/media/movie.mp4><A name=href></A></video>
<video src=movie></video>
<video src=movie.mp4></video>)"sv,
	        "<video SRC=\"dir/movie.mp4\"><A name=href></A></video>\n<video src=\"dir/movie-1.mp4\"></video>\n<video src=\"dir/movie.mp4\"></video>"sv,
	        "http://www.example.com/media/",
	        "dir"sv,
	        {
	            {"http://www.example.com/media/movie", "video/mp4"},
	        },
	    },
	    {
	        "<img src=/media/>"sv,
	        "<img src=\"dir/index.svg\">"sv,
	        "http://www.example.com/",
	        "dir"sv,
	        {
	            {"http://www.example.com/media/",
	             "image/svg+xml; charset=utf-8"},
	        },
	    },
	    {
	        R"(<img src=/data/img.gif>
<video>
<source src="/data/track1.mov" />
<source src=http://www.example.com/data/track2.mov />
<source src=http://jira.example.com/data/track3.mov />
</video>
<A HREF="img.gif">The image</A>)"sv,
	        R"(<img src="files/img.gif">
<video>
<source src="files/track1.mov" />
<source src="files/track2.mov" />
<source src=http://jira.example.com/data/track3.mov />
</video>
<A HREF="files/img.gif">The image</A>)"sv,
	        "http://www.example.com/data/",
	        "files"sv,
	    },
	    {
	        R"(<img src=/data/img.gif>
<video>
<source src="/data/track1.mov" />
<source src=http://jira.example.com/data/track2.mov />
</video>
<img src=https://user-images.githubusercontent.com/user/GUID-GUID-GUID-GUID-GUID-GUID-GUID-GUID>)"sv,
	        R"(<img src="files/img.gif">
<video>
<source src="files/track1.mov" />
<source src=http://jira.example.com/data/track2.mov />
</video>
<img src="files/GUID-GUID-GUID-GUID-GUID-GUID-GUID-GUID.png">)"sv,
	        "http://api.github.com/",  // note http/https mismatch
	        "files"sv,
	        {
	            {"https://user-images.githubusercontent.com/user/"
	             "GUID-GUID-GUID-GUID-GUID-GUID-GUID-GUID",
	             "image/png"},
	        },
	    },
	};

	INSTANTIATE_TEST_SUITE_P(snippets,
	                         offline_media,
	                         ::testing::ValuesIn(snippets));

	INSTANTIATE_TEST_SUITE_P(intranet_snippets,
	                         intranet_offline_media,
	                         ::testing::ValuesIn(intranet_snippets));
}  // namespace tangle::browser::testing
