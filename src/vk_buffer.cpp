#include "vk_buffer.hpp"
#include "vma_util.hpp"

namespace vallk::vk {

Buffer::Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage) noexcept :
    _allocator{ allocator }
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    
    VmaAllocationCreateInfo vmaInfo{};
    vmaInfo.usage = bufferUsageToVmaMemory(usage);

    vmaCreateBuffer(_allocator, &bufferInfo, &vmaInfo, &_handle, &_alloc, nullptr);
}

Buffer::Buffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage, VkSparseBufferMemoryBindInfo sparseInfo, VkQueue sparseQueue) noexcept :
    _allocator{ allocator }
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.flags = VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;

    VmaAllocationCreateInfo vmaInfo{};
    vmaInfo.usage = bufferUsageToVmaMemory(usage);

    vmaCreateBuffer(_allocator, &bufferInfo, &vmaInfo, &_handle, &_alloc, nullptr);

    VkBindSparseInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
    submitInfo.pBufferBinds = &sparseInfo; // if issuing a dealyed command, make a copy on sparse end
    submitInfo.bufferBindCount = 1;

    // TODO: createSparse(submitInfo, sparseQueue);
}

Buffer::~Buffer() {
    if (_allocator != nullptr) {
        vmaDestroyBuffer(_allocator, _handle, _alloc);
    }   
}

Buffer& Buffer::operator=(Buffer&& oth) noexcept {
    if (_allocator != nullptr) {
        vmaDestroyBuffer(_allocator, _handle, _alloc);
    }

    _allocator = oth._allocator;
    _alloc = oth._alloc;
    _handle = oth._handle;

    oth._allocator = nullptr;
    return *this;
}

}