#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>
//使用RAII，包含一个init函数，进行vkinstance的初始化
//对vk的instance进行封装

/*
* validation layer:验证层
* validation layer现在只有instance层面的验证层，所以我们也在instance中管理
* 
* surface是instance级别的概念，所以我们同样将surface放在这里进行处理
*/

class mVKInstace
{
public:
	void CreateVulkanInstance(VkInstanceCreateInfo _vkInstanceCreateInfo);
	void CreateVulkanInstance();
	void DestroyVulkanInstance();
	VkInstance GetVulkanInstance() { return this->mVKInstance; }

	void CreateVulkanSurface(GLFWwindow* window);
	void DestroyVulkanSurface();
	VkSurfaceKHR GetVulkanSurface() { return this->mVulkanSurface; }
private:
	void InitDefaultVKInstanceCreateInfo();
	void CheckVulkanExtensionSupport();

	bool CheckVulkanLayerSupport(std::vector<const char*> _layerNames, uint32_t _layerCount);//用来检查验证层支持情况
private:
	VkInstance mVKInstance;
	VkSurfaceKHR mVulkanSurface=VK_NULL_HANDLE;//抽象的surface，本质上是一个指针
	VkInstanceCreateInfo mDefaultVKInstanceCreateInfo{};
	VkApplicationInfo mDefaultVKApplicationInfo{};
	bool mbEnableValidationLayers = true;
};