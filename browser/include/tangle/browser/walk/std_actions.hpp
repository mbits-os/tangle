// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/browser/walk/selector_action.hpp>

namespace tangle::browser::walk {
	struct std_action_env {
		virtual ~std_action_env();

		virtual bool set_string(std::string const& name,
		                        std::string_view value) = 0;
		virtual bool set_href_text(std::string const& name,
		                           href_text const&) = 0;
	};

	void add_std_actions(action_callbacks&, std_action_env* env);

	template <typename Final, typename Result, typename ParsedPages>
	struct page_matcher : std_action_env {
		Result result{};

		bool set_string(std::string const&, std::string_view) override {
			return false;
		}

		bool set_href_text(std::string const&, href_text const&) override {
			return false;
		}

		static std::ostream* debug() noexcept { return nullptr; }

		void setup_actions(action_callbacks& actions) {
			add_std_actions(actions, this);
		}

		Result visit(std::string_view text) {
			walk::action_callbacks actions{};
			static_cast<Final*>(this)->setup_actions(actions);
			auto const& defs = ParsedPages::get_pages();

			walk::root_cursor cur{text};
			if (!defs.visit(cur.root(), Final::page_name(), actions, nullptr,
			                Final::debug())) {
				this->result = Result{};
			}

			return std::move(this->result);
		}
	};
}  // namespace tangle::browser::walk
