#pragma once

#include "oys_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace oys {
	class OysModel {
	public:

		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		OysModel(OysDevice &device, const std::vector<Vertex>& vertices);
		~OysModel();

		OysModel(const OysModel&) = delete;
		OysModel& operator=(const OysModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		OysDevice& oysDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}