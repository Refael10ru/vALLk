cmake_minimum_required(VERSION 3.12)

project(vAllk_framegraph)
set(CMAKE_CXX_STANDARD 17)

set(VK_SRC
    vk_buffer.cpp
    vma_impl.cpp
)

add_library(vAllk_framegraph ${VK_SRC})

target_link_libraries(vAllk_framegraph PUBLIC vma vulkan)