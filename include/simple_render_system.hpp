#pragma once

#include "oys_device.hpp"
#include "oys_game_object.hpp"
#include "oys_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace oys {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(OysDevice& oysDevice, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<OysGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		OysDevice& oysDevice;

		std::unique_ptr<OysPipeline> oysPipeline;
		VkPipelineLayout pipelineLayout;
	};
} // namespace oys