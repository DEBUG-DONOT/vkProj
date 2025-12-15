cmake_minimum_required(VERSION 3.15)
project(HelloVulkan)
# 检查如果是 MSVC 编译器 (Visual Studio)
if (MSVC)
    # /utf-8 选项做了两件事：
    # 1. 告诉编译器源文件是 UTF-8 编码
    # 2. 告诉编译器生成的可执行文件内部字符串也用 UTF-8
    add_compile_options(/utf-8)
endif()
# 1. 设置 C++ 标准 (Pico 常用 C++17 或 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 自动寻找 Vulkan SDK (这是 CMake 最强大的地方，不用手动配路径)
find_package(Vulkan REQUIRED)

# 3. 设置 GLFW 路径 (这里假设你下载了 binaries 放在 libs 目录)
# 实际工作中通常用 vcpkg 或 FetchContent，但初学者手动指定最直观
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/libs/glfw")
include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/lib-vc2022) # 注意：根据你的 VS 版本选对应文件夹

# 4. 包含 volk (如果用 volk 的话)
#include_directories("${CMAKE_SOURCE_DIR}/volk")
# 把 volk.c 加入编译源文件
#set(VOLK_SOURCE "${CMAKE_SOURCE_DIR}/volk/volk.c")
add_subdirectory(volk)
# 5. 定义可执行文件
add_executable(HelloVulkan 
    src/main.cpp 
    src/vkApp.cpp
 "src/mVKInstance.h" "src/mVKInstance.cpp")

# 6. 链接库
target_link_libraries(HelloVulkan PRIVATE
    Vulkan::Vulkan
    glfw3.lib      # 链接 GLFW
    PRIVATE volk_headers
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

# 7. (可选) 拷贝 DLL 到输出目录 (防止运行报错找不到 dll)
add_custom_command(TARGET HelloVulkan POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_DIR}/lib-vc2022/glfw3.dll"
    $<TARGET_FILE_DIR:HelloVulkan>
)