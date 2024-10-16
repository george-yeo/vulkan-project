#pragma once

#include "oys_device.hpp"
#include "oys_model.hpp"
#include "oys_swap_chain.hpp"
#include "oys_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace oys {
	class OysRenderer {

	public:
		OysRenderer(OysWindow& widnow, OysDevice &device);
		~OysRenderer();

		OysRenderer(const OysRenderer&) = delete;
		OysRenderer& operator=(const OysRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return oysSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");

			return commandBuffers[currentImageIndex];
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		OysWindow& oysWindow;
		OysDevice& oysDevice;
		std::unique_ptr<OysSwapChain> oysSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		bool isFrameStarted{ false };
	};
} // namespace oys