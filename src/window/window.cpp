#include "window/window.hpp"

//std
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace oys {

	Window::Window(int width, int height, std::string name)
		: m_width{ width }, m_height{ height }, m_windowName(name) {
		init();
	}

	void Window::cleanup() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::init() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);
		// glfwSetFramebufferSizeCallback(m_window, frameBufferResizeCallback);
	}

	void Window::createSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}
} // namespace oys