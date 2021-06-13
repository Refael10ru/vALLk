



#include "util/pool_set.hpp"
#include "../extern/vulkan/include/vulkan/vulkan.h"



namespace vallk::vk
{

template<size_t Cache>
class descriptor_sub_pool
{

public:
    descriptor_sub_pool(VkDevice device, uint32_t descriptorSetCount, VkDescriptorPoolSize poolSizes[],
                        VkDescriptorSetLayoutBinding layoutBinding[], uint32_t bindingCount = 1)
        : _device(device) size(descriptorSetCount)
    {


        VkDescriptorPoolCreateInfo poolInfo{};
        // descriptor pool for a specific layout
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.pPoolSizes = poolSizes;
        poolInfo.maxSets = descriptorSetCount;

        for (uint32_t ii = 0; ii < bindingCount; ii++)
        {
            poolSizes[ii].descriptorCount = layoutBinding[ii].descriptorCount;
            poolSizes[ii].type = layoutBinding[ii].descriptorType;
        }
        poolInfo.poolSizeCount = bindingCount;

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &_pool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    ~descriptor_sub_pool()
    {
        vkDestroyDescriptorPool(this->device, _pool, nullptr);
    }

    bool allocate(uint32_t count, VkDescriptorSet sets[], VkDescriptorSetLayout layoutSpace[])
    {
        if (count > _size)
            return false;
        
        for (uint32_t ii = 0; ii < count; ii++)
            layoutSpace[ii] = layout;
        VkDescriptorSetAllocateInfo descriptorAllocInfo{};
        descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorAllocInfo.descriptorPool = _pool;
        descriptorAllocInfo.descriptorSetCount = count;
        descriptorAllocInfo.pSetLayouts = layoutSpace;
        if (vkAllocateDescriptorSets(this->device, &descriptorAllocInfo, sets) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        _size -= count;
        return true;
    }
    -
private:
    VkDevice _device;
    VkDescriptorSetLayout _layout;
    VkDescriptorPool _pool;
    uint32_t _size;
    uint32_t _cachedCount;
    std::array<VkDescriptorSet,Cache> _cache;
};

class DescriptorPool : public util::pool_set<VkDescriptorPool>
{
public:


private:
    VkDevice device;

}




}