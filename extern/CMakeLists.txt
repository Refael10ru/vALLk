cmake_minimum_required(VERSION 3.12)

project(vAllk_deps)

# 1. entt
add_subdirectory(entt)

# 2. glfw, skip installs
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "" FORCE)
set(GLFW_INSTALL OFF CACHE INTERNAL "" FORCE)

add_subdirectory(glfw)

# 3. glm
add_subdirectory(glm)

# 4. imgui
set(IMGUI_SRC
    ${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
)
add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/imgui)

# 5. vma
add_library(vma INTERFACE)
target_include_directories(vma INTERFACE ${PROJECT_SOURCE_DIR}/vma/include)

# 6. find vulkan
find_library(VULKAN_LIBRARY NAMES vulkan-1 HINTS "$ENV{VULKAN_SDK}/Lib" "$ENV{VULKAN_SDK}/lib")
if (NOT ${VULKAN_LIBRARY} STREQUAL "VULKAN_LIBRARY-NOTFOUND")
    if (WIN32)
        set(VULKAN_INCLUDE "$ENV{VULKAN_SDK}/Include")

        add_library(vulkan UNKNOWN IMPORTED)
        set_target_properties(vulkan PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${VULKAN_INCLUDE})
        set_target_properties(vulkan PROPERTIES IMPORTED_LOCATION ${VULKAN_LIBRARY})
    else()
        set(VULKAN_INCLUDE "$ENV{VULKAN_SDK}/include")
        add_library(vulkan INTERFACE)
        target_include_directories(vulkan INTERFACE ${VULKAN_INCLUDE})
    endif()
else()
    message(FATAL_ERROR "-- Could not find vulkan SDK, make sure VULKAN_SDK env var is populated")
endif()