// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <tangle/nav/navigator.hpp>

namespace tangle::browser {
	struct fetch_interface {
		virtual ~fetch_interface() = 0;
		virtual nav::document fetch(nav::request const&) = 0;
	};

	class simple_fetch : public fetch_interface {
	public:
		simple_fetch(nav::navigator const& nav);
		nav::document fetch(nav::request const&) override;

	protected:
		virtual nav::request modify(nav::request const&);

		nav::navigator nav;
	};

	class basic_auth_fetch : public simple_fetch {
	public:
		basic_auth_fetch(std::string&& username,
		                 std::string&& secret,
		                 nav::navigator const& nav);

		std::string const& username() const noexcept { return username_; }

	protected:
		virtual nav::request modify(nav::request const&);

	private:
		std::string username_;
		std::string secret_;
	};
}  // namespace tangle::browser