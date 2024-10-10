#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

namespace oys {

	class OysWindow {

	public:
		OysWindow(int w, int h, std::string name);
		~OysWindow();

		OysWindow(const OysWindow&) = delete;
		OysWindow& operator*(const OysWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
} // namespace oys