#pragma once

#define GLFW_INCLUDE_VULKAN
#pragma once

#include <GLFW/glfw3.h>

// std
#include <string>

namespace oys {

	class Window {

	public:
		Window(int width, int height, std::string name);

		void cleanup();

		Window(const Window&) = delete;
		Window& operator*(const Window&) = delete;

		void poll() { glfwPollEvents(); };

		bool shouldClose() { return glfwWindowShouldClose(m_window); }

		void createSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getGLFWwindow() { return m_window; }
		VkExtent2D getExtent() {
			int width, height;
			glfwGetFramebufferSize(m_window, &width, &height);
			return {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};
		}

	private:
		void init();

		int m_width;
		int m_height;

		std::string m_windowName;
		GLFWwindow* m_window;
	};
} // namespace oys