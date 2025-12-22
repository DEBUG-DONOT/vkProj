#include "mVKPhysicalDevice.h"
#include <iostream>
#include <vector>
#include <map>

void mVKPhysicalDevice::PickPhysicalDevice(VkInstance instance)
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
        mPhysicalDevice=rankedDevices.rbegin()->second;
    }
    else
    {
        std::cerr<<"Failed to find a suitable GPU!"<<std::endl;
    }
}

uint32_t mVKPhysicalDevice::RatePhysicalDevice(VkPhysicalDevice _device)
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

MyQueueFamilyIndices mVKPhysicalDevice::FindQueueFamilies(VkPhysicalDevice _device)
{
    //对于传递进来的device，检查它提供的queue family
    MyQueueFamilyIndices indices;    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }
    }
    return indices;
}
