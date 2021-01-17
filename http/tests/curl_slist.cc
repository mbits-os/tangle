// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/http/curl.hpp>

using namespace std::literals;

namespace tangle::http::curl::testing {
	TEST(slist, append) {
		StringList strings{};

		static constexpr std::string_view raven[] = {
		    "Then this ebony bird beguiling my sad fancy into smiling,"sv,
		    "By the grave and stern decorum of the countenance it wore,"sv,
		    "\"Though thy crest be shorn and shaven, thou,\" I said, \"art sure no craven,"sv,
		    "Ghastly grim and ancient Raven wandering from the Nightly shore-"sv,
		    "Tell me what thy lordly name is on the Night’s Plutonian shore!\""sv,
		    "Quoth the Raven \"Nevermore.\""sv,
		};

		for (auto const& verse : raven)
			strings.append(verse.data());

		auto node = static_cast<curl_slist*>(strings);
		for (auto const& verse : raven) {
			if (!node) {
				auto result = ::testing::AssertionFailure()
				              << "Missing a verse in the slist";
				GTEST_NONFATAL_FAILURE_(result.failure_message());
				break;
			}

			EXPECT_EQ(verse, node->data);
			node = node->next;
		}
	}
}  // namespace tangle::http::curl::testing