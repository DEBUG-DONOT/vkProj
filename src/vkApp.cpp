#include "vkApp.h"

#include <iostream>
void VulkanApp::run()
{
	initWindow();
	initVulkan();
	initVulkanInstance();
	initDevice();
	
	mainLoop();
	cleanUp();
}

void VulkanApp::initWindow()
{
	//初始化glfw
	glfwInit();
	//window hint
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfw create window
	window = glfwCreateWindow(VulkanApp::WIDTH, VulkanApp::HEIGHT, "Vulkan Window", nullptr, nullptr);
	std::cout << "VulkanApp::Init Window function" << std::endl;
	uint32_t extensionCount = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
	std::cout << "Required GLFW extensions:" << std::endl;
	for (uint32_t i = 0; i < extensionCount; i++) {
		std::cout << "\t" << extensions[i] << std::endl;
	}
}

void VulkanApp::initVulkan()
{
	std::cout << "VulkanApp::Init vulkan function" << std::endl;
}

void VulkanApp::initVulkanInstance()
{
	std::cout << "VulkanApp::Init vulkan instance function" << std::endl;
	this->vkInstance.CreateVulkanInstance();
}

void VulkanApp::initDevice()
{
	this->vkDevice.InitDevice(this->vkInstance.GetVulkanInstance());
}

void VulkanApp::mainLoop()
{
	//主循环
	//while not close
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void VulkanApp::cleanUp()
{
	//清理资源
	this->vkDevice.DestroyDevice();
	this->vkInstance.DestroyVulkanInstance();

	glfwDestroyWindow(window);
	glfwTerminate();
}
