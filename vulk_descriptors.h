#pragma once

#include "vulk_device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace vWind{

    class VulkDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(VulkDevice& vulkDevice) : vulkDevice{ vulkDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<VulkDescriptorSetLayout> build() const;

        private:
            VulkDevice& vulkDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        VulkDescriptorSetLayout(
            VulkDevice& vulkDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~VulkDescriptorSetLayout();
        VulkDescriptorSetLayout(const VulkDescriptorSetLayout&) = delete;
        VulkDescriptorSetLayout& operator=(const VulkDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        VulkDevice& vulkDevice;
        VkDescriptorSetLayout descriptorSetLayout{};
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VulkDescriptorWriter;
    };

    class VulkDescriptorPool {
    public:
        class Builder {
        public:
            Builder(VulkDevice& vulkDevice) : vulkDevice{ vulkDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<VulkDescriptorPool> build() const;

        private:
            VulkDevice& vulkDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VulkDescriptorPool(
            VulkDevice& vulkDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VulkDescriptorPool();
        VulkDescriptorPool(const VulkDescriptorPool&) = delete;
        VulkDescriptorPool& operator=(const VulkDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        VulkDevice& vulkDevice;
        VkDescriptorPool descriptorPool;

        friend class VulkDescriptorWriter;
    };

    class VulkDescriptorWriter {
    public:
        VulkDescriptorWriter(VulkDescriptorSetLayout& setLayout, VulkDescriptorPool& pool);

        VulkDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VulkDescriptorSetLayout& setLayout;
        VulkDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace vWind