// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/nav/headers.hpp>
#include "headers_parsers.hpp"
#include <algorithm>
#include <cctype>

namespace tangle::nav {
	header_key header_key::make(std::string hdr) noexcept {
		static constexpr struct {
			const char* name;
			header value;
		} headers[] = {
		    {"accept", header::Accept},
		    {"accept-charset", header::Accept_Charset},
		    {"accept-encoding", header::Accept_Encoding},
		    {"accept-language", header::Accept_Language},
		    {"accept-ranges", header::Accept_Ranges},
		    {"access-control-allow-credentials",
		     header::Access_Control_Allow_Credentials},
		    {"access-control-allow-headers",
		     header::Access_Control_Allow_Headers},
		    {"access-control-allow-methods",
		     header::Access_Control_Allow_Methods},
		    {"access-control-allow-origin",
		     header::Access_Control_Allow_Origin},
		    {"access-control-expose-headers",
		     header::Access_Control_Expose_Headers},
		    {"access-control-max-age", header::Access_Control_Max_Age},
		    {"age", header::Age},
		    {"allow", header::Allow},
		    {"authorization", header::Authorization},
		    {"bcc", header::BCC},
		    {"cache-control", header::Cache_Control},
		    {"cc", header::CC},
		    {"comments", header::Comments},
		    {"connection", header::Connection},
		    {"content-disposition", header::Content_Disposition},
		    {"content-encoding", header::Content_Encoding},
		    {"content-language", header::Content_Language},
		    {"content-length", header::Content_Length},
		    {"content-location", header::Content_Location},
		    {"content-md5", header::Content_MD5},
		    {"content-range", header::Content_Range},
		    {"content-type", header::Content_Type},
		    {"cookie", header::Cookie},
		    {"date", header::Date},
		    {"encrypted", header::Encrypted},
		    {"etag", header::ETag},
		    {"expect", header::Expect},
		    {"expires", header::Expires},
		    {"from", header::From},
		    {"host", header::Host},
		    {"if-match", header::If_Match},
		    {"if-modified-since", header::If_Modified_Since},
		    {"if-none-match", header::If_None_Match},
		    {"if-range", header::If_Range},
		    {"if-unmodified-since", header::If_Unmodified_Since},
		    {"in-reply-to", header::In_Reply_To},
		    {"keywords", header::Keywords},
		    {"last-modified", header::Last_Modified},
		    {"link", header::Link},
		    {"location", header::Location},
		    {"max-forwards", header::Max_Forwards},
		    {"message-id", header::Message_ID},
		    {"origin", header::Origin},
		    {"pragma", header::Pragma},
		    {"proxy-authenticate", header::Proxy_Authenticate},
		    {"proxy-authorization", header::Proxy_Authorization},
		    {"range", header::Range},
		    {"received", header::Received},
		    {"references", header::References},
		    {"referer", header::Referer},
		    {"resent-bcc", header::Resent_BCC},
		    {"resent-cc", header::Resent_CC},
		    {"resent-date", header::Resent_Date},
		    {"resent-from", header::Resent_From},
		    {"resent-message-id", header::Resent_Message_ID},
		    {"resent-reply-to", header::Resent_Reply_To},
		    {"resent-sender", header::Resent_Sender},
		    {"resent-to", header::Resent_To},
		    {"retry-after", header::Retry_After},
		    {"return-path", header::Return_Path},
		    {"sender", header::Sender},
		    {"server", header::Server},
		    {"set-cookie", header::Set_Cookie},
		    {"subject", header::Subject},
		    {"te", header::TE},
		    {"to", header::To},
		    {"trailer", header::Trailer},
		    {"transfer-encoding", header::Transfer_Encoding},
		    {"upgrade", header::Upgrade},
		    {"user-agent", header::User_Agent},
		    {"vary", header::Vary},
		    {"via", header::Via},
		    {"warning", header::Warning},
		    {"www-authenticate", header::WWW_Authenticate},
		};

		for (auto& c : hdr)
			c = static_cast<char>(std::tolower(static_cast<uint8_t>(c)));

		auto it = std::lower_bound(
		    std::begin(headers), std::end(headers), hdr,
		    [](const auto& h, const auto& s) { return h.name < s; });

		if (it != std::end(headers)) {
			if (it->name == hdr) {
				return it->value;
			}
		}

		return header_key{std::move(hdr)};
	}

