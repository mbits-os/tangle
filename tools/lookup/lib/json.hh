// Copyright (c) 2020 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace json {
	struct value : std::variant<std::monostate,
	                            std::string,
	                            bool,
	                            long long,
	                            double,
	                            std::vector<value>,
	                            std::map<std::string, value>> {
		using base_type = std::variant<std::monostate,
		                               std::string,
		                               bool,
		                               long long,
		                               double,
		                               std::vector<value>,
		                               std::map<std::string, value>>;
		using base_type::base_type;
		using array = std::vector<value>;
		using object = std::map<std::string, value>;

		value() : base_type(std::in_place_type<std::monostate>) {}

		value(const char* str)
		    : base_type(std::in_place_type<std::string>, str) {}

		base_type const& base() const& noexcept { return *this; };
		base_type& base() & noexcept { return *this; };
		base_type&& base() && noexcept { return std::move(*this); };
	};

	using array = std::vector<value>;
	using object = std::map<std::string, value>;

	value read_json(std::string_view text);

	template <typename Kind>
	static inline Kind* cast(value& jsn) {
		if (!std::holds_alternative<Kind>(jsn)) return nullptr;
		return &std::get<Kind>(jsn);
	}

	template <typename Kind>
	static inline Kind const* cast(value const& jsn) {
		if (!std::holds_alternative<Kind>(jsn)) return nullptr;
		return &std::get<Kind>(jsn);
	}

	template <typename Kind>
	static inline Kind* cast(object& jsn, std::string const& key) {
		auto it = jsn.find(key);
		if (it == jsn.end()) return nullptr;
		return cast<Kind>(it->second);
	}

	template <typename Kind>
	static inline Kind const* cast(object const& jsn, std::string const& key) {
		auto it = jsn.find(key);
		if (it == jsn.end()) return nullptr;
		return cast<Kind>(it->second);
	}

	template <typename Kind>
	static inline Kind* cast(value* jsn) {
		if (!jsn) return nullptr;
		return cast<Kind>(*jsn);
	}

	template <typename Kind>
	static inline Kind const* cast(value const* jsn) {
		if (!jsn) return nullptr;
		return cast<Kind>(*jsn);
	}

	template <typename Kind>
	static inline Kind* cast(object* jsn, std::string const& key) {
		if (!jsn) return nullptr;
		return cast<Kind>(*jsn, key);
	}

	template <typename Kind>
	static inline Kind const* cast(object const* jsn, std::string const& key) {
		if (!jsn) return nullptr;
		return cast<Kind>(*jsn, key);
	}

	template <typename Kind>
	static inline Kind* cast(value& jsn, std::string const& key) {
		return cast<Kind>(cast<object>(jsn), key);
	}

	template <typename Kind>
	static inline Kind const* cast(value const& jsn, std::string const& key) {
		return cast<Kind>(cast<object>(jsn), key);
	}

	template <typename Kind>
	static inline Kind* cast(value* jsn, std::string const& key) {
		return cast<Kind>(cast<object>(jsn), key);
	}

	template <typename Kind>
	static inline Kind const* cast(value const* jsn, std::string const& key) {
		return cast<Kind>(cast<object>(jsn), key);
	}

	value* from_json(object& jsn, std::string_view path);
	value const* from_json(object const& jsn, std::string_view path);

	static inline value* from_json(object* jsn, std::string_view path) {
		if (!jsn) return nullptr;
		return from_json(*jsn, path);
	}

	static inline value const* from_json(object const* jsn,
	                                     std::string_view path) {
		if (!jsn) return nullptr;
		return from_json(*jsn, path);
	}

	static inline value* from_json(value& jsn, std::string_view path) {
		return from_json(cast<object>(jsn), path);
	}

	static inline value const* from_json(value const& jsn,
	                                     std::string_view path) {
		return from_json(cast<object>(jsn), path);
	}

	static inline value* from_json(value* jsn, std::string_view path) {
		return from_json(cast<object>(jsn), path);
	}

	static inline value const* from_json(value const* jsn,
	                                     std::string_view path) {
		return from_json(cast<object>(jsn), path);
	}

	template <typename Kind>
	static inline Kind* cast_from_json(object& jsn, std::string const& path) {
		return cast<Kind>(from_json(jsn, path));
	}

	template <typename Kind>
	static inline Kind const* cast_from_json(object const& jsn,
	                                         std::string const& path) {
		return cast<Kind>(from_json(jsn, path));
	}

	template <typename Kind>
	static inline Kind* cast_from_json(object* jsn, std::string const& path) {
		return cast<Kind>(from_json(jsn, path));
	}

	template <typename Kind>
	static inline Kind const* cast_from_json(object const* jsn,
	                                         std::string const& path) {
		return cast<Kind>(from_json(jsn, path));
	}

	template <typename Kind>
	static inline Kind* cast_from_json(value& jsn, std::string const& path) {
		auto result = from_json(jsn, path);
		if (!result) return nullptr;
		return cast<Kind>(*result);
	}

	template <typename Kind>
	static inline Kind const* cast_from_json(value const& jsn,
	                                         std::string const& path) {
		auto result = from_json(jsn, path);
		if (!result) return nullptr;
		return cast<Kind>(*result);
	}

	template <typename Kind>
	static inline Kind* cast_from_json(value* jsn, std::string const& path) {
		if (!jsn) return nullptr;
		return cast_from_json<Kind>(*jsn, path);
	}

	template <typename Kind>
	static inline Kind const* cast_from_json(value const* jsn,
	                                         std::string const& path) {
		if (!jsn) return nullptr;
		return cast_from_json<Kind>(*jsn, path);
	}
}  // namespace json
