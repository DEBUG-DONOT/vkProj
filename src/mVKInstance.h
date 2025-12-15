#pragma once
#include <vulkan/vulkan.h>

//使用RAII，包含一个init函数，进行vkinstance的初始化


class mVKInstace
{
public:
	void CreateVulkanInstance(VkInstanceCreateInfo _vkInstanceCreateInfo);
	void CreateVulkanInstance();
	void DestroyVulkanInstance();
private:
	VkInstance* mVKInstance=nullptr;
};