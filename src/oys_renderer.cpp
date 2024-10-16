#include "oys_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace oys {

	OysRenderer::OysRenderer(OysWindow& window, OysDevice& device) : oysWindow{ window }, oysDevice{ device } {
		recreateSwapChain();
		createCommandBuffers();
	}

	OysRenderer::~OysRenderer() {
		freeCommandBuffers();
	}

	void OysRenderer::recreateSwapChain() {
		auto extent = oysWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = oysWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(oysDevice.device());

		if (oysSwapChain == nullptr) {
			oysSwapChain = std::make_unique<OysSwapChain>(oysDevice, extent);
		}
		else {
			oysSwapChain = std::make_unique<OysSwapChain>(oysDevice, extent, std::move(oysSwapChain));
			if (oysSwapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
	}

	void OysRenderer::createCommandBuffers() {
		commandBuffers.resize(oysSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = oysDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(oysDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	};

	void OysRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(
			oysDevice.device(),
			oysDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer OysRenderer::beginFrame() {
		assert(!isFrameStarted && "cannot call beginFrame while already in progress");

		auto result = oysSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		return commandBuffer;
	}

	void OysRenderer::endFrame() {
		assert(isFrameStarted && "cannot call endFrame when frame not in progress");

		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}

		auto result = oysSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || oysWindow.wasWindowResized()) {
			oysWindow.resetWindowResizeFlag();
			recreateSwapChain();
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}

		isFrameStarted = false;
	}

	void OysRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "cannot call beginSwapChainRenderPass when frame not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "cannot begin render pass on command buffer from different frame");
	
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = oysSwapChain->getRenderPass();
		renderPassInfo.framebuffer = oysSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = oysSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(oysSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(oysSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, oysSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void OysRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(isFrameStarted && "cannot call endSwapChainRenderPass when frame not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "cannot end render pass on command buffer from different frame");

		vkCmdEndRenderPass(commandBuffer);
	}
} // namespace oys