	const char* header_key::name(header h) noexcept {
		switch (h) {
			case header::Accept:
				return "Accept";
			case header::Accept_Charset:
				return "Accept-Charset";
			case header::Accept_Encoding:
				return "Accept-Encoding";
			case header::Accept_Language:
				return "Accept-Language";
			case header::Accept_Ranges:
				return "Accept-Ranges";
			case header::Age:
				return "Age";
			case header::Allow:
				return "Allow";
			case header::Authorization:
				return "Authorization";
			case header::Cache_Control:
				return "Cache-Control";
			case header::Connection:
				return "Connection";
			case header::Content_Encoding:
				return "Content-Encoding";
			case header::Content_Language:
				return "Content-Language";
			case header::Content_Length:
				return "Content-Length";
			case header::Content_Location:
				return "Content-Location";
			case header::Content_MD5:
				return "Content-MD5";
			case header::Content_Range:
				return "Content-Range";
			case header::Content_Type:
				return "Content-Type";
			case header::Cookie:
				return "Cookie";
			case header::Date:
				return "Date";
			case header::ETag:
				return "ETag";
			case header::Expect:
				return "Expect";
			case header::Expires:
				return "Expires";
			case header::From:
				return "From";
			case header::Host:
				return "Host";
			case header::If_Match:
				return "If-Match";
			case header::If_Modified_Since:
				return "If-Modified-Since";
			case header::If_None_Match:
				return "If-None-Match";
			case header::If_Range:
				return "If-Range";
			case header::If_Unmodified_Since:
				return "If-Unmodified-Since";
			case header::Last_Modified:
				return "Last-Modified";
			case header::Location:
				return "Location";
			case header::Max_Forwards:
				return "Max-Forwards";
			case header::Pragma:
				return "Pragma";
			case header::Proxy_Authenticate:
				return "Proxy-Authenticate";
			case header::Proxy_Authorization:
				return "Proxy-Authorization";
			case header::Range:
				return "Range";
			case header::Referer:
				return "Referer";
			case header::Retry_After:
				return "Retry-After";
			case header::Server:
				return "Server";
			case header::Set_Cookie:
				return "Set-Cookie";
			case header::TE:
				return "TE";
			case header::Trailer:
				return "Trailer";
			case header::Transfer_Encoding:
				return "Transfer-Encoding";
			case header::Upgrade:
				return "Upgrade";
			case header::User_Agent:
				return "User-Agent";
			case header::Vary:
				return "Vary";
			case header::Via:
				return "Via";
			case header::Warning:
				return "Warning";
			case header::WWW_Authenticate:
				return "WWW-Authenticate";
			case header::Content_Disposition:
				return "Content-Disposition";
			case header::Origin:
				return "Origin";
			case header::Access_Control_Allow_Origin:
				return "Access-Control-Allow-Origin";
			case header::Access_Control_Allow_Credentials:
				return "Access-Control-Allow-Credentials";
			case header::Access_Control_Allow_Methods:
				return "Access-Control-Allow-Methods";
			case header::Access_Control_Allow_Headers:
				return "Access-Control-Allow-Headers";
			case header::Access_Control_Max_Age:
				return "Access-Control-Max-Age";
			case header::Access_Control_Expose_Headers:
				return "Access-Control-Expose-Headers";
			case header::Link:
				return "Link";
			case header::Sender:
				return "Sender";
			case header::To:
				return "To";
			case header::Resent_To:
				return "Resent-To";
			case header::CC:
				return "cc";
			case header::Resent_CC:
				return "Resent-cc";
			case header::BCC:
				return "bcc";
			case header::Resent_BCC:
				return "Resent-bcc";
			case header::Message_ID:
				return "Message-ID";
			case header::Resent_Message_ID:
				return "Resent-Message-ID";
			case header::In_Reply_To:
				return "In-Reply-To";
			case header::References:
				return "References";
			case header::Keywords:
				return "Keywords";
			case header::Subject:
				return "Subject";
			case header::Comments:
				return "Comments";
			case header::Encrypted:
				return "Encrypted";
			case header::Received:
				return "Received";
			case header::Resent_Reply_To:
				return "Resent-Reply-To";
			case header::Resent_From:
				return "Resent-From";
			case header::Resent_Sender:
				return "Resent-Sender";
			case header::Resent_Date:
				return "Resent-Date";
			case header::Return_Path:
				return "Return-Path";
			default:
				break;
		}

		return nullptr;
	}

	const char* header_key::name() const noexcept {
		switch (header_) {
			case header::empty:
				return nullptr;
			case header::extension_header:
				return user_defined_.c_str();
			default:
				break;
		}

		return name(header_);
	}

	std::string_view content_type_value::mime_type() const noexcept {
		return std::string_view{mime}.substr(0, slash_pos);
	}

	std::string_view content_type_value::mime_subtype() const noexcept {
		return std::string_view{mime}.substr(slash_pos + 1);
	}

	content_type_value headers::get_content_type() const {
		return impl::get_content_type(*this);
	}
}  // namespace tangle::nav
