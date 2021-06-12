#pragma once

#include <utility>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vallk::vk {

// TODO: can be an object, just need an interface
//extern void createSparse(VkBindSparseInfo, VkQueue);

class Buffer {
    Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage) noexcept;
    Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage, VkSparseBufferMemoryBindInfo sparseInfo, VkQueue sparseQueue) noexcept;

    Buffer() noexcept = default;
    Buffer(Buffer&& oth) noexcept { *this = std::move(oth); }
    ~Buffer();

    Buffer& operator=(Buffer&&) noexcept;

private:
    VmaAllocator _allocator = nullptr;
    VmaAllocation _alloc;
    VkBuffer _handle;
};

}