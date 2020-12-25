// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace tangle::http {
	enum class header {
		empty,
		extension_header,
		// [Request header](https://tools.ietf.org/html/rfc2616#section-5.3)
		Accept,
		Accept_Charset,
		Accept_Encoding,
		Accept_Language,
		Authorization,
		Expect,
		From,
		Host,
		If_Match,
		If_Modified_Since,
		If_None_Match,
		If_Range,
		If_Unmodified_Since,
		Max_Forwards,
		Proxy_Authorization,
		Range,
		Referer,
		TE,
		User_Agent,

		// [Response header](https://tools.ietf.org/html/rfc2616#section-6.2)
		Accept_Ranges,
		Age,
		ETag,
		Location,
		Proxy_Authenticate,
		Retry_After,
		Server,
		Vary,
		WWW_Authenticate,

		// [Entity header](https://tools.ietf.org/html/rfc2616#section-7.1)
		Allow,
		Content_Encoding,
		Content_Language,
		Content_Length,
		Content_Location,
		Content_MD5,
		Content_Range,
		Content_Type,
		Expires,
		Last_Modified,

		// [General header](https://tools.ietf.org/html/rfc2616#section-14)
		Cache_Control,
		Connection,
		Date,
		Pragma,
		Trailer,
		Transfer_Encoding,
		Upgrade,
		Via,
		Warning,

		// Cookie headers:
		Cookie,
		Set_Cookie,

		// https://tools.ietf.org/html/rfc6266
		Content_Disposition,

		// https://fetch.spec.whatwg.org/#origin-header
		Origin,

		// [CORS requests](https://fetch.spec.whatwg.org/#http-requests)
		Access_Control_Allow_Origin,
		Access_Control_Allow_Credentials,

		// [CORS responses](https://fetch.spec.whatwg.org/#http-responses)
		Access_Control_Allow_Methods,
		Access_Control_Allow_Headers,
		Access_Control_Max_Age,
		Access_Control_Expose_Headers,

		// https://tools.ietf.org/html/rfc8288#section-3
		Link,
	};

	class header_key {
	public:
		header_key(header h) noexcept : header_(h) {}

		header value() const noexcept { return header_; }
		std::string const& extension() const noexcept { return extension_; }

		static header_key make(std::string) noexcept;
		static char const* name(header) noexcept;
		char const* name() const noexcept;

		bool empty() const noexcept { return header_ == header::empty; }
		bool extension_header() const noexcept {
			return header_ == header::extension_header;
		}
		bool operator==(header_key const& oth) const noexcept {
			return header_ == oth.header_ &&
			       (header_ == header::extension_header
			            ? extension_ == oth.extension_
			            : true);
		}
		bool operator!=(header_key const& oth) const noexcept {
			return !(*this == oth);
		}

		bool operator<(header_key const& oth) const noexcept {
			if (header_ == oth.header_) {
				if (header_ == header::extension_header)
					return extension_ < oth.extension_;
				return false;
			}
			return header_ < oth.header_;
		}

	private:
		header header_ = header::empty;
		std::string extension_;
		header_key(std::string&& ex) noexcept
		    : header_(header::extension_header), extension_(std::move(ex)) {}
	};
}  // namespace tangle::http

namespace std {
	template <>
	struct hash<tangle::http::header_key> {
		using header = tangle::http::header;
		using header_key = tangle::http::header_key;
		size_t operator()(tangle::http::header_key const& key) const
		    noexcept(noexcept(hash<header>{}(header::Accept)) && noexcept(
		        hash<std::string>{}(std::string{}))) {
			if (key.value() != header::extension_header)
				return hash<header>{}(key.value());
			return hash<std::string>{}(key.extension());
		}
	};
}  // namespace std

namespace tangle::http {
	class headers {
		std::unordered_map<header_key, std::vector<std::string>> headers_;

	public:
		void add(header_key const& key, std::string const& value) {
			headers_[key].emplace_back(value);
		}
		void add(header_key const& key, std::string&& value) {
			headers_[key].emplace_back(std::move(value));
		}
		void set(header_key const& key, std::string const& value) {
			auto& values = headers_[key];
			values.clear();
			values.emplace_back(value);
		}
		void set(header_key const& key, std::string&& value) {
			auto& values = headers_[key];
			values.clear();
			values.emplace_back(std::move(value));
		}
		void erase(header_key const& key) noexcept { headers_.erase(key); }

		auto empty() const noexcept { return headers_.empty(); }
		auto size() const noexcept { return headers_.size(); }
		auto find(header_key const& key) const { return headers_.find(key); }
		auto begin() const { return headers_.begin(); }
		auto end() const { return headers_.end(); }
		void clear() { headers_.clear(); }

		bool has(header_key const& key) const {
			auto it = find(key);
			return it != end() && !it->second.empty();
		}

		const std::string* find_front(header_key const& key) const {
			auto it = find(key);
			if (it != end() && !it->second.empty()) return &it->second.front();
			return nullptr;
		}
	};

	namespace http_version {
		struct version_t {
			constexpr version_t() = default;
			constexpr version_t(uint16_t M_ver, uint16_t m_ver) noexcept
			    : version{(static_cast<uint32_t>(M_ver) << 16) |
			              static_cast<uint32_t>(m_ver)} {}

			constexpr inline uint16_t M_ver() const noexcept {
				return static_cast<uint16_t>((version >> 16) & 0xFFFF);
			}

			constexpr inline uint16_t m_ver() const noexcept {
				return static_cast<uint16_t>((version)&0xFFFF);
			}

			constexpr bool operator==(version_t const& rhs) const noexcept {
				return version == rhs.version;
			}
			constexpr bool operator!=(version_t const& rhs) const noexcept {
				return !(*this == rhs);
			}
			constexpr bool operator<(version_t const& rhs) const noexcept {
				return version < rhs.version;
			}
			constexpr bool operator>=(version_t const& rhs) const noexcept {
				return rhs < *this;
			}
			constexpr bool operator<=(version_t const& rhs) const noexcept {
				return !(rhs < *this);
			}
			constexpr bool operator>(version_t const& rhs) const noexcept {
				return rhs < *this;
			}

		private:
			uint32_t version = 0;
		};

		static constexpr version_t http_none{0, 0};
		static constexpr version_t http_1_0{1, 0};
		static constexpr version_t http_1_1{1, 1};
	}  // namespace http_version
	using http_version_t = http_version::version_t;
}  // namespace tangle::http
