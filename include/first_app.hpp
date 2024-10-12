#pragma once

#include "oys_device.hpp"
#include "oys_pipeline.hpp"
#include "oys_swap_chain.hpp"
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
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		OysWindow oysWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		OysDevice oysDevice{ oysWindow };
		OysSwapChain oysSwapChain{ oysDevice, oysWindow.getExtent() };
		std::unique_ptr<OysPipeline> oysPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
} // namespace oys