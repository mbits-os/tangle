// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include <tangle/browser/walk/cursor.hpp>
#include <tangle/browser/walk/std_actions.hpp>
#include <tangle/str.hpp>

namespace tangle::browser::walk {
	namespace actions {
		std::string varname(std::vector<std::string> const& args) {
			if (args.empty()) {
				return {};
			}
			return args.front();
		}

		bool text(std_action_env* env,
		          std::vector<std::string> const& args,
		          cursor& scope) {
			auto value = scope.innerText();
			return env->set_string(varname(args), value);
		}

		bool link(std_action_env* env,
		          std::vector<std::string> const& args,
		          cursor& scope) {
			auto value = scope.attr("a"sv, "href"sv);
			return env->set_string(varname(args), value);
		}

		bool link_text(std_action_env* env,
		               std::vector<std::string> const& args,
		               cursor& scope) {
			auto ht = scope.aHrefText();
			return env->set_href_text(varname(args), ht);
		}

		bool image(std_action_env* env,
		           std::vector<std::string> const& args,
		           cursor& scope) {
			auto value = scope.attr("img"sv, "src"sv);
			return env->set_string(varname(args), value);
		}

		bool image_srcset(std_action_env* env,
		                  std::vector<std::string> const& args,
		                  cursor& scope) {
			auto value = scope.attr("img"sv, "srcset"sv);
			return env->set_string(varname(args), value);
		}

		bool outer_html(std_action_env* env,
		                std::vector<std::string> const& args,
		                cursor& scope) {
			auto value = scope.text;
			return env->set_string(varname(args), value);
		}

		bool inner_html(std_action_env* env,
		                std::vector<std::string> const& args,
		                cursor& scope) {
			auto value = scope.innerHTML().text;
			return env->set_string(varname(args), value);
		}

		using Handler = bool (*)(std_action_env*,
		                         std::vector<std::string> const&,
		                         cursor&);
		template <Handler handler>
		struct standard : action_callback {
			std_action_env* env;

			standard(std_action_env* env) : env{env} {}

			bool on_action(selector_action const& action,
			               cursor& scope) override {
				return handler(env, action.args, scope);
			}
		};
	}  // namespace actions

	std_action_env::~std_action_env() = default;

	template <actions::Handler handler>
	std::unique_ptr<action_callback> make_std(std_action_env* env) {
		return std::make_unique<actions::standard<handler>>(env);
	}

	void add_std_actions(action_callbacks& callbacks, std_action_env* env) {
		callbacks["text"] = make_std<actions::text>(env);
		callbacks["link"] = make_std<actions::link>(env);
		callbacks["linkText"] = make_std<actions::link_text>(env);
		callbacks["image"] = make_std<actions::image>(env);
		callbacks["srcset"] = make_std<actions::image_srcset>(env);
		callbacks["outerHTML"] = make_std<actions::outer_html>(env);
		callbacks["innerHTML"] = make_std<actions::inner_html>(env);
	}
}  // namespace tangle::browser::walk
