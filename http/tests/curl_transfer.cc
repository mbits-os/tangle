// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <gtest/gtest.h>
#include <tangle/http/curl.hpp>

using namespace std::literals;

namespace tangle::http::curl::Transfer::testing {
	static constexpr std::string_view raven[] = {
	    "Then this ebony bird beguiling my sad fancy into smiling,\n"sv,
	    "By the grave and stern decorum of the countenance it wore,\n"sv,
	    "\"Though thy crest be shorn and shaven, thou,\" I said, \"art sure no craven,\n"sv,
	    "Ghastly grim and ancient Raven wandering from the Nightly shore-\n"sv,
	    "Tell me what thy lordly name is on the Night’s Plutonian shore!\"\n"sv,
	    "Quoth the Raven \"Nevermore.\""sv,
	};

	TEST(transfer, generic) {
		auto doc = std::make_shared<doc_impl>(uri{}, nullptr);

		for (auto const& verse : raven)
			onData(doc, verse.data(), verse.size());

		std::string expected{};
		{
			size_t length = 0;
			for (auto const& verse : raven)
				length += verse.size();
			expected.reserve(length);

			for (auto const& verse : raven)
				expected.append(verse);
		}

		EXPECT_EQ(doc->text(), expected);
        auto wrapped = nav::document::wrap(std::move(doc));
		EXPECT_EQ(wrapped.text(), expected);
		EXPECT_EQ(wrapped.moveable_text(), expected);
	}

	TEST(transfer, same_size) {
		auto doc = std::make_shared<doc_impl>(uri{}, nullptr);

		for (auto const& verse : raven)
			Data<true>::onData(doc, verse.data(), verse.size());

		std::string expected{};
		{
			size_t length = 0;
			for (auto const& verse : raven)
				length += verse.size();
			expected.reserve(length);

			for (auto const& verse : raven)
				expected.append(verse);
		}

		EXPECT_EQ(doc->text(), expected);
	}

	TEST(transfer, different_size) {
		auto doc = std::make_shared<doc_impl>(uri{}, nullptr);

		for (auto const& verse : raven)
			Data<false>::onData(doc, verse.data(), verse.size());

		std::string expected{};
		{
			size_t length = 0;
			for (auto const& verse : raven)
				length += verse.size();
			expected.reserve(length);

			for (auto const& verse : raven)
				expected.append(verse);
		}

		EXPECT_EQ(doc->text(), expected);
	}
}  // namespace tangle::http::curl::Transfer::testing
