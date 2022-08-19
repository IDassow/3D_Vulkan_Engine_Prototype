#pragma once

#include "vulk_window.h"
#include "vulk_pipeline.h"
#include "vulk_device.h"
#include "vulk_swap_chain.h"
#include "vulk_game_object.h"



//std
#include <memory>
#include <vector>
namespace vWind {

	class VulkRenderer {
	public:

		VulkRenderer(VulkWindow &window, VulkDevice &device);
		~VulkRenderer();

		VulkRenderer(const VulkRenderer&) = delete;
		VulkRenderer& operator=(const VulkRenderer&) = delete;

		bool isFrameInProgress() const { return isFrameStarted; }
		VkRenderPass getSCRenderPass() const { return vulkSwapChain->getRenderPass(); }
		VkCommandBuffer getCurrentCB() const {
			assert(isFrameStarted && "Cannot get command Buffer when frame is not in progress");
			return commandBuffers[currentFrameIndex];
		}
		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame is not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSCRenderPass(VkCommandBuffer commandBuffer);//swapchain renderpass
		void endSCRenderPass(VkCommandBuffer commandBuffer);//swapchain renderpass


	private:
		void createCommandBuffers();
		void recreateSwapChain();
		void freeCommandBuffers();

		VulkWindow& vulkWindow;
		VulkDevice& vulkDevice;
		std::unique_ptr<VulkSwapChain> vulkSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted{false};

	};
}//endof namespace vWind