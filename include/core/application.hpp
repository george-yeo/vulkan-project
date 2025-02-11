#include <memory>
#include <vulkan/pipeline.hpp>
#include <vulkan/swapchain.hpp>
#include <vulkan/vulkan_device.hpp>
#include <vulkan/vulkan_instance.hpp>
#include <window/window.hpp>

namespace oys {

	class Application {

	public:
		Application();
		~Application();

		void init();
		void update();
		void cleanup();

		bool isActive() { return !m_window.shouldClose(); }

	private:
		Window m_window;
		VulkanInstance m_vulkanInstance;
		VulkanDevice m_vulkanDevice;
		VkSurfaceKHR m_surface;
		Swapchain m_swapchain;
		Pipeline m_pipeline;
	};
}