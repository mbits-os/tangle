// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string_view>
#include <tangle/browser/html_split.hpp>
#include <tangle/browser/offline_media.hpp>
#include <tangle/str.hpp>

#ifdef WIN32
#include <Windows.h>
#undef min
#undef max
#endif

using namespace std::literals;
namespace fs = std::filesystem;

namespace tangle::browser {
	namespace {
		constexpr std::string_view embedded[] = {"img"sv, "source"sv, "video"sv,
		                                         "audio"sv};

		bool is_embedded(elem const& tag) {
			for (auto const& name : embedded) {
				if (tag.name.is(name)) return true;
			}
			return false;
		}

		auto find_attr(elem const& tag, std::string_view attr_name) {
			auto it = tag.attrs.begin();
			auto end = tag.attrs.end();
			while (it != end && !attr_pos::is(it->first, attr_name))
				++it;
			return it;
		}

		struct mime_ext {
			std::string_view mime;
			std::string_view ext;
		};

		constexpr mime_ext ext_map[] = {
		    {"application/json"sv, "json"sv},
		    {"application/ogg"sv, "ogg"sv},
		    {"audio/3gpp"sv, "3gp"sv},
		    {"audio/3gpp2"sv, "3g2"sv},
		    {"audio/aac"sv, "aac"sv},
		    {"audio/mid"sv, "midi"sv},
		    {"audio/mp4"sv, "m4a"sv},
		    {"audio/mpeg"sv, "mp3"sv},
		    {"audio/ogg"sv, "oga"sv},
		    {"audio/opus"sv, "opus"sv},
		    {"audio/vnd.wav"sv, "wav"sv},
		    {"audio/vorbis"sv, "oga"sv},
		    {"audio/wav"sv, "wav"sv},
		    {"audio/wave"sv, "wav"sv},
		    {"audio/webm"sv, "weba"sv},
		    {"audio/x-midi"sv, "midi"sv},
		    {"audio/x-pn-wav"sv, "wav"sv},
		    {"audio/x-wav"sv, "wav"sv},
		    {"image/apng"sv, "apng"sv},
		    {"image/bmp"sv, "bmp"sv},
		    {"image/gif"sv, "gif"sv},
		    {"image/jpeg"sv, "jpg"sv},
		    {"image/png"sv, "png"sv},
		    {"image/svg+xml"sv, "svg"sv},
		    {"image/tiff"sv, "tiff"sv},
		    {"image/vnd.microsoft.icon"sv, "ico"sv},
		    {"image/webp"sv, "webp"sv},
		    {"image/x-icon"sv, "ico"sv},
		    {"text/html"sv, "html"sv},
		    {"text/plain"sv, "txt"sv},
		    {"video/3gpp"sv, "3gp"sv},
		    {"video/3gpp2"sv, "3g2"sv},
		    {"video/mp2t"sv, "ts"sv},
		    {"video/mp4"sv, "mp4"sv},
		    {"video/mpeg"sv, "mpeg"sv},
		    {"video/ogg"sv, "ogv"sv},
		    {"video/webm"sv, "webm"sv},
		    {"video/x-msvideo"sv, "avi"sv},
		};

		std::string_view mime_type_ext(nav::headers const& response) {
			auto content_type = response.find_front(nav::header::Content_Type);
			if (!content_type) return {};

			auto mime_type = std::string_view{*content_type};
			auto pos = mime_type.find(';');
			if (pos != std::string_view::npos)
				mime_type = mime_type.substr(0, pos);
			mime_type = strip_sv(mime_type);
			auto it = std::lower_bound(std::begin(ext_map), std::end(ext_map),
			                           mime_type,
			                           [](auto const& pair, auto const& test) {
				                           return pair.mime < test;
			                           });

			if (it == std::end(ext_map) || it->mime != mime_type) return {};
			return it->ext;
		}

		struct attr_replacement {
			size_t start{};
			size_t stop{};
			std::string replacement{};

			bool operator<(attr_replacement const& rhs) const noexcept {
				return start < rhs.start;
			}
		};

