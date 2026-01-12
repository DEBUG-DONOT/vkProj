#include "mVKInstance.h"
#include <iostream>
#include <cstring>
static const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"};

static const std::vector<const char*> deviceExtensions = {
            "VK_KHR_surface",
            "VK_KHR_win32_surface" };

void mVKInstace::CreateVulkanInstance(VkInstanceCreateInfo _vkInstanceCreateInfo)
{
    //几乎所有的vulkan的create函数都返回VkResult类型，并且都需要一个structure作为参数传递信息，接受一个指针作为alloctor，还有一个输出指针
    InitDefaultVKInstanceCreateInfo();
    _vkInstanceCreateInfo.pApplicationInfo = &this->mDefaultVKApplicationInfo;
    VkResult result = vkCreateInstance(&_vkInstanceCreateInfo, nullptr, &this->mVKInstance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
    }
    //CheckVulkanExtensionSupport();
}

void mVKInstace::CreateVulkanInstance()
{
    //使用默认的createinfo来创建vkinstance
    InitDefaultVKInstanceCreateInfo();
    VkResult result = vkCreateInstance(&this->mDefaultVKInstanceCreateInfo, nullptr, &this->mVKInstance);//mVKInstance是空指针
if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
    }   
   // CheckVulkanExtensionSupport();
}

void mVKInstace::DestroyVulkanInstance()
{
    //vkinstance要在window销毁前销毁
    vkDestroyInstance(this->mVKInstance, nullptr);
}

void mVKInstace::CreateVulkanSurface(GLFWwindow* window)
{
    //使用glfw可以规避平台相关的代码
    if (glfwCreateWindowSurface(this->mVKInstance, window, nullptr, &this->mVulkanSurface) != VK_SUCCESS) {
        std::cerr << "Failed to create window surface!" << std::endl;
    }
}

void mVKInstace::DestroyVulkanSurface()
{
    vkDestroySurfaceKHR(this->mVKInstance, this->mVulkanSurface, nullptr);
}

void mVKInstace::InitDefaultVKInstanceCreateInfo()
{
    //TODO：改为使用临时变量，不使用成员变量
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
    if(mbEnableValidationLayers)
    {
        if (!CheckVulkanLayerSupport(validationLayers, static_cast<uint32_t>(validationLayers.size()))) {
            std::cerr << "Validation layers requested, but not available!" << std::endl;
            mbEnableValidationLayers = false;
        }
        this->mDefaultVKInstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        this->mDefaultVKInstanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        this->mDefaultVKInstanceCreateInfo.enabledLayerCount = 0;
        this->mDefaultVKInstanceCreateInfo.ppEnabledLayerNames = nullptr;
    }
    //启用拓展  
    this->mDefaultVKInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    this->mDefaultVKInstanceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
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

bool mVKInstace::CheckVulkanLayerSupport(std::vector<const char *> _layerNames, uint32_t _layerCount)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    std::cout << "Available Vulkan layer:" << std::endl;
    for (const auto& _layer : availableLayers) {
        std::cout << "\t" << _layer.layerName << std::endl;
    }
// 2. 修正逻辑：外层循环遍历“请求的层”，内层遍历“可用的层”
    for (uint32_t i = 0; i < _layerCount; i++) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(_layerNames[i], layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false; // 只要有一个请求的层没找到，才返回 false
        }
    }
    return true;
}
