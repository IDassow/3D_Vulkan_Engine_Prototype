#include "vulk_model.h"

//std
#include <cassert>
#include <cstring>


namespace vWind {

	VulkModel::VulkModel(VulkDevice& device, const std::vector<Vertex>& vertices) 
		: vulkDevice{ device } {
		createVertexBuffers(vertices);
	}
	VulkModel::~VulkModel() {
		vkDestroyBuffer(vulkDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(vulkDevice.device(), vertexBufferMemory, nullptr);
	}


	void VulkModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize buffSize = sizeof(vertices[0]) * vertexCount;

		vulkDevice.createBuffer(
			buffSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory
		);

		void* data;
		vkMapMemory(vulkDevice.device(), vertexBufferMemory, 0, buffSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<uint32_t>(buffSize));
		vkUnmapMemory(vulkDevice.device(), vertexBufferMemory);
	}

	void VulkModel::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffs[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffs, offsets);

	}

	void VulkModel::draw(VkCommandBuffer commandBuffer) {
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	std::vector<VkVertexInputBindingDescription> VulkModel::Vertex::getBindingDesciptions() {
		//return{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}; 
		std::vector<VkVertexInputBindingDescription>bindingDesciptions(1);
		bindingDesciptions[0].binding;
		bindingDesciptions[0].stride = sizeof(Vertex);
		bindingDesciptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDesciptions;
	}
	std::vector<VkVertexInputAttributeDescription> VulkModel::Vertex::getAttributeDesciptions() {
		//return{
		//	{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)}
		//	{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}
		// };
		std::vector<VkVertexInputAttributeDescription>attribDesciptions(2);
		attribDesciptions[0].binding = 0;
		attribDesciptions[0].location = 0;
		attribDesciptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDesciptions[0].offset = offsetof(Vertex, position);

		attribDesciptions[1].binding = 0;
		attribDesciptions[1].location = 1;
		attribDesciptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDesciptions[1].offset = offsetof(Vertex,color);
		return attribDesciptions;
	}

}//namespace vWind