		std::string patch(std::string_view text,
		                  std::vector<attr_replacement> const& replacements) {
			size_t length{};
			size_t prev = 0;
			for (auto const& rep : replacements) {
				length += rep.start - prev;
				length += rep.replacement.size();
				prev = rep.stop;
			}
			length += text.size() - prev;

			std::string result{};
			result.reserve(length);

			prev = 0;
			for (auto const& rep : replacements) {
				auto const chunk = text.substr(prev, rep.start - prev);
				result.append(chunk);
				result.append(rep.replacement);
				prev = rep.stop;
			}
			result.append(text.substr(prev));

			return result;
		}

		struct file {
			void operator()(FILE* file) { std::fclose(file); }
			static std::unique_ptr<FILE, file> open(fs::path const& path) {
				using ptr = std::unique_ptr<FILE, file>;
#ifdef WIN32
				return ptr{_wfopen(path.native().c_str(), L"wb")};
#else
				return ptr{std::fopen(path.string().c_str(), "wb")};
#endif
			}
		};

		fs::path utf8_path(std::string_view const& utf8) {
#ifdef WIN32
			int size = MultiByteToWideChar(CP_UTF8, 0, utf8.data(),
			                               utf8.length(), nullptr, 0);
			if (size < 1) return fs::path{utf8};

			auto buffer =
			    std::make_unique<wchar_t[]>(static_cast<size_t>(size) + 1);
			int result = MultiByteToWideChar(CP_UTF8, 0, utf8.data(),
			                                 utf8.length(), buffer.get(), size);
			if (result < 1) return fs::path{utf8};
			buffer[size] = 0;
			return fs::path{buffer.get()};
#else
			return fs::path{utf8};
#endif
		}

		std::string utf8_path(fs::path const& path) {
#ifdef WIN32
			auto wide = path.native();
			int size =
			    WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide.length(),
			                        nullptr, 0, nullptr, nullptr);
			if (size < 1) return path.string();

