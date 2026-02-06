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
    void InitDevice(VkInstance instance,VkSurfaceKHR surface);
    void DestroyDevice();
    MyQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _device,VkSurfaceKHR surface);
    inline VkDevice GetDevice(){return this->logicalDevice;};
    inline VkPhysicalDevice GetPhysicalDevice(){return this->physicalDevice;};
private:
    void PickPhysicalDevice(VkInstance instance);
    void CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
    //void EnablePhysicalDeviceExtension(VkPhysicalDevice physicalDevice);
    void CreateLogicalDevice(MyQueueFamilyIndices index, VkPhysicalDevice physicalDevice);
    void GetGraphicsQueues(MyQueueFamilyIndices index,VkDevice device);
    void GetPresentQueues(MyQueueFamilyIndices index,VkDevice device);
private:
    uint32_t RatePhysicalDevice(VkPhysicalDevice _device);
private:
    //需要默认赋值为空
    VkPhysicalDevice physicalDevice=VK_NULL_HANDLE;
    VkDevice logicalDevice=VK_NULL_HANDLE;
    VkQueue graphicsQueue=VK_NULL_HANDLE;
    VkQueue presentQueue=VK_NULL_HANDLE;
};


#endif //HELLOVULKAN_MVKDEVICE_H