#pragma once
#include "utils.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
//对swapchain的封装
class mVKSwapchain
{
public:
    void InitSwapChain(VkSurfaceKHR surface,VkPhysicalDevice physical_device,VkDevice logical_device,
        GLFWwindow* window,MyQueueFamilyIndices queue_family_indices);
    void DestroySwapChain(VkDevice logical_device);
private:
    //选择创建swapchian需要的参数
    void CheckSurfaceCapabilities(VkSurfaceKHR surface,VkPhysicalDevice physical_device);
    void CheckSurfaceFormats(VkSurfaceKHR surface,VkPhysicalDevice physical_device);
    void CheckSurfacePresentModes(VkSurfaceKHR surface,VkPhysicalDevice physical_device);

    VkSurfaceFormatKHR ChooseSurfaceFormats();
    VkPresentModeKHR ChoosePresentModes();
    VkExtent2D ChooseExtent(GLFWwindow* window);

    void CreateSwapChainImageViews(VkDevice device);
private:
    SwapChainSupportDetails swapChainSupportDetail;
    VkSwapchainKHR swapChain=VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
};