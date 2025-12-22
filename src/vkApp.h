#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "mVKInstance.h"
#include "mVKPhysicalDevice.h"
#include "mVKLogicalDevice.h"
class VulkanApp
{
public:
	void run();

private:
    //初始化GLFW窗口
	void initWindow();
	void initVulkan();
	void initVulkanInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void mainLoop();
	void cleanUp();
	
private:
    GLFWwindow* window;//GLFW窗口指针
    static const uint32_t WIDTH = 800;
    static const uint32_t HEIGHT = 600;

private:
    mVKInstace vkInstance;//Vulkan实例封装类
	mVKPhysicalDevice vkPhysicalDevice;//物理设备封装类
	mVKLogicalDevice vkLogicalDevice;//逻辑设备封装类
};