			auto buffer =
			    std::make_unique<char[]>(static_cast<size_t>(size) + 1);
			int result =
			    WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide.length(),
			                        buffer.get(), size, nullptr, nullptr);
			if (result < 1) return path.string();
			buffer[size] = 0;
			return buffer.get();
#else
			return path.string();
#endif
		}

		std::pair<std::string, std::string> splitext(
		    std::string_view path_view) {
			// path comes from normalized uri, which means it will always
			// contain a slash (at the start)
			auto const pos = path_view.rfind('/') + 1;
			auto filename = path_view.substr(pos);
			if (filename.empty()) filename = "index"sv;

			auto path = utf8_path(filename);
			auto ext = utf8_path(path.extension());

			if (!ext.empty() && ext.front() == '.') ext = ext.substr(1);

			return {utf8_path(path.stem()), ext};
		}

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
	}  // namespace

	offline_media::offline_media(nav::navigator& browser) : browser_{browser} {}

	offline_media::~offline_media() = default;

	std::string offline_media::analyze(std::string_view text,
	                                   uri const& base_url) {
		auto tags = browser::html_split(text);

		std::vector<attr_replacement> replacements{};

		for (auto const& tag : tags) {
			if (!is_embedded(tag)) continue;
			auto it = find_attr(tag, "src"sv);
			if (it == tag.attrs.end()) continue;

			auto repl = downloaded(it->second.value, base_url);
			if (!repl.empty()) {
				replacements.push_back({it->second.start,
				                        it->second.stop,
				                        {repl.data(), repl.size()}});
			}
		}

		for (auto const& tag : tags) {
			if (!tag.name.is("a"sv)) continue;
			auto it = find_attr(tag, "href"sv);
			if (it == tag.attrs.end()) continue;

			auto repl = cached(it->second.value, base_url);
			if (!repl.empty()) {
				replacements.push_back({it->second.start,
				                        it->second.stop,
				                        {repl.data(), repl.size()}});
			}
		}

		std::sort(replacements.begin(), replacements.end());
		return patch(text, replacements);
	}

	std::optional<uri> offline_media::normalize(std::string_view href,
	                                            uri const& base_url) const {
		return uri::canonical(attr_decode(href), uri::make_base(base_url));
	}

	std::string_view offline_media::downloaded(std::string_view href,
	                                           uri const& base_url) {
		auto address = normalize(href, base_url);
		if (!address) return {};

		auto it = srcs_.find(address->string());
		if (it == srcs_.end()) {
			auto path = download(*address);
			if (path.empty()) return {};
			auto [iterator, success] =
			    srcs_.insert({address->string(), std::move(path)});
			if (!success) return {};

			it = iterator;
		}

		return it->second;
	}

	std::string_view offline_media::cached(std::string_view href,
	                                       uri const& base_url) const {
		auto address = normalize(href, base_url);
		if (!address) return {};
		auto it = srcs_.find(address->string());
		if (it == srcs_.end()) return {};
		return it->second;
	}

	std::string offline_media::download(uri const& address) {
		auto req = nav::request{address};
		req.set(nav::header::Accept,
		        "text/html,application/xhtml+xml,application/"
		        "xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
		auto resp = browser_.open(req);

		auto [fname, ext] = splitext(address.path());
		if (ext.empty()) {
			auto ext_view = mime_type_ext(resp.headers());
			if (ext_view.empty()) ext_view = "bin"sv;
			ext = ext_view;
		}

		return store(fname, ext, resp.text());
	}

	fs_offline_media::fs_offline_media(nav::navigator& browser,
	                                   fs::path const& dirname)
	    : offline_media{browser}, dirname_{dirname} {}

	std::string fs_offline_media::store(std::string const& filename,
	                                    std::string const& ext,
	                                    std::string_view bytes) {
		if (first_) {
			first_ = false;
			std::error_code ec{};

			fs::remove_all(dirname_, ec);
			if (ec) return {};

			fs::create_directories(dirname_, ec);
			if (ec) return {};
		}

		unsigned counter = 0;
		auto path = dirname_ / concat(filename, ".", ext);

		std::error_code ec{};
		while (fs::exists(path, ec) && !ec) {
			++counter;
			path = dirname_ /
			       concat(filename, "-", std::to_string(counter), ".", ext);
		};
		if (ec) return {};

		if (auto outfile = file::open(path)) {
			std::fwrite(bytes.data(), 1, bytes.size(), outfile.get());
		}

		return "\"" + attr_encode(path.generic_string()) + "\"";
	}

	namespace {
		template <typename Item>
		struct simple_list {
			constexpr simple_list() noexcept = default;
			template <size_t Length>
			constexpr simple_list(Item const (&array)[Length]) noexcept
			    : first_{array}, last_{array + Length} {}

			constexpr Item const* begin() const noexcept { return first_; }
			constexpr Item const* end() const noexcept { return last_; }

		private:
			Item const* first_{};
			Item const* last_{};
		};

		struct alias {
			std::string_view known_server;
			simple_list<std::string_view> content_aliases;
		};

		constexpr std::string_view github_aliases[] = {
		    "user-images.githubusercontent.com"sv,
		};
		constexpr alias servers[] = {
		    {
		        "api.github.com"sv,
		        github_aliases,
		    },
		};
	}  // namespace

	std::optional<uri> intranet_offline_media_helper::filter_by_server(
	    std::string_view href,
	    uri const& base_url) {
		auto resource = uri::canonical(attr_decode(href), base_url);

		auto url_auth = base_url.parsed_authority();
		auto res_auth = resource.parsed_authority();

		if (res_auth.host != url_auth.host) {
			for (auto [name, aliases] : servers) {
				if (std::string_view{name} != url_auth.host) continue;

				for (auto alias : aliases) {
					if (std::string_view{alias} == res_auth.host) {
						return resource;
					}
				}

				break;
			}
			return std::nullopt;
		}

		return resource;
	}
}  // namespace tangle::browser
