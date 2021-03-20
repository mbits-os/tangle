#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace tangle::browser {
	struct selector_action {
		std::string name;
		std::vector<std::string> args{};
		size_t hash{std::hash<std::string>{}(name)};

		friend std::ostream& operator<<(std::ostream& out,
		                                selector_action const& descr);

		static selector_action parse(std::string_view text);
	};
}  // namespace tangle::browser
