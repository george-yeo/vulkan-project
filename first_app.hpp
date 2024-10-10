#pragma once

#include "oys_window.hpp"

namespace oys {
	class FirstApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		OysWindow oysWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };

	};
} // namespace oys