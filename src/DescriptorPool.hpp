



#include "util/pool_set.hpp"
#include "../extern/vulkan/include/vulkan/vulkan.h"



namespace vallk::vk
{


    class DescriptorPool : public util::pool_set<void>
    {

        VkDevice device;
        VkDescriptorPool pool;

    }




}