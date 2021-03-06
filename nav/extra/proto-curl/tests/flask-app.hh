// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#include "process.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

namespace tangle::flask {
	using namespace TinyProcessLib;

	class app {
	public:
		app();
		~app();

		void start();
		void stop();

	private:
		void run();

		std::thread th_{};
		std::unique_ptr<Process> process_{};
		std::mutex m_{};
		std::condition_variable cv_{};
		int stage_{0};
	};
}  // namespace tangle::flask