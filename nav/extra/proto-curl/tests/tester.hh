// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/http/curl.hpp>

namespace tangle::http::curl {
	class Curl::Tester {
	public:
		static CURL* curl(Curl& ref) { return ref.m_curl; }

		static size_t curl_onData(const void* str,
		                          size_t size,
		                          size_t count,
		                          Curl* self) {
			return Curl::curl_onData(str, size, count, self);
		}

		static size_t curl_onHeader(const void* str,
		                            size_t size,
		                            size_t count,
		                            Curl* self) {
			return Curl::curl_onHeader(str, size, count, self);
		}

		static int curl_onProgress(Curl* self,
		                           double dltotal,
		                           double dlnow,
		                           double ultotal,
		                           double ulnow) {
			return Curl::curl_onProgress(self, dltotal, dlnow, ultotal, ulnow);
		}

		static int curl_onTrace(CURL* curl,
		                        curl_infotype type,
		                        char* data,
		                        size_t size,
		                        Curl* self) {
			return Curl::curl_onTrace(curl, type, data, size, self);
		}

		static size_type onData(Curl* self,
		                        const char* data,
		                        size_type length) {
			return self->onData(data, length);
		}

		static size_type onHeader(Curl* self,
		                          const char* data,
		                          size_type length) {
			return self->onHeader(data, length);
		}

		static bool onProgress(Curl* self,
		                       double dltotal,
		                       double dlnow,
		                       double ultotal,
		                       double ulnow) {
			return self->onProgress(dltotal, dlnow, ultotal, ulnow);
		}

		static int onTrace(Curl* self,
		                   curl_infotype type,
		                   char* data,
		                   size_t size) {
			return self->onTrace(type, data, size);
		}
	};
}  // namespace tangle::http::curl