// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/cookie/io.hpp>
#include <algorithm>
#include <iterator>

namespace tangle::cookie::io::v1 {
	uint32_t string_bag::add(std::string const& value) {
		auto it = strings.lower_bound(value);
		if (it == strings.end() || it->first != value) {
			auto const result = length;
			length += static_cast<uint32_t>(value.length() + 1);
			strings.insert(it, {value, result});
			return result;
		}
		return it->second;
	}

	io::item string_bag::add(cookie::item const& item) {
		io::item out{};
		out.name = add(item.name());
		out.value = add(item.value());
		out.domain = add(item.scope().domain);
		out.path = add(item.scope().path);
		return out;
	}

	std::vector<string_bag::reversed> string_bag::sorted_strings() {
		std::vector<reversed> sorted;
		sorted.reserve(strings.size());
		std::transform(strings.begin(), strings.end(),
		               std::back_inserter(sorted), [](auto const& pair) {
			               return reversed{pair.second, pair.first};
		               });
		std::sort(sorted.begin(), sorted.end());
		return sorted;
	}
}  // namespace tangle::cookie::io::v1
