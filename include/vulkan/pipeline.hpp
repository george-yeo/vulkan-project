#pragma once

#include "vulkan/vulkan.h"
#include "vulkan_device.hpp"
#include <vector>

namespace oys {

	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline {
	public:
		Pipeline(VulkanDevice& device, const PipelineConfigInfo& configInfo);

		void cleanup();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		VulkanDevice& m_device;
		VkPipeline m_graphicsPipeline;
		VkShaderModule m_vertShaderModule;
		VkShaderModule m_fragShaderModule;
		VkPipelineLayout m_pipelineLayout;

		void createGraphicsPipeline(const PipelineConfigInfo& configInfo);
		void createPipelineLayout();

		std::vector<char> readFile(const std::string& filename);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	};
}