#include "first_app.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// std
#include <array>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <oys_model.hpp>
#include <oys_pipeline.hpp>
#include <oys_swap_chain.hpp>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <oys_game_object.hpp>

namespace oys {

#ifdef _MSC_VER
#pragma warning(disable: 4324) // disable warning 4324
#endif
	struct SimplePushConstantData {
		glm::mat2 transform{ 1.f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
#ifdef _MSC_VER
#pragma warning(default: 4324) // enable warning 4324 back
#endif

	FirstApp::FirstApp() {
		loadGameObjects();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(oysDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run() {
		while (!oysWindow.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(oysDevice.device());
	}

	void FirstApp::sierpinski(
		std::vector<OysModel::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top) {
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}

	void FirstApp::loadGameObjects() {
		std::vector<OysModel::Vertex> vertices{			
			{{ 0.0f, -0.5f }, {1.0f, 0.0f, 0.0f}},
			{{ 0.5f, 0.5f }, {0.0f, 1.0f, 0.0f}},
			{{ -0.5f, 0.5f }, {0.0f, 0.0f, 1.0f}}
		};

		/*sierpinski(vertices, 5, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });*/

		auto oysModel = std::make_shared<OysModel>(oysDevice, vertices);

		auto triangle = OysGameObject::createGameObject();
		triangle.model = oysModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}

	void FirstApp::createPipelineLayout() {
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(oysDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void FirstApp::createPipeline() {
		PipelineConfigInfo pipelineConfig{};
		OysPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = oysSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		oysPipeline = std::make_unique<OysPipeline>(
			oysDevice,
			"../shaders/simple_shader.vert.spv",
			"../shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::recreateSwapChain() {
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

		// if render pass is compatible do nothing else
		createPipeline();
	}

	void FirstApp::createCommandBuffers() {
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

	void FirstApp::freeCommandBuffers() {
		vkFreeCommandBuffers(
			oysDevice.device(),
			oysDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApp::recordCommandBuffer(size_t imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = oysSwapChain->getRenderPass();
		renderPassInfo.framebuffer = oysSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = oysSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(oysSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(oysSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, oysSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		renderGameObjects(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}

	void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
		oysPipeline->bind(commandBuffer);

		for (auto& obj : gameObjects) {
			obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.offset = obj.transform2d.translation;
			push.color = obj.color;
			push.transform = obj.transform2d.mat2();

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}

	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = oysSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image");
		}

		recordCommandBuffer(imageIndex);
		result = oysSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || oysWindow.wasWindowResized()) {
			oysWindow.resetWindowResizeFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	};
} // namespace oys