#pragma once

#include "vulk_device.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vWind {
	class VulkModel {
	public:

		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;
			static std::vector<VkVertexInputBindingDescription> getBindingDesciptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDesciptions();
		};

		VulkModel(VulkDevice &device, const std::vector<Vertex>& vertices);
		~VulkModel();

		VulkModel(const VulkWindow&) = delete;
		VulkModel& operator=(const VulkWindow&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);


		VulkDevice& vulkDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		
	};
}