cmake_minimum_required(VERSION 3.12)

project(vAllk_framegraph)
set(CMAKE_CXX_STANDARD 20)

set(VK_SRC
    VkBuffer.cpp
    VMAImpl.cpp
    DescriptorPool.cpp
    VertexDataPool.cpp
)

add_library(vAllk_framegraph ${VK_SRC})

target_link_libraries(vAllk_framegraph PUBLIC vma vulkan)
