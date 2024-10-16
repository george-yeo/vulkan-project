#pragma once

#include "oys_device.hpp"
#include "oys_game_object.hpp"
#include "oys_renderer.hpp"
#include "oys_window.hpp"

// std
#include <memory>
#include <vector>

namespace oys {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		OysWindow oysWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		OysDevice oysDevice{ oysWindow };
		OysRenderer oysRenderer{ oysWindow, oysDevice };
		
		std::vector<OysGameObject> gameObjects;
	};
} // namespace oys