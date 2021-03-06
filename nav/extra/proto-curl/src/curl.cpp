// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/curl/curl.hpp>
#include <tangle/curl/proto.hpp>
#include <tangle/msg/http_parser.hpp>
#include <tangle/nav/protocol.hpp>
#include <mutex>

using namespace std::literals;

namespace tangle::curl {
	Curl::Curl() : m_curl(curl_easy_init()) {}

	Curl::~Curl() {
		if (m_curl) curl_easy_cleanup(m_curl);
		m_curl = nullptr;
	}

	DocumentPtr Curl::getCallback() const { return m_callback.lock(); }
	void Curl::setCallback(const DocumentPtr& callback) {
		m_callback = callback;
	}

	void Curl::followLocation() {
		curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(m_curl, CURLOPT_AUTOREFERER, 1L);
	}

	void Curl::dontFollowLocation() {
		curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 0L);
		curl_easy_setopt(m_curl, CURLOPT_AUTOREFERER, 0L);
		curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, -1L);
	}

	void Curl::setAcceptEncoding(char const* value) {
		curl_easy_setopt(m_curl, CURLOPT_ACCEPT_ENCODING, value);
	}

	void Curl::setMaxRedirs(long redirs) {
		curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, redirs);
	}

	void Curl::setConnectTimeout(long timeout) {
		curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, timeout);
	}

	void Curl::setUrl(const std::string& url) {
		m_finalLocation = url;
		curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	}

	void Curl::setCookie(std::string const& cookie) {
		curl_easy_setopt(m_curl, CURLOPT_COOKIE, cookie.c_str());
	}

	void Curl::postForm(std::string const& form) {
		curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, form.c_str());
	}

	void Curl::setHeaders(curl_slist* headers) {
		curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);
	}

	void Curl::setPostData(const void* data, size_t length) {
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, length);
		curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data);
	}

	void Curl::setSSLVerify(bool verify) {
		long val = verify ? 1 : 0;
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, val);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, val);
		// curl_easy_setopt(m_curl, CURLOPT_CERTINFO, 1L);
		// curl_easy_setopt(m_curl, CURLOPT_SSL_CTX_FUNCTION,
		// curl_ssl_ctx_callback);
	}

	void Curl::setWrite() {
		curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, this);
		curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, curl_onHeader);
		curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, curl_onData);
	}

	void Curl::setDebug(std::shared_ptr<nav::request_trace> trace) {
		m_trace = std::move(trace);
		curl_easy_setopt(m_curl, CURLOPT_VERBOSE, m_trace ? 1L : 0L);
		if (m_trace) {
			curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, curl_onTrace);
			curl_easy_setopt(m_curl, CURLOPT_DEBUGDATA, this);
		} else {
			curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, nullptr);
			curl_easy_setopt(m_curl, CURLOPT_DEBUGDATA, nullptr);
		}
	}

	CURLcode Curl::fetch() { return curl_easy_perform(m_curl); }

	char const* Curl::effectiveLocation() const {
		char* url = nullptr;
		if (curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &url) != CURLE_OK)
			url = nullptr;  // GCOV_EXCL_LINE -- we do not test curl here...
		if (url && !*url) url = nullptr;
		return url;
	}

#if 0
	static void dbghex(const void* str, size_t size, size_t count) {
		constexpr size_t length = 32;
		static constexpr char alphabet[] = "0123456789abcdef";
		char line[length * 4 + 2];
		memset(line, ' ', sizeof(line));
		size_t ch = 0;
		auto chars = static_cast<char const*>(str);
		for (size_t i = 0; i < size; ++i) {
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
					fputs(line, stderr);
				}
			}
		}

		if (ch != 0) {
			for (size_t i = ch; i < length; ++i) {
				line[i * 3] = line[i * 3 + 1] = line[i * 3 + 2] =
				    line[length * 3 + i] = ' ';
			}

			line[length * 4] = '\n';
			line[length * 4 + 1] = 0;
			fputs(line, stderr);
		}
	}
#endif

	size_t Curl::curl_onData(const void* str,
	                         size_t size,
	                         size_t count,
	                         Curl* self) {
		return static_cast<size_t>(
		    self->onData(reinterpret_cast<const char*>(str),
		                 static_cast<size_type>(size) * count) /
		    size);
	}

	size_t Curl::curl_onHeader(const void* str,
	                           size_t size,
	                           size_t count,
	                           Curl* self) {
		return static_cast<size_t>(
		    self->onHeader(reinterpret_cast<const char*>(str),
		                   static_cast<size_type>(size) * count) /
		    size);
	}

	int Curl::curl_onTrace(CURL*,
	                       curl_infotype type,
	                       char* data,
	                       size_t size,
	                       Curl* self) {
		return self->onTrace(type, data, size);
	}

	inline bool Curl::isRedirect() const {
		return nav::doc_impl::is_redirect(m_inParser.status());
	}

	Curl::size_type Curl::onData(const char* data, size_type length) {
		// Redirects should not have bodies anyway
		// And if we redirect, there will be a new header soon...
		if (isRedirect()) return length;

		auto callback = getCallback();
		if (!callback) return 0;
		return Transfer::onData(callback, data, length);
	}

	Curl::size_type Curl::onHeader(const char* data, size_type length) {
		auto const [size, status] = m_inParser.append(data, length);
		if (status == msg::parsing::separator) {
			m_headersLocked = true;
			if (!isRedirect()) readResponseHeaders();
		}
		return size;
	}

	void Curl::readResponseHeaders() const {
		auto callback = getCallback();
		if (!callback) return;

		if (m_wasRedirected) callback->on_final_location(m_finalLocation);

		callback->on_headers(m_inParser.reason(), m_inParser.status(),
		                     m_inParser.dict());
	}

	int Curl::onTrace(curl_infotype type, char* data, size_t size) {
		auto ptr = m_trace;
		if (!ptr) return 0;
		auto& ref = *ptr;

		auto chars = std::string_view{data, size};

		switch (type) {
			case CURLINFO_SSL_DATA_IN:
				ref.ssl_data_out(chars);
				break;
			case CURLINFO_SSL_DATA_OUT:
				ref.ssl_data_in(chars);
				break;
			case CURLINFO_HEADER_OUT:
				ref.req_header(chars);
				break;
			case CURLINFO_DATA_OUT:
				ref.req_data(chars);
				break;
			case CURLINFO_HEADER_IN:
				ref.resp_header(chars);
				break;
			case CURLINFO_DATA_IN:
				ref.resp_data(chars);
				break;
			case CURLINFO_TEXT:
				ref.message(chars);
				[[fallthrough]];
			default:
				break;
		}

		return 0;
	}

	static StringList convert(nav::headers const& headers) {
		StringList result{};
		for (auto const& [key, values] : headers) {
			if (key == nav::header::Accept_Encoding) continue;

			auto raw_name = key.name();
			if (!raw_name) continue;

			auto name = std::string_view{raw_name};
			for (auto const& value : values) {
				std::string combined{};
				combined.reserve(name.size() + value.size() + 1);
				combined.append(name);
				combined.push_back(':');
				combined.append(value);
				result.append(combined.c_str());
			}
		}
		return result;
	}

	struct protocol : nav::protocol {
		nav::document open(const nav::request& req,
		                   nav::navigator& nav) override {
			auto const addr = [](uri addr) {
				addr.fragment({});
				return addr;
			}(req.address());

			auto document = std::make_shared<nav::doc_impl>(addr, &nav);

			Curl curl{};

			curl.setCallback(document);
			curl.setConnectTimeout(5);
			curl.setSSLVerify(false);
			curl.setWrite();
			curl.setUrl(addr.string());

			if (req.follow_redirects()) {
				curl.followLocation();
				if (auto const redirs = req.max_redir(); redirs > 0)
					curl.setMaxRedirs(redirs);
			} else {
				curl.dontFollowLocation();
			}

			if (auto cookies =
			        nav.cookies().get(addr, addr.scheme() == "https"sv);
			    !cookies.empty())
				curl.setCookie(cookies);

			auto headers = convert(req.headers());

			if (auto custom_user_agent =
			        req.headers().find_front(nav::header::User_Agent);
			    !custom_user_agent && !nav.user_agent().empty()) {
				headers.append(("User-Agent:" + nav.user_agent()).c_str());
			}

			{
				auto accept_encoding =
				    req.headers().find_front(nav::header::Accept_Encoding);
				curl.setAcceptEncoding(
				    accept_encoding ? accept_encoding->c_str() : nullptr);
			}

			if (headers) curl.setHeaders(headers.get());

			if (req.method() == nav::method::post) {
				if (!req.form_fields().empty()) {
					curl.postForm(req.form_fields());
				}

				auto const& content = req.content();
				if (!content.empty()) {
					curl.setPostData(content.c_str(), content.size());
				}
			}

			curl.setDebug(req.trace());

			CURLcode ret = curl.fetch();
			if (curl.isRedirect()) {
				// we must have hit max or a circular
				curl.readResponseHeaders();
			}

			if (ret != CURLE_OK)
				document->on_library_error(ret, curl_easy_strerror(ret));

			if (auto const effective = curl.effectiveLocation())
				document->on_final_location(effective);

			return nav::document::wrap(document);
		};
	};

	static std::once_flag inited;

	std::shared_ptr<nav::protocol> proto() {
		std::call_once(inited, curl_global_init, CURL_GLOBAL_ALL);
		return std::make_shared<protocol>();
	}
}  // namespace tangle::curl
