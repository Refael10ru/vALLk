


#include "../extern/Vulkan/include/vulkan/vulkan.h"



#include <vector>


namespace vk
{



template<class Type> 
class SubPool
{   
    std::vector<Type>;
};


template<template<class> class MySubPool>
class Pool
{
    static_assert(sizeof(MySubPool<char>) == sizeof(MySubPool<int>));
    template<typename Type>
    using pool_type_id = utils::type_id<Type,MySubPool>;

    VkDevice device;
    std::vector<MySubPool<void>> subPools;

    Pool() = delete;
    Pool(Pool&) = delete;
    Pool(Pool&& other) { *this = std::move(other); }

    Pool(VkDevice device) { this->device = device; }
    void operator=(Pool&& other);

    template<class Type>
    Type* getSubPool()
    {   
        util::id_type id = pool_type_id<Type>::value();
        if(id >= this->subPools.size())
            this->subPools.resize(id + 1);
        return reinterpret_cast<MySubPool<Type>&>(subPools[id]);
        
    }
    
    
};

template<class MySubPool>
class Pool
{
    static_assert(sizeof(MySubPool<char>) == sizeof(MySubPool<int>));
    template<typename Type>
    using pool_type_id = utils::type_id<Type,MySubPool>;

    VkDevice device;
    std::vector<MySubPool<void>> subPools;

    Pool() = delete;
    Pool(Pool&) = delete;
    Pool(Pool&& other) { *this = std::move(other); }

    Pool(VkDevice device) { this->device = device; }
    void operator=(Pool&& other);

    template<class Type>
    MySubPool<Type>& getSubPool()
    {   
        util::id_type id = pool_type_id<Type>::value();
        if(id >= this->subPools.size())
            this->subPools.resize(id + 1);
        return reinterpret_cast<MySubPool<Type>&>(subPools[id]);
        
    }
    
    
};

using VertexDataPool = Pool<SubPool>;


}