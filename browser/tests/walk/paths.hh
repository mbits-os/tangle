// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/pages.hpp>
#include "cxx_string.hh"

using namespace std::literals;

namespace tangle::browser::walk::testing {
	using browser::testing::cxx_string;

	struct visit_data {
		struct header {
			std::string_view markup;
			std::string_view definition;
			std::string page_id{};
			bool result_during_walk{true};
		} hdr;
		std::vector<std::string_view> expected;
		std::string_view debug_output{};

		friend std::ostream& operator<<(std::ostream& out,
		                                visit_data const& test) {
			return out << test.hdr.definition << ' '
			           << cxx_string{test.hdr.markup, 60};
		}
	};

	std::string innerText(cursor& scope);
	std::string innerHTML(cursor& scope);
	std::string outerHTML(cursor& scope);
	std::string imgSrc(cursor& scope);
	std::string aHref(cursor& scope);
	std::string hrefText(cursor& scope);
	std::string textAfterMark(cursor& scope);

	inline std::unique_ptr<action_callback> wrap(
	    std::string (*modifier)(cursor&),
	    std::vector<std::string>* output) {
		auto callback = [=](selector_action const&, cursor& scope) {
			output->push_back(modifier(scope));
			return true;
		};
		return action_callback::wrap(callback);
	}

	inline action_callbacks setup(std::vector<std::string>* output) {
		action_callbacks callbacks{};
#define CB(NAME) callbacks[#NAME] = wrap(NAME, output)
		CB(innerText);
		CB(innerHTML);
		CB(outerHTML);
		CB(imgSrc);
		CB(aHref);
		CB(hrefText);
		CB(textAfterMark);
#undef CB
		return callbacks;
	}

	struct always_false : action_callback {
		bool on_action(selector_action const&, cursor&) override {
			return false;
		}
	};

}  // namespace tangle::browser::walk::testing