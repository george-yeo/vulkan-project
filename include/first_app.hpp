#pragma once

#include "oys_device.hpp"
#include "oys_model.hpp"
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
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(size_t imageIndex);

		void sierpinski(
			std::vector<OysModel::Vertex>& vertices,
			int depth,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 top);

		OysWindow oysWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		OysDevice oysDevice{ oysWindow };
		std::unique_ptr<OysSwapChain> oysSwapChain;
		std::unique_ptr<OysPipeline> oysPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<OysModel> oysModel;
	};
} // namespace oys