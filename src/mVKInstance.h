#pragma once
#include <vulkan/vulkan.h>

//使用RAII，包含一个init函数，进行vkinstance的初始化
//对vk的instance进行封装

class mVKInstace
{
public:
	void CreateVulkanInstance(VkInstanceCreateInfo _vkInstanceCreateInfo);
	void CreateVulkanInstance();
	void DestroyVulkanInstance();
private:
	void InitDefaultVKInstanceCreateInfo();
	void CheckVulkanExtensionSupport();
private:
	VkInstance mVKInstance;
	VkInstanceCreateInfo mDefaultVKInstanceCreateInfo{};
	VkApplicationInfo mDefaultVKApplicationInfo{};
};