// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "flask-app.hh"
#include <stdlib.h>
#include <chrono>
#include "flask-app-dir.hh"

#ifdef _WIN32
#include <Windows.h>
#undef max
#undef min
#endif

using namespace std::literals;

namespace tangle::http::flask {
	app::app() = default;

	app::~app() { stop(); }

	void app::start() {
		stop();
		th_ = std::thread{[](app* self) { self->run(); }, this};
		stage_ = 2;
		cv_.notify_one();

		std::unique_lock lck{m_};
		auto self = this;
		cv_.wait(lck, [&] { return self->stage_ == 3; });
	}

	void app::stop() {
		if (process_) {
			fprintf(stderr, "Stopping flask\n");
			process_->kill();
		}
		if (th_.joinable()) th_.join();

		th_ = std::thread{};
		process_.reset();
		stage_ = 0;
	}

#ifdef _WIN32
	inline void setenv(char const* name, char const* value, int) {
		SetEnvironmentVariableA(name, value);
	}
#endif

	void app::run() {
		setenv("FLASK_APP", FLASK_APP, 1);
		fprintf(stderr, "Starting flask\n");
		process_ = std::make_unique<Process>("flask run", std::string{});
		fprintf(stderr, "Started flask\n");
		std::this_thread::sleep_for(1s);
		{
			std::unique_lock lck{m_};
			auto self = this;
			cv_.wait(lck, [&] { return self->stage_ == 2; });
		}
		stage_ = 3;
		cv_.notify_one();

		auto result = process_->get_exit_status();
	}
}  // namespace tangle::http::flask
