
#include <utility>


namespace vk
{


class DescriptorPool
{
    DescriptorPool();
    
    DescriptorPool(DescriptorPool&) = delete;
    DescriptorPool(DescriptorPool&& other) { *this = std::move(other);  other = {}; }

    
};


}