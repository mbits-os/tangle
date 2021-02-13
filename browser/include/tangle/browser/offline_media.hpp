// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <filesystem>
#include <tangle/browser/fetch.hpp>

namespace tangle::browser {
	class offline_media {
	public:
		offline_media(fetch_interface& browser);
		virtual ~offline_media();
		std::string analyze(std::string_view, uri const& base_url);

	protected:
		virtual std::optional<uri> normalize(std::string_view,
		                                     uri const& base_url) const;
		virtual std::string download(uri const&);
		virtual std::string store(std::string const& filename,
		                          std::string const& ext,
		                          std::string_view bytes) = 0;

	private:
		std::string_view downloaded(std::string_view, uri const& base_url);
		std::string_view cached(std::string_view, uri const& base_url) const;

		fetch_interface& browser_;
		std::unordered_map<std::string, std::string> srcs_;
	};

	class fs_offline_media : public offline_media {
	public:
		fs_offline_media(fetch_interface& browser,
		                 std::filesystem::path const&);

	private:
		std::string store(std::string const& filename,
		                  std::string const& ext,
		                  std::string_view bytes) override;

		std::filesystem::path dirname_;
		bool first_{true};
	};

	// Getting images from the same server is easy, but if the HTML snippet is
	// on one service and the image is on another, then we might not be able to
	// authenticate for the other server.
	//
	// Think analyzing HTML review comments from GitHub Enterprise server with
	// embedded images from either the same GitHub or Atlassian Jira task or a
	// KB article.
	//
	// In such case, the report this snippet ends as part of might still be able
	// to cache offline images from GitHub, but the application might not have
	// enough credentials to get any resource from a Jira server.
	class intranet_offline_media_helper {
	public:
		static std::optional<uri> filter_by_server(std::string_view,
		                                           uri const& base_url);
	};

	template <typename BaseClass>
	class basic_intranet_offline_media : public BaseClass {
	protected:
		using BaseClass::BaseClass;

		std::optional<uri> normalize(std::string_view href,
		                             uri const& base_url) const override {
			return intranet_offline_media_helper::filter_by_server(href,
			                                                       base_url);
		}
	};

	using intranet_offline_media = basic_intranet_offline_media<offline_media>;
	using fs_intranet_offline_media =
	    basic_intranet_offline_media<fs_offline_media>;
}  // namespace tangle::browser
