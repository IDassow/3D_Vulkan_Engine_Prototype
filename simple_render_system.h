#pragma once

#include "vulk_camera.h"
#include "vulk_device.h"
#include "vulk_game_object.h"
#include "vulk_pipeline.h"
#include "vulk_frame_info.h"

// std
#include <memory>
#include <vector>

namespace vWind {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(VulkDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSL );
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		
		void renderGameObjects(FrameInfo& frameInfo, std::vector<VulkGameObj>& gameObjects);


	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSL);
		void createPipeline(VkRenderPass renderPass);

		VulkDevice& vulkDevice;

		std::unique_ptr<VulkPipeline> vulkPipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve