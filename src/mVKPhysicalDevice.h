#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "utils.h"
/*
* 物理设备封装类
* 同时我们将queue family的选择也放在这里，因为queue family是物理设备绑定的
*/

class mVKPhysicalDevice
{
public:
private:
    void PickPhysicalDevice(VkInstance instance);//选择物理设备
    uint32_t RatePhysicalDevice(VkPhysicalDevice _device);//评估物理设备

    MyQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _device);//寻找队列族
private:
    VkPhysicalDevice mPhysicalDevice=VK_NULL_HANDLE;//vk_null_handle本质上是对不同的nullptr的包装
};

