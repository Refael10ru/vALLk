#pragma once

#include <vulkan/vulkan.h>

#include "util/PoolSet.hpp"

namespace vallk::vk {

class DescriptorPool : public util::PoolSet<> {

    VkDevice device;
    VkDescriptorPool pool;

};

}