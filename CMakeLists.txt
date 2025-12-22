cmake_minimum_required(VERSION 3.15)

# 1. 编码与环境变量处理 (针对不同编译器前端)
if (MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # 针对 MSVC 环境下的 Clang (clang-cl) 强制 UTF-8 输出
    set(ENV{CL} "$ENV{CL} /utf-8")
endif()

project(HelloVulkan)

# 2. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. 编译器特定选项处理
if (MSVC)
    add_compile_options(/utf-8)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Clang 和 GCC 的编码设置
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
    # 建议：利用 Clang 强大的警告功能，帮助发现 Vulkan 调用中的潜在逻辑问题
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

# 4. 自动寻找 Vulkan SDK
find_package(Vulkan REQUIRED)

# 5. GLFW 配置 (重点：库路径适配)
set(GLFW_PATH "${CMAKE_SOURCE_DIR}/libs/glfw")
set(GLFW_INCLUDE_DIR "${GLFW_PATH}/include")

# 判断编译器类型以选择正确的 GLFW 库版本
if (MSVC OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND WIN32))
    # Windows 上的 Clang (通常是 clang-cl) 兼容 MSVC 的 .lib 文件
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2022/glfw3dll.lib")
    set(GLFW_DLL "${GLFW_PATH}/lib-vc2022/glfw3.dll")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # 如果你切换到 MinGW (GCC)，则需要使用 .a 文件
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64/libglfw3dll.a")
    set(GLFW_DLL "${GLFW_PATH}/lib-mingw-w64/glfw3.dll")
endif()

# 6. 定义可执行文件
add_executable(HelloVulkan 
    src/main.cpp 
    src/vkApp.cpp
    src/mVKInstance.cpp
    src/mVKPhysicalDevice.cpp
    src/utils.h
)

# 7. 头文件路径
target_include_directories(HelloVulkan PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    ${GLFW_INCLUDE_DIR}
    ${Vulkan_INCLUDE_DIRS}
)

# 8. 链接库
target_link_libraries(HelloVulkan PRIVATE
    Vulkan::Vulkan
    ${GLFW_LIB}
)

# 9. 拷贝 DLL 到输出目录 (保持运行环境完整)
add_custom_command(TARGET HelloVulkan POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_DLL}"
    $<TARGET_FILE_DIR:HelloVulkan>
    COMMENT "Copying GLFW DLL to execution directory..."
)