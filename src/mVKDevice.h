//
// Created by 87784 on 2025/12/23.
//

#ifndef HELLOVULKAN_MVKDEVICE_H
#define HELLOVULKAN_MVKDEVICE_H

/*
 * 该类是对于physical device和logical device的封装
 */
#include <vulkan/vulkan.h>
#include "utils.h"
class mVKDevice
{
public:
    void InitDevice(VkInstance instance);
    void DestroyDevice();
private:
    void PickPhysicalDevice(VkInstance instance);
    void CreateLogicalDevice(MyQueueFamilyIndices index, VkPhysicalDevice physicalDevice);
    void GetGraphicsQueues(MyQueueFamilyIndices index,VkDevice device);
private:
    MyQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _device);
    uint32_t RatePhysicalDevice(VkPhysicalDevice _device);
private:
    //需要默认赋值为空
    VkPhysicalDevice physicalDevice=VK_NULL_HANDLE;
    VkDevice logicalDevice=VK_NULL_HANDLE;
    VkQueue graphicsQueue=VK_NULL_HANDLE;
};


#endif //HELLOVULKAN_MVKDEVICE_H