#include "mVKInstance.h"
#include <iostream>
#include <vector>
void mVKInstace::CreateVulkanInstance(VkInstanceCreateInfo _vkInstanceCreateInfo)
{
    //几乎所有的vulkan的create函数都返回VkResult类型，并且都需要一个structure作为参数传递信息，接受一个指针作为alloctor，还有一个输出指针
    InitDefaultVKInstanceCreateInfo();
    _vkInstanceCreateInfo.pApplicationInfo = &this->mDefaultVKApplicationInfo;
    VkResult result = vkCreateInstance(&_vkInstanceCreateInfo, nullptr, &this->mVKInstance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
    }
}

void mVKInstace::CreateVulkanInstance()
{
    //使用默认的createinfo来创建vkinstance
    InitDefaultVKInstanceCreateInfo();
    VkResult result = vkCreateInstance(&this->mDefaultVKInstanceCreateInfo, nullptr, &this->mVKInstance);//mVKInstance是空指针
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
    }
    CheckVulkanExtensionSupport();
}

void mVKInstace::DestroyVulkanInstance()
{
    //vkinstance要在window销毁前销毁
    vkDestroyInstance(this->mVKInstance, nullptr);
}

void mVKInstace::InitDefaultVKInstanceCreateInfo()
{
    //初始化默认的vkinstancecreateinfo结构体
    //app info
    this->mDefaultVKApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    this->mDefaultVKApplicationInfo.pNext = nullptr;
    this->mDefaultVKApplicationInfo.pApplicationName = "VulkanApp";
    this->mDefaultVKApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    this->mDefaultVKApplicationInfo.pEngineName = "No Engine";
    this->mDefaultVKApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    this->mDefaultVKApplicationInfo.apiVersion = VK_API_VERSION_1_1;
    //instance create info
    this->mDefaultVKInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    this->mDefaultVKInstanceCreateInfo.pNext = nullptr;
    this->mDefaultVKInstanceCreateInfo.flags = 0;
    this->mDefaultVKInstanceCreateInfo.pApplicationInfo = &this->mDefaultVKApplicationInfo;
    //验证层相关
    this->mDefaultVKInstanceCreateInfo.enabledLayerCount = 0;
    this->mDefaultVKInstanceCreateInfo.ppEnabledLayerNames = nullptr;
    //启用拓展  
    this->mDefaultVKInstanceCreateInfo.enabledExtensionCount = 0;
    this->mDefaultVKInstanceCreateInfo.ppEnabledExtensionNames = nullptr;//指向扩展名字符串数组的指针
}

void mVKInstace::CheckVulkanExtensionSupport()
{
    //检查vulkan扩展支持情况
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    std::cout << "Available Vulkan extensions:" << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }

}
