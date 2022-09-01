#pragma once

#include "vulk_camera.h"

// lib
#include <vulkan/vulkan.h>

namespace vWind {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VulkCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}  // namespace vWind