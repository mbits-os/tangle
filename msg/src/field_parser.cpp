// Copyright (c) 2016 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <algorithm>
#include <cctype>
#include <cstring>
#include <tangle/msg/field_parser.hpp>

namespace tangle::msg {
	namespace {
		template <typename V>
		auto find(V& contents, const char* sub) {
			auto cur = std::begin(contents);
			auto end = std::end(contents);

			if (!sub || !*sub) return end;

			auto len = std::strlen(sub);
			do {
				cur = std::find(cur, end, *sub);
				if (cur == end) break;
				if (size_t(end - cur) < len) break;
				if (!memcmp(&*cur, sub, len)) return cur;
				++cur;
			} while (true);
			return end;
		}

		inline size_t report_read(size_t prev, size_t position) {
			return (position > prev) ? position - prev : 0;
		}
	}  // namespace

	std::pair<size_t, parsing> field_parser::append(const char* data,
	                                                size_t length) {
		if (!length) return {0, parsing::reading};

		auto prev = m_contents.size();
		m_contents.insert(m_contents.end(), data, data + length);

		auto begin = std::begin(m_contents);
		auto cur = std::next(begin, static_cast<ptrdiff_t>(m_last_line_end));
		auto end = std::end(m_contents);

		while (cur != end) {
			auto it = std::find(cur, end, '\r');
			if (it == end) break;
			if (std::next(it) == end) break;
			if (*std::next(it) != '\n') {
				// mid-line \r? - check with RFC if ignore, or error
				return {report_read(prev, static_cast<size_t>(
				                              std::distance(begin, it))),
				        parsing::error};
			}

			if (it == cur) {  // empty line
				rearrange();
				return {report_read(prev, static_cast<size_t>(
				                              std::distance(begin, it))) +
				            2,
				        parsing::separator};
			}

			std::advance(it, 2);
			if (isspace(static_cast<uint8_t>(*cur))) {
				if (m_field_list.empty()) {
					return {report_read(prev, static_cast<size_t>(
					                              std::distance(begin, it))),
					        parsing::error};
				}

				m_last_line_end = static_cast<size_t>(std::distance(begin, it));
				auto& fld = std::get<1>(m_field_list.back());
				fld = span(fld.offset(), m_last_line_end - fld.offset());
			} else {
				auto colon = std::find(cur, it, ':');
				if (colon == it) {
					// no colon in field's first line
					return {report_read(prev, static_cast<size_t>(
					                              std::distance(begin, it))),
					        parsing::error};
				}

				m_last_line_end = static_cast<size_t>(std::distance(begin, it));
				m_field_list.emplace_back(
				    // GCOV_EXCL_START - 0 here, 175 times in next line
				    span(static_cast<size_t>(std::distance(begin, cur)),
				         // GCOV_EXCL_STOP
				         static_cast<size_t>(std::distance(cur, colon))),
				    span(static_cast<size_t>(std::distance(begin, colon) + 1),
				         static_cast<size_t>(std::distance(colon, it) - 1)));
			}

			cur = it;
		}

		return {length, parsing::reading};
	}

	std::string produce(std::string_view cs) {
		auto ptr = cs.data();
		auto len = cs.length();
		auto end = ptr + len;

		while (ptr != end && std::isspace(static_cast<uint8_t>(*ptr))) {
			++ptr;
			--len;
		}
		while (ptr != end && std::isspace(static_cast<uint8_t>(end[-1]))) {
			--end;
			--len;
		}
		cs = {ptr, len};

		bool in_cont = false;
		for (auto cur = ptr; cur != end; ++cur) {
			auto const uc = static_cast<uint8_t>(*cur);
			if (in_cont) {
				in_cont = !!std::isspace(uc);
				if (in_cont) --len;
				continue;
			}
			if (uc == '\r') {
				in_cont = true;
				continue;
			}
		}

		std::string out;
		out.reserve(len);

		in_cont = false;
		for (auto cur = ptr; cur != end; ++cur) {
			auto const uc = static_cast<uint8_t>(*cur);
			if (in_cont) {
				in_cont = !!std::isspace(uc);
				if (in_cont)
					--len;
				else
					out.push_back(static_cast<char>(uc));
				continue;
			}
			if (uc == '\r') {
				in_cont = true;
				while (ptr != cur &&
				       std::isspace(static_cast<uint8_t>(cur[-1]))) {
					--cur;
					out.pop_back();
				}
				out.push_back(' ');
				continue;
			}
			out.push_back(static_cast<char>(uc));
		}

		out.shrink_to_fit();
		return out;
	}

	std::string lower(std::string&& in) {
		for (auto& c : in)
			c = static_cast<char>(std::tolower(static_cast<uint8_t>(c)));
		return in;
	}

	void field_parser::rearrange() {
		m_dict.clear();

		for (auto& pair : m_field_list) {
			auto key = lower(produce(get(std::get<0>(pair))));
			auto value = produce(get(std::get<1>(pair)));
			m_dict[std::move(key)].emplace_back(std::move(value));
		}

		m_field_list.clear();
		m_contents.clear();
		m_contents.shrink_to_fit();
		m_headers_seen = true;
		m_last_line_end = 0;
	}
}  // namespace tangle::msg
