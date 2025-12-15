#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class VulkanApp
{
public:
	void run();

private:
    //初始化GLFW窗口
	void initWindow();
	void initVulkan();
	
	
	void mainLoop();
	void cleanUp();
	
private:
    GLFWwindow* window;//GLFW窗口指针
    static const uint32_t WIDTH = 800;
    static const uint32_t HEIGHT = 600;
};