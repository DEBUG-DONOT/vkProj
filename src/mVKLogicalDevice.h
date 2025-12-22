#pragma once
#include <vulkan/vulkan.h>
#include "mVKPhysicalDevice.h"
#include "utils.h"
//TODO: logical device和pyhsical device需要封装在一起作为一个device类

class mVKLogicalDevice
{
public:
    void CreateLogicalDevice(MyQueueFamilyIndices index, VkPhysicalDevice physicalDevice);
    void DestroyLogicalDevice();
    inline VkDevice GetLogicalDevice() { return mDevice; }
private:
    VkDevice mDevice;
};