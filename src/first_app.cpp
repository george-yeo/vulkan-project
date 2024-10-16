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
		createPipeline();
	}

	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(oysDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run() {
		while (!oysWindow.shouldClose()) {
			glfwPollEvents();
			
			if (auto commandBuffer = oysRenderer.beginFrame()) {
				oysRenderer.beginSwapChainRenderPass(commandBuffer);
				renderGameObjects(commandBuffer);
				oysRenderer.endSwapChainRenderPass(commandBuffer);
				oysRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(oysDevice.device());
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
		assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		OysPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = oysRenderer.getSwapChainRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		oysPipeline = std::make_unique<OysPipeline>(
			oysDevice,
			"../shaders/simple_shader.vert.spv",
			"../shaders/simple_shader.frag.spv",
			pipelineConfig);
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
} // namespace oys