#pragma once

#include "vulkan/vulkan.h"
#include <vector>

namespace oys {

	class VulkanInstance {
	public:
		VulkanInstance();

		void cleanup();

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;

		VkInstance getInstance() { return m_instance; }

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;

		// helper functions
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();

		void setupDebugMessenger();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		const std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };
	};
}