// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <curl/curl.h>
#undef min
#undef max
#include <tangle/nav/doc_impl.hpp>
#include <tangle/nav/request.hpp>

namespace tangle::curl {
	using DocumentPtr = std::shared_ptr<nav::doc_impl>;

	struct StringList {
		void append(char const* value) {
			auto val = curl_slist_append(slist_.get(), value);
			if (!slist_) slist_.reset(val);
		}

		operator curl_slist*() { return slist_.get(); }

	private:
		struct free_all {
			void operator()(curl_slist* slist) { ::curl_slist_free_all(slist); }
		};
		using ptr = std::unique_ptr<curl_slist, free_all>;
		ptr slist_{};
	};

	class Curl {
	public:
		typedef unsigned long long size_type;

		Curl();
		~Curl();

		DocumentPtr getCallback() const;
		void setCallback(const DocumentPtr& callback);

		void followLocation();
		void dontFollowLocation();
		void setMaxRedirs(long redirs);
		void setConnectTimeout(long timeout);
		void setUrl(const std::string& url);
		void setUA(const std::string& userAgent);
		void setReferrer(std::string const& url);
		void setCookie(std::string const& cookie);
		void postForm(std::string const& form);
		void setHeaders(curl_slist* headers);
		void setPostData(const void* data, size_t length);
		void setSSLVerify(bool verify = true);
		void setWrite();
		void setProgress();
		void setDebug(std::shared_ptr<nav::request_trace> trace = {});
		CURLcode fetch();
		char const* effectiveLocation() const;

		bool isRedirect() const;
		void readResponseHeaders() const;

		class Tester;

	private:
		friend class Tester;

		static size_t curl_onData(const void* str,
		                          size_t size,
		                          size_t count,
		                          Curl* self);
		static size_t curl_onHeader(const void* str,
		                            size_t size,
		                            size_t count,
		                            Curl* self);
		static int curl_onProgress(Curl* self,
		                           double dltotal,
		                           double dlnow,
		                           double ultotal,
		                           double ulnow);
		static int curl_onTrace(CURL*,
		                        curl_infotype type,
		                        char* data,
		                        size_t size,
		                        Curl* self);

		size_type onData(const char* data, size_type length);
		size_type onHeader(const char* data, size_type length);
		bool onProgress(double, double, double, double);
		int onTrace(curl_infotype type, char* data, size_t size);

		CURL* m_curl{};
		bool m_headersLocked{true};
		bool m_wasRedirected{false};
		std::string m_finalLocation{};
		msg::http_response m_inParser{};
		std::weak_ptr<nav::doc_impl> m_callback{};
		std::shared_ptr<nav::request_trace> m_trace{};
	};

	namespace Transfer {
		template <bool equal>
		struct Data;

		template <>
		struct Data<true> {
			static Curl::size_type onData(const DocumentPtr& http_callback,
			                              const char* data,
			                              Curl::size_type length) {
				return http_callback->on_data(data,
				                              static_cast<size_t>(length));
			}
		};

		template <>
		struct Data<false> {
			static Curl::size_type onData(const DocumentPtr& http_callback,
			                              const char* data,
			                              Curl::size_type length) {
				Curl::size_type written = 0;
				while (length) {
					Curl::size_type chunk = static_cast<size_t>(-1);
					if (chunk > length) chunk = length;
					length -= chunk;
					size_t st_chunk = static_cast<size_t>(chunk);
					size_t ret = http_callback->on_data(data, st_chunk);
					data += st_chunk;
					written += ret;
					if (ret != st_chunk) break;
				}

				return written;
			}
		};

		inline Curl::size_type onData(const DocumentPtr& http_callback,
		                              const char* data,
		                              Curl::size_type length) {
			return Data<sizeof(Curl::size_type) == sizeof(size_t)>::onData(
			    http_callback, data, length);
		}
	}  // namespace Transfer

}  // namespace tangle::curl
