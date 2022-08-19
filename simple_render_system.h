#pragma once

#include "vulk_device.h"
#include "vulk_game_object.h"
#include "vulk_pipeline.h"

// std
#include <memory>
#include <vector>

namespace vWind {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(VulkDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VulkGameObj>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VulkDevice& vulkDevice;

		std::unique_ptr<VulkPipeline> vulkPipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve