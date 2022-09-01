#pragma once

#include "vulk_device.h"
#include "vulk_buffer.h"

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

//std
#include <memory>
#include <vector>

namespace vWind {
	class VulkModel {
	public:

		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};
			static std::vector<VkVertexInputBindingDescription> getBindingDesciptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDesciptions();

			bool operator==(const Vertex& other) const {
				return position ==	other.position 
					&& color	==	other.color 
					&& normal	==	other.normal 
					&& uv		==	other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		VulkModel(VulkDevice &device, const VulkModel::Builder &builder);
		~VulkModel();

		VulkModel(const VulkWindow&) = delete;
		VulkModel& operator=(const VulkWindow&) = delete;

		static std::unique_ptr<VulkModel> createModelFromFile(
			VulkDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffer(const std::vector<uint32_t>& indices);

		VulkDevice& vulkDevice;

		std::unique_ptr<VulkBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<VulkBuffer> indexBuffer;
		uint32_t indexCount;
		
	};
}