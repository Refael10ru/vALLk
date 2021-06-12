#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace vallk::vk {

constexpr VmaMemoryUsage bufferUsageToVmaMemory(VkBufferUsageFlags usage) noexcept {
    switch (usage) {
    default: return VMA_MEMORY_USAGE_CPU_TO_GPU;
    }
}

}