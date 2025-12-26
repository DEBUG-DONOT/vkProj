#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "mVKInstance.h"
#include "mVKDevice.h"
class VulkanApp
{
public:
	void run();

private:
    //初始化GLFW窗口
	void initWindow();
	void initVulkan();
	void initVulkanInstance();
	//void pickPhysicalDevice();
	//void createLogicalDevice();
	void initDevice();
	void mainLoop();
	void cleanUp();
	
private:
    GLFWwindow* window;//GLFW窗口指针
    static const uint32_t WIDTH = 800;
    static const uint32_t HEIGHT = 600;

private:
    mVKInstace vkInstance;//Vulkan实例封装类
	mVKDevice vkDevice;//physical device 和logical device的封装类

};