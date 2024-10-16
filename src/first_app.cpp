#include "first_app.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "simple_render_system.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace oys {

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ oysDevice, oysRenderer.getSwapChainRenderPass() };

		while (!oysWindow.shouldClose()) {
			glfwPollEvents();
			
			if (auto commandBuffer = oysRenderer.beginFrame()) {
				oysRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
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
} // namespace oys