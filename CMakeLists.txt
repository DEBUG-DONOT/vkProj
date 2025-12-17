cmake_minimum_required(VERSION 3.15)
project(HelloVulkan)

# --- 1. 基础设置 ---
# Windows 编码修正
if (MSVC)
    add_compile_options(/utf-8)
endif()

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. 寻找依赖 ---
# 2.1 Vulkan
find_package(Vulkan REQUIRED)

# 2.2 GLFW (设置路径变量，方便后面引用)
set(GLFW_PATH "${CMAKE_SOURCE_DIR}/libs/glfw")
# 指向 GLFW 的 include 目录
set(GLFW_INCLUDE_DIR "${GLFW_PATH}/include")
# 指向 GLFW 的 .lib 文件 (注意：这里选 glfw3dll.lib 以配合 DLL 模式)
set(GLFW_LIB "${GLFW_PATH}/lib-vc2022/glfw3dll.lib")

# 2.3 Volk (最简单的集成方式：直接把 volk.c 当源代码编译)
# 这样你不需要 volk 文件夹里有 CMakeLists.txt
set(VOLK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/volk")
set(VOLK_SOURCE "${CMAKE_SOURCE_DIR}/volk/volk.c")

# --- 3. 定义可执行文件 ---
add_executable(HelloVulkan 
    src/main.cpp 
    src/vkApp.cpp
    src/mVKInstance.cpp
    # 把 volk.c 加进来一起编译
    ${VOLK_SOURCE} 
)

# --- 4. 头文件路径 ---
target_include_directories(HelloVulkan PRIVATE
    "${CMAKE_SOURCE_DIR}/src"  # 修复了硬编码路径
    ${GLFW_INCLUDE_DIR}        # GLFW 头文件
    ${VOLK_INCLUDE_DIR}        # Volk 头文件
)

# --- 5. 链接库 ---
target_link_libraries(HelloVulkan PRIVATE
    Vulkan::Vulkan
    ${GLFW_LIB}  # 直接链接具体的 lib 文件，比 link_directories 更安全
)

# --- 6. 定义宏 (可选) ---
# 告诉 GLFW 我们在使用 DLL 版本 (虽然对于 client 没强制要求，但写上更好)
target_compile_definitions(HelloVulkan PRIVATE GLFW_DLL)

# --- 7. 后处理：拷贝 DLL ---
add_custom_command(TARGET HelloVulkan POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_PATH}/lib-vc2022/glfw3.dll"
    $<TARGET_FILE_DIR:HelloVulkan>
    COMMENT "Copying GLFW DLL to output directory..."
)