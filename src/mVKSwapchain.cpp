#include "mVKSwapchain.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>
#include <numeric>

#include "GLFW/glfw3.h"

void mVKSwapchain::InitSwapChain(VkSurfaceKHR surface,VkPhysicalDevice physical_device, VkDevice logical_device,
    GLFWwindow* window,MyQueueFamilyIndices queue_family_indices)
{
    CheckSurfaceCapabilities(surface,physical_device);
    CheckSurfaceFormats(surface,physical_device);
    CheckSurfacePresentModes(surface,physical_device);
    //创建swapchain
    auto surfaceFormat=ChooseSurfaceFormats();
    auto presentMode=ChoosePresentModes();
    auto extent=ChooseExtent(window);

    //计算swapchain中使用的image的数量
    uint32_t imageCount=swapChainSupportDetail.capabilities.minImageCount+1;
    if (imageCount>swapChainSupportDetail.capabilities.maxImageCount)
    {
        imageCount=swapChainSupportDetail.capabilities.maxImageCount;
    }
   
    uint32_t queueFamilyIndices[]={
        queue_family_indices.graphicsFamily.value(),queue_family_indices.presentFamily.value(),
    };
    VkSwapchainCreateInfoKHR swapchainInfo = {};
    swapchainInfo.sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface=surface;
    swapchainInfo.minImageCount=imageCount;
    swapchainInfo.imageFormat=surfaceFormat.format;
    swapchainInfo.imageColorSpace=surfaceFormat.colorSpace;
    swapchainInfo.imageExtent=extent;
    swapchainInfo.presentMode=presentMode;
    swapchainInfo.imageArrayLayers=1;//指定每张图片由多少层构成
    swapchainInfo.imageUsage=VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//图片用途
    if (queue_family_indices.graphicsFamily.value()!=queue_family_indices.presentFamily.value())
    {
        //两个不同队列
        //并发模式，该swapchain中的图片预先被指定为可以被多个queue family同时访问
        swapchainInfo.imageSharingMode=VK_SHARING_MODE_CONCURRENT;
        //标记哪些queue family会共享
        swapchainInfo.queueFamilyIndexCount=2;
        swapchainInfo.pQueueFamilyIndices=queueFamilyIndices;
    }
    else
    {
        //本质上是同一队列
        //独占模式，swapchain中的image只能被一个queue family碰，拥有最好的性能
        swapchainInfo.imageSharingMode=VK_SHARING_MODE_EXCLUSIVE;
        swapchainInfo.queueFamilyIndexCount=0;
        swapchainInfo.pQueueFamilyIndices=nullptr;
    }
    swapchainInfo.preTransform=swapChainSupportDetail.capabilities.currentTransform;//可以指定旋转
    swapchainInfo.compositeAlpha=VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.clipped=VK_TRUE;//是否跳过被遮挡的区域不去绘制
    swapchainInfo.oldSwapchain=VK_NULL_HANDLE;//在窗口大小被调整时需要重新设置swapchain
    
    auto result=vkCreateSwapchainKHR(logical_device,&swapchainInfo,nullptr,&swapChain);
    if (result!=VK_SUCCESS)
    {
        std::cerr<<"failed to create swapchain"<<std::endl;
    }
    //创建image handle
    vkGetSwapchainImagesKHR(logical_device,swapChain,&imageCount,nullptr);
    this->swapChainImages.resize(imageCount);
    result = vkGetSwapchainImagesKHR(logical_device,swapChain,&imageCount,this->swapChainImages.data());
    if (result!=VK_SUCCESS)
    {
        std::cerr<<"failed to get swapchain images"<<std::endl;
    }
    CreateSwapChainImageViews(logical_device);
}

void mVKSwapchain::DestroySwapChain(VkDevice logical_device)
{
    for (auto imageview:swapChainImageViews)
    {
        vkDestroyImageView(logical_device,imageview,nullptr);
    }
    vkDestroySwapchainKHR(logical_device,swapChain,nullptr);
}


void mVKSwapchain::CheckSurfaceCapabilities(VkSurfaceKHR surface,VkPhysicalDevice physical_device)
{
    VkResult result=
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device,surface,&this->swapChainSupportDetail.capabilities);
    if(result!=VK_SUCCESS)
    {
        std::cout<<"Failed to get surface capabilities"<<std::endl;
    }
}

void mVKSwapchain::CheckSurfaceFormats(VkSurfaceKHR surface,VkPhysicalDevice physical_device)
{
    uint32_t formatCount;
    VkResult result= vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,surface,&formatCount,nullptr);
    if(result!=VK_SUCCESS)
    {
        std::cerr<<"Failed to get surface formats!"<<std::endl;
    }
    this->swapChainSupportDetail.formats.resize(formatCount);
    result= vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,surface,&formatCount,this->swapChainSupportDetail.formats.data());
    if(result!=VK_SUCCESS)
    {
        std::cerr<<"Failed to get surface formats!"<<std::endl;
    }
    if (this->swapChainSupportDetail.formats.empty())
    {
        std::cerr<<"Empty!Failed to get surface formats!"<<std::endl;
    }
}

void mVKSwapchain::CheckSurfacePresentModes(VkSurfaceKHR surface,VkPhysicalDevice physical_device)
{
    uint32_t presentModeCount;
    VkResult result= vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,surface,&presentModeCount,nullptr);
    if(result!=VK_SUCCESS)
    {
        std::cerr<<"Fail to get presentent modes"<<std::endl;
    }
    this->swapChainSupportDetail.presentModes.resize(presentModeCount);
    result=vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,surface,&presentModeCount,
        this->swapChainSupportDetail.presentModes.data());
    if(result!=VK_SUCCESS)
    {
        std::cerr<<"Failed to get presenting modes!"<<std::endl;
    }
    if (this->swapChainSupportDetail.presentModes.empty())
    {
        std::cerr<<"Empty! Failed to get presenting modes!"<<std::endl;
    }
}

//从所有的format中选择一种
VkSurfaceFormatKHR mVKSwapchain::ChooseSurfaceFormats()
{
    for (auto& format : this->swapChainSupportDetail.formats)
    {
        if (format.format==VK_FORMAT_B8G8R8_SRGB&&format.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
           return format;
        }
    }
    return this->swapChainSupportDetail.formats[0];
}

VkPresentModeKHR mVKSwapchain::ChoosePresentModes()
{
    for (auto& presentMode : this->swapChainSupportDetail.presentModes)
    {
        if (presentMode==VK_PRESENT_MODE_MAILBOX_KHR)//优化的
        {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D mVKSwapchain::ChooseExtent(GLFWwindow* window)
{
    //判断是否能够调整窗口大小
    if (this->swapChainSupportDetail.capabilities.currentExtent.width!=std::numeric_limits<uint32_t>::max())
    {
         //不能，直接return
        return this->swapChainSupportDetail.capabilities.currentExtent;
    }
    else//如果可以调整窗口大小
    {
        int width,height;
        glfwGetFramebufferSize(window,&width,&height);
        VkExtent2D actualExtent={static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
        actualExtent.width=std::clamp(actualExtent.width,
            swapChainSupportDetail.capabilities.minImageExtent.width,
            swapChainSupportDetail.capabilities.maxImageExtent.width);
        actualExtent.height=std::clamp(actualExtent.width,
            swapChainSupportDetail.capabilities.maxImageExtent.height,
            swapChainSupportDetail.capabilities.minImageExtent.height);
        return actualExtent;
    }
}

void mVKSwapchain::CreateSwapChainImageViews(VkDevice device)
{
    swapChainImageViews.resize(swapChainImages.size());
    for (uint32_t i=0;i<swapChainImages.size();i++)
    {
        VkImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image=swapChainImages[i];
        imageViewInfo.viewType=VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format=ChooseSurfaceFormats().format;
        imageViewInfo.components.r=VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g=VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b=VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel=0;
        imageViewInfo.subresourceRange.levelCount=1;
        imageViewInfo.subresourceRange.baseArrayLayer=0;
        imageViewInfo.subresourceRange.layerCount=1;
        if (vkCreateImageView(device,&imageViewInfo,nullptr,&swapChainImageViews[i])!=VK_SUCCESS)
        {
            std::cerr<<"Failed to create swapchain image views!"<<std::endl;
        }
    }
}