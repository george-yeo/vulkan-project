#include "oys_window.hpp"

//std
#include <stdexcept>
#include <string>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace oys {

	OysWindow::OysWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName(name) {
		initWindow();
	}

	OysWindow::~OysWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void OysWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
	}

	void OysWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void OysWindow::frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto oysWindow = reinterpret_cast<OysWindow*>(glfwGetWindowUserPointer(window));
		oysWindow->frameBufferResized = true;
		oysWindow->width = width;
		oysWindow->height = height;
	}
} // namespace oys