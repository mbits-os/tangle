// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <filesystem>
#include <tangle/nav/navigator.hpp>

namespace tangle {
	class offline_media {
	public:
		offline_media(nav::navigator& browser);
		virtual ~offline_media();
		std::string analyze(std::string_view, uri const& doc_url);

    protected:
		virtual std::string download(uri const&);
		virtual std::string store(std::string const& filename,
		                          std::string const& ext,
		                          std::string_view bytes) = 0;

	private:
		virtual std::optional<uri> normalize(std::string_view,
		                                     uri const& doc_url) const;
		std::string_view downloaded(std::string_view, uri const& doc_url);
		std::string_view cached(std::string_view, uri const& doc_url) const;

		nav::navigator& browser_;
		std::unordered_map<std::string, std::string> srcs_;
	};

	class fs_offline_media : public offline_media {
	public:
		fs_offline_media(nav::navigator& browser, std::filesystem::path const&);

	private:
		std::string store(std::string const& filename,
		                  std::string const& ext,
		                  std::string_view bytes) override;

		std::filesystem::path dirname_;
		bool first_{true};
	};
}  // namespace tangle
