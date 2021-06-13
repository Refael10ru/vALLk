#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "util/PoolSet.hpp"
#include "util/ReferenceCounter.hpp"

namespace vallk::vk {

using VertexDataPool = util::PoolSet<>;

}