#include "vulk_renderer.h"


//std
#include <stdexcept>
#include <array>
#include <cassert>

namespace vWind {
    VulkRenderer::VulkRenderer( VulkWindow &window, VulkDevice &device) 
        : vulkWindow{ window }, vulkDevice{ device } {
        recreateSwapChain();
        createCommandBuffers();
    }

    VulkRenderer::~VulkRenderer() { freeCommandBuffers(); }

    void VulkRenderer::recreateSwapChain() {
        auto extent = vulkWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = vulkWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(vulkDevice.device());

        if (vulkSwapChain == nullptr) {
            vulkSwapChain = std::make_unique<VulkSwapChain>(vulkDevice, extent);
        }
        else {
            std::shared_ptr<VulkSwapChain> oldSwapChain = std::move(vulkSwapChain);
            vulkSwapChain = std::make_unique<VulkSwapChain>(vulkDevice, extent, oldSwapChain);
            if (!oldSwapChain->compareSwapFormats(*vulkSwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }
        }
        //pending actions
    }

    void VulkRenderer::createCommandBuffers() {
        commandBuffers.resize(vulkSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(
            vulkDevice.device(),
            &allocInfo,
            commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!...");
        }
    }

    void VulkRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
            vulkDevice.device(),
            vulkDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer VulkRenderer::beginFrame() {
        assert(!isFrameStarted && "CANT call 'beginFrame()' while already in progress");
        auto result = vulkSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCB();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin command buffer!");
        }
        return commandBuffer;
    }

    void VulkRenderer::endFrame() {
        assert(isFrameStarted && "CANT call 'endFrame()' while frame not in progress");
        auto commandBuffer = getCurrentCB();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = vulkSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkWindow.wasWindowResized()) {
            vulkWindow.resetWindowResizeFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % VulkSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void VulkRenderer::beginSCRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "CANT call 'beginSCRenderPass()' while frame not in progress");
        assert(commandBuffer == getCurrentCB() 
            && "Cannot begin renderpass on command buffer from a different frame");
        
        VkRenderPassBeginInfo renderPassInfo{};

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkSwapChain->getRenderPass();
        renderPassInfo.framebuffer = vulkSwapChain->getFrameBuffer(currentImageIndex);
        //defines where shaders load and store
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vulkSwapChain->getSwapChainExtent();
        //specifying intial framebuffer attachments cleared to 
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.015f, 0.015f, 0.015f, 0.1f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        //record to commandbuffer to begin renderpass
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(vulkSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, vulkSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }//swapchain renderpass

    void VulkRenderer::endSCRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "CANT call 'endSCRenderPass()' while frame not in progress");
        assert(commandBuffer == getCurrentCB()
            && "Cannot end renderpass on command buffer from a different frame");
        vkCmdEndRenderPass(commandBuffer);

    }//swapchain renderpass


}//endof namespace vWind