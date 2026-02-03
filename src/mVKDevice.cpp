//
// Created by 87784 on 2025/12/23.
//

#include "mVKDevice.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

std::vector<const char*> enabledExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };


void mVKDevice::InitDevice(VkInstance instance,VkSurfaceKHR surface)
{
    //作为对外的接口，负责physical device和logical device的整体初始化流程
    PickPhysicalDevice(instance);
    auto indices= FindQueueFamilies(this->physicalDevice,surface);
    CreateLogicalDevice(indices, physicalDevice);
    GetGraphicsQueues( indices,this->logicalDevice);
    GetPresentQueues( indices,this->logicalDevice);
}

void mVKDevice::DestroyDevice()
{
    vkDestroyDevice(this->logicalDevice, nullptr);
}

void mVKDevice::PickPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr<<"Failed to find GPUs with Vulkan support!"<<std::endl;
        return;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    //评估物理设备
    std::multimap<uint32_t, VkPhysicalDevice> rankedDevices;
    for(auto & device:devices)
    {
        rankedDevices.insert({RatePhysicalDevice(device), device});
    }
    //选择评分最高的物理设备
    if(rankedDevices.rbegin()->first>0)
    {
        this->physicalDevice=rankedDevices.rbegin()->second;
        std::cout<<"Physical device: "<<this->physicalDevice<<std::endl;
        CheckPhysicalDeviceExtensionSupport(this->physicalDevice);
    }
    else
    {
        std::cerr<<"Failed to find a suitable GPU!"<<std::endl;
    }
}

/**
 * 检测物理设备支持哪些拓展
 * @param physicalDevice 物理设备handle
 */
void mVKDevice::CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
    // for (auto proposedExtension : extensions)
    // {
    //     std::cout << proposedExtension.extensionName << std::endl;
    // }
    //检测我们需要的extension是否都支持，教程中的设计是放在判断physical device是否合规的判断中，但是由于是在本地跑，可以放弃那一段逻辑
    std::set<std::string> enabledExtensionsString(enabledExtensions.begin(), enabledExtensions.end());
    for (auto& extension : extensions)
    {
        enabledExtensionsString.erase(extension.extensionName);
    }
    if (!enabledExtensionsString.empty())
    {
        std::cerr<<"The following extensions were not found:"<<std::endl;
        for (auto& extension : enabledExtensionsString)
        {
            std::cout<<extension<<std::endl;
        }
        std::cerr<<"Please check the following extensions:"<<std::endl;
    }
    else
    {
        std::cout<<"==================Extension all found:===================="<<std::endl;
        for (auto extension : enabledExtensions)
        {
            std::cout<<std::string (extension)<<std::endl;
        }
        std::cout<<"====================End=================="<<std::endl;
    }
}


void mVKDevice::CreateLogicalDevice(MyQueueFamilyIndices index, VkPhysicalDevice physicalDevice)
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilyIndices{index.graphicsFamily.value(),index.presentFamily.value()};
    for (auto uniqueQueueFamilyIndex : uniqueQueueFamilyIndices)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = uniqueQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = new float[1]{1.0f};
        queueCreateInfos.push_back(queueCreateInfo);
    }
    // queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    // queueCreateInfo.queueFamilyIndex = index.graphicsFamily.value();
    // queueCreateInfo.queueCount = 1;
    // queueCreateInfo.pQueuePriorities = new float[1]{1.0f};

    VkPhysicalDeviceFeatures deviceFeatures{}; // 可以根据需要启用特性
    std::vector<const char*> deviceEnableExtensions{};

    VkDeviceCreateInfo deviceCreateInfoLocal{};
    deviceCreateInfoLocal.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfoLocal.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfoLocal.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfoLocal.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfoLocal.enabledExtensionCount=static_cast<uint32_t>(enabledExtensions.size());
    deviceCreateInfoLocal.ppEnabledExtensionNames = enabledExtensions.data();

    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfoLocal, nullptr, &this->logicalDevice);
    if (result != VK_SUCCESS)
    {
        std::cerr << "Failed to create logical device!" << std::endl;
    }
    else
    {
        std::cout<<"Logical device: "<<this->logicalDevice<<std::endl;
    }
}

void mVKDevice::GetGraphicsQueues(MyQueueFamilyIndices index,VkDevice device)
{
    vkGetDeviceQueue(device, index.graphicsFamily.value(), 0, &this->graphicsQueue);
}

void mVKDevice::GetPresentQueues(MyQueueFamilyIndices index, VkDevice device)
{
    vkGetDeviceQueue(device, index.presentFamily.value(), 0, &this->presentQueue);
}


MyQueueFamilyIndices mVKDevice::FindQueueFamilies(VkPhysicalDevice _device,VkSurfaceKHR surface)
{
    MyQueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        //check graphics support
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        //check present support
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(_device,i,/*surface*/surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }
    }
    return indices;
}

uint32_t mVKDevice::RatePhysicalDevice(VkPhysicalDevice _device)
{
    //对物理设备进行评分
    //实际上是对物理设备的features和properties进行查询
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceProperties(_device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);
    uint32_t score = 0;
    if(_device!=VK_NULL_HANDLE)
    {
        score+=1;
    }
    //如果是独显
    if(deviceProperties.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score+=1000;
    }
    //检查VR的multiviewport支持
    if(deviceFeatures.multiViewport)
    {
        score+=500;
    }
    std::cout<<"Physical Device Name: "<<deviceProperties.deviceName<<" Physical Device Score: "<<score<<std::endl;
    return score;
}




