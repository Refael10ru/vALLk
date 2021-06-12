

#include <cstdint>
#include <vector>
#include <type_traits>

namespace vk::util 
{



class StackAllocator
{
    std::vector<unsigned char> data;
public:
    uint32_t offset;
    StackAllocator(uint32_t sizeBytes) : data(sizeBytes) , offset(0) {};

    template<class Type, typename std::enable_if<std::is_default_constructible<Type>::value>::type>
    Type* Allocate(uint32_t count)
    {   
        auto location = (offset + _Alignof(Type)) % _Alignof(Type);
        auto size = count * sizeof(Type);
        if(location + size > data.size()) return nullptr;
        this->offset = location + size;
        return &data[location];
        
    }

};


}