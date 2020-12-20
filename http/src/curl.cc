/*
 * Copyright (C) 2020 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <curl/curl.h>
#undef min
#undef max
#include <tangle/msg/http_parser.h>
#include <tangle/nav/http_doc_impl.h>
#include <tangle/nav/protocol.h>
#include <tangle/http/proto.hh>

using namespace std::literals;

namespace tangle::http::curl {
	using DocumentPtr = std::shared_ptr<nav::http_doc_impl>;

	struct StringList {
		void append(char const* value) {
			auto val = curl_slist_append(slist_.release(), value);
			slist_.reset(val);
		}

		operator curl_slist*() { return slist_.get(); }

	private:
		struct free_all {
			void operator()(curl_slist* slist) { ::curl_slist_free_all(slist); }
		};
		using ptr = std::unique_ptr<curl_slist, free_all>;
		ptr slist_{};
	};

	template <typename Final>
	struct CurlBase {
		typedef unsigned long long size_type;

		CURL* m_curl;

		CurlBase() : m_curl(nullptr) { m_curl = curl_easy_init(); }
		virtual ~CurlBase() {
			if (m_curl) curl_easy_cleanup(m_curl);
			m_curl = nullptr;
		}
		explicit operator bool() const { return m_curl != nullptr; }

		void followLocation() {
			curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(m_curl, CURLOPT_AUTOREFERER, 1L);
		}

		void dontFollowLocation() {
			curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 0L);
			curl_easy_setopt(m_curl, CURLOPT_AUTOREFERER, 0L);
			curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, -1L);
		}

		void setMaxRedirs(long redirs) {
			curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, redirs);
		}

		void setConnectTimeout(long timeout) {
			curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, timeout);
		}

		void setUrl(const std::string& url) {
			curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
		}

		void setUA(const std::string& userAgent) {
			curl_easy_setopt(m_curl, CURLOPT_USERAGENT, userAgent.c_str());
		}

		void setReferrer(std::string const& url) {
			curl_easy_setopt(m_curl, CURLOPT_REFERER, url.c_str());
		}

		void setCookie(std::string const& cookie) {
			curl_easy_setopt(m_curl, CURLOPT_COOKIE, cookie.c_str());
		}

		void postForm(std::string const& form) {
			curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
			curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, form.c_str());
		}

		void setHeaders(curl_slist* headers) {
			curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);
		}

		void setPostData(const void* data, size_t length) {
			curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, length);
			curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data);
		}

		void setSSLVerify(bool verify = true) {
			long val = verify ? 1 : 0;
			curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, val);
			curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, val);
			// curl_easy_setopt(m_curl, CURLOPT_CERTINFO, 1L);
			// curl_easy_setopt(m_curl, CURLOPT_SSL_CTX_FUNCTION,
			// curl_ssl_ctx_callback);
		}

		void setWrite() {
			auto _this = static_cast<Final*>(this);
			curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, _this);
			curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, curl_onHeader);
			curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, _this);
			curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, curl_onData);
		}

		void setProgress() {
			curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
			curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, curl_onProgress);
			curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA,
			                 static_cast<Final*>(this));
		}

		void setDebug(bool debug = true) {
			curl_easy_setopt(m_curl, CURLOPT_VERBOSE, debug ? 1L : 0L);
			if (debug) {
				curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, curl_onTrace);
				curl_easy_setopt(m_curl, CURLOPT_DEBUGDATA,
				                 static_cast<Final*>(this));
			} else {
				curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, nullptr);
				curl_easy_setopt(m_curl, CURLOPT_DEBUGDATA, nullptr);
			}
		}

		CURLcode fetch() { return curl_easy_perform(m_curl); }

	protected:
		size_type onData(const char* data, size_type length) { return 0; }
		size_type onHeader(const char* data, size_type length) { return 0; }
		size_type onUnderflow(void* data, size_type length) { return 0; }
		bool onProgress(double dltotal,
		                double dlnow,
		                double ultotal,
		                double ulnow) {
			return true;
		}
		int onTrace(curl_infotype type, char* data, size_t size) { return 0; }

	private:
#define CURL_(name)                                                          \
	static size_t curl_##name(const void* _Str, size_t _Size, size_t _Count, \
	                          Final* _this) {                                \
		return (size_t)(                                                     \
		    _this->name((const char*)_Str, (size_type)_Size * _Count) /      \
		    _Size);                                                          \
	}
		CURL_(onData);
		CURL_(onHeader);
#undef CURL_
		static size_t curl_onUnderflow(void* _Str,
		                               size_t _Size,
		                               size_t _Count,
		                               Final* _this) {
			return (size_t)(
			    _this->onUnderflow(_Str, (size_type)_Size * _Count) / _Size);
		}
		static int curl_onProgress(Final* _this,
		                           double dltotal,
		                           double dlnow,
		                           double ultotal,
		                           double ulnow) {
			return _this->onProgress(dltotal, dlnow, ultotal, ulnow) ? 1 : 0;
		}
		static int curl_onTrace(CURL* handle,
		                        curl_infotype type,
		                        char* data,
		                        size_t size,
		                        Final* _this) {
			return _this->onTrace(type, data, size);
		}
	};

	class Curl : public CurlBase<Curl> {
		bool m_headersLocked;
		bool m_wasRedirected;
		std::string m_finalLocation;
		msg::http_response m_inParser;
		std::weak_ptr<nav::http_doc_impl> m_callback;

	public:
		explicit Curl() : m_headersLocked(true), m_wasRedirected(false) {}

		DocumentPtr getCallback() const { return m_callback.lock(); }
		void setCallback(const DocumentPtr& callback) { m_callback = callback; }

		void setUrl(const std::string& url) {
			m_finalLocation = url;
			CurlBase<Curl>::setUrl(url);
		}

		inline bool isRedirect() const;
		void sendHeaders() const;

		size_type onData(const char* data, size_type length);
		size_type onHeader(const char* data, size_type length);
		size_type onUnderflow(void* data, size_type length) {
			return 0;
		}  // still not implemented
		inline bool onProgress(double, double, double, double);
		int onTrace(curl_infotype type, char* data, size_t size);
	};

	inline bool Curl::onProgress(double, double, double, double) {
		return false;
	}

	namespace Transfer {
		template <bool equal>
		struct Data {
			static Curl::size_type onData(const DocumentPtr& http_callback,
			                              const char* data,
			                              Curl::size_type length);
		};

		template <>
		struct Data<true> {
			static Curl::size_type onData(const DocumentPtr& http_callback,
			                              const char* data,
			                              Curl::size_type length) {
				return http_callback->on_data(data, (size_t)length);
			}
		};

		template <>
		struct Data<false> {
			static Curl::size_type onData(const DocumentPtr& http_callback,
			                              const char* data,
			                              Curl::size_type length) {
				Curl::size_type written = 0;
				while (length) {
					Curl::size_type chunk = (size_t)-1;
					if (chunk > length) chunk = length;
					length -= chunk;
					size_t st_chunk = (size_t)chunk;
					size_t ret = http_callback->on_data(data, st_chunk);
					data += st_chunk;
					written += ret;
					if (ret != st_chunk) break;
				}

				return written;
			}
		};

		static Curl::size_type onData(const DocumentPtr& http_callback,
		                              const char* data,
		                              Curl::size_type length) {
			return Data<sizeof(Curl::size_type) == sizeof(size_t)>::onData(
			    http_callback, data, length);
		}
	};  // namespace Transfer

	inline bool Curl::isRedirect() const {
		return nav::http_doc_impl::is_redirect(m_inParser.status());
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
			if (!isRedirect()) sendHeaders();
		}
		return size;
	}

	void Curl::sendHeaders() const {
		auto callback = getCallback();
		if (!callback) return;

		if (m_wasRedirected) callback->on_final_location(m_finalLocation);

		callback->on_headers(m_inParser.reason(), m_inParser.status(),
		                     m_inParser.dict());
	}

	int Curl::onTrace(curl_infotype type, char* data, size_t size) {
		const char* text = nullptr;

		switch (type) {
			case CURLINFO_TEXT:
				// text = "TEXT";
				fprintf(stderr, "# %s", data);
				return 0;
			default: /* in case a new one is introduced to shock us */
				fprintf(stderr, "trace(%d)\n", type);
				return 0;

			case CURLINFO_HEADER_OUT:
				// text = "HEADER_OUT";
				break;
			case CURLINFO_DATA_OUT:
				// text = "DATA_OUT";
				break;
			case CURLINFO_SSL_DATA_OUT:
				text = "=> Send SSL data";
				return 0;
				break;
			case CURLINFO_HEADER_IN:
				// text = "HEADER_IN";
				break;
			case CURLINFO_DATA_IN:
				// text = "DATA_IN";
				break;
			case CURLINFO_SSL_DATA_IN:
				text = "<= Recv SSL data";
				return 0;
				break;
		}

		if (text) {
			fprintf(stderr, "%s, %ld bytes (0x%lx)\n", text, (long)size,
			        (long)size);
		}
		// fwrite(data, 1, size, stderr);

		return 0;
	}

	struct protocol : nav::protocol {
		cache::document open(const nav::request& req,
		                     nav::navigator& nav) override {
			auto const addr = [](uri addr) {
				addr.fragment({});
				return addr;
			}(req.address());

			auto cookies = nav.cookies().get(addr, addr.scheme() == "https"sv);

			auto document = std::make_shared<nav::http_doc_impl>(addr, &nav);

			Curl curl{};

			curl.setCallback(document);
			curl.setConnectTimeout(5);
			curl.setUA(req.custom_agent().empty() ? nav.user_agent()
			                                      : req.custom_agent());
			curl.setProgress();
			curl.setSSLVerify(false);
			curl.setWrite();

			if (req.follow_redirects()) {
				curl.followLocation();
				curl.setMaxRedirs(req.max_redir());
				auto const redirs = req.max_redir();
				if (redirs != 0) curl.setMaxRedirs(redirs);
			} else {
				curl.dontFollowLocation();
			}

			curl.setUrl(addr.string());
			if (!req.referrer().string().empty())
				curl.setReferrer(req.referrer().string());
			if (!cookies.empty()) {
				// std::cerr << addr.string() << " --> " << cookies << "\n";
				curl.setCookie(cookies);
			}

			StringList headers{};
			if (!req.headers().empty()) {
				for (auto const& [name, value] : req.headers()) {
					std::string combined{};
					combined.reserve(name.size() + value.size() + 1);
					combined.append(name);
					combined.push_back(':');
					combined.append(value);
					headers.append(combined.c_str());
				}
				curl.setHeaders(headers);
			}

			if (req.method() == nav::method::post) {
				if (!req.form_fields().empty()) {
					curl.postForm(req.form_fields());
				}

				/*size_t length;
				void* content = http_callback->getContent(length);

				if (content && length)
				{
				    //curl_httppost; HTTPPOST_CALLBACK;
				    curl.setPostData(content, length);
				}*/
			}

			curl.setDebug(false);

			CURLcode ret = curl.fetch();
			if (curl.isRedirect())
				curl.sendHeaders();  // we must have hit max or a circular

			if (ret != CURLE_OK) document->on_error(ret);

			return cache::document::wrap(document);
		};
	};
}  // namespace tangle::http::curl

namespace tangle::http {
	std::shared_ptr<nav::protocol> proto() {
		curl_global_init(CURL_GLOBAL_ALL);
		return std::make_shared<curl::protocol>();
	}
}  // namespace tangle::http
