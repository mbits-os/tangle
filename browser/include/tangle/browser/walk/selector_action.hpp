// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace tangle::browser::walk {
	struct cursor;

	struct selector_action {
		std::string name;
		std::vector<std::string> args{};
		size_t hash{std::hash<std::string>{}(name)};

		friend std::ostream& operator<<(std::ostream& out,
		                                selector_action const& descr);

		static selector_action parse(std::string_view text);
	};

	struct action_callback {
		virtual ~action_callback();
		virtual bool on_action(selector_action const&, cursor&) = 0;

		template <typename Handler>
		static std::unique_ptr<action_callback> wrap(Handler handler) {
			struct callback : action_callback {
				Handler handler;
				callback(Handler&& handler) : handler{std::move(handler)} {}
				bool on_action(selector_action const& action,
				               cursor& cur) override {
					return handler(action, cur);
				}
			};

			return std::make_unique<callback>(std::move(handler));
		}
	};

	using action_callbacks =
	    std::unordered_map<std::string, std::unique_ptr<action_callback>>;
}  // namespace tangle::browser::walk
