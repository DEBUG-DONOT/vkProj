#include "mVKLogicalDevice.h"
#include <iostream>
void mVKLogicalDevice::CreateLogicalDevice(MyQueueFamilyIndices index, VkPhysicalDevice physicalDevice)
{
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = index.graphicsFamily.value(); 
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = new float[1]{1.0f};

    VkPhysicalDeviceFeatures deviceFeatures{}; // 可以根据需要启用特性

    VkDeviceCreateInfo deviceCreateInfoLocal{};
    deviceCreateInfoLocal.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfoLocal.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfoLocal.queueCreateInfoCount = 1;
    deviceCreateInfoLocal.pEnabledFeatures = &deviceFeatures;

    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfoLocal, nullptr, &mDevice);
    if (result != VK_SUCCESS)
    {
        std::cerr << "Failed to create logical device!" << std::endl;
    }
}

void mVKLogicalDevice::DestroyLogicalDevice()
{
    vkDestroyDevice(mDevice, nullptr);
}
