#include "first_app.hpp"

#include "oys_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
        OysCamera camera{};

		while (!oysWindow.shouldClose()) {
			glfwPollEvents();

            float aspect = oysRenderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);
			
			if (auto commandBuffer = oysRenderer.beginFrame()) {
				oysRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				oysRenderer.endSwapChainRenderPass(commandBuffer);
				oysRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(oysDevice.device());
	}

	std::unique_ptr<OysModel> createCubeModel(OysDevice& device, glm::vec3 offset) {
        std::vector<OysModel::Vertex> vertices{

            // left face (white) - CCW
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow) - CCW
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange) - CCW
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red) - CCW
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue) - CCW
            {{-.5f, -.5f, .5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, .5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, .5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, .5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, .5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, .5f}, {.1f, .1f, .8f}},

            // tail face (green) - CCW
            {{-.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -.5f}, {.1f, .8f, .1f}},

        };
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<OysModel>(device, vertices);
	}

	void FirstApp::loadGameObjects() {
		std::shared_ptr<OysModel> oysModel = createCubeModel(oysDevice, { .0f, .0f, .0f });

		auto cube = OysGameObject::createGameObject();
		cube.model = oysModel;
		cube.transform.translation = { .0f, .0f, 2.5f };
		cube.transform.scale = { .5f,.5f,.5f };

		gameObjects.push_back(std::move(cube));
	}
} // namespace oys