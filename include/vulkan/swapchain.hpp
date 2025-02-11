#pragma once

#include "vulkan/vulkan.h"
#include "vulkan_device.hpp"
#include <vector>

namespace oys {

	class Swapchain {
	public:
		Swapchain(VulkanDevice* device, VkSurfaceKHR surface, VkExtent2D windowExtent);
		
		void cleanup();

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;

		VkSwapchainKHR getSwapchain() const { return m_swapChain; }
		VkFormat getImageFormat() const { return m_imageFormat; }
		VkExtent2D getExtent() const { return m_extent; }
		const std::vector<VkImageView>& getImageViews() const { return m_swapChainImageViews; }
		size_t getImageCount() const { return m_swapChainImages.size(); }

	private:
		VulkanDevice* m_device;
		VkSwapchainKHR m_swapChain;
		VkFormat m_imageFormat;
		VkExtent2D m_extent;

		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D windowExtent);

		void createSwapChain(VkSurfaceKHR surface, VkExtent2D windowExtent);
		void createImageViews();
		void createRenderPass();
	};
}