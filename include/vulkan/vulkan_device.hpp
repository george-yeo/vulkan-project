#pragma once

#include "vulkan/vulkan.h"
#include <optional>
#include <vector>
#include <window/window.hpp>

namespace oys {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanDevice {
	public:
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface);

		void cleanup();

		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(const VulkanDevice&) = delete;

		VkDevice getLogicalDevice() const { return m_device; }
		VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
		VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
		VkQueue getPresentQueue() const { return m_presentQueue; }
		QueueFamilyIndices getQueueFamilies() const { return m_queueFamilies; }

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;

	private:
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		QueueFamilyIndices m_queueFamilies;
		VkSurfaceKHR m_surface;

		void pickPhysicalDevice(VkInstance instance);
		void createLogicalDevice();

		bool isDeviceSuitable(VkPhysicalDevice device) const;
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
		bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
		
		int rateDeviceSuitability(VkPhysicalDevice device) const;

		const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};
}