//
// Created by 87784 on 2026/2/8.
//

#ifndef HELLOVULKAN_MVKSHADER_H
#define HELLOVULKAN_MVKSHADER_H
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
/*
 * 对shader的读取
 */

class mVKShader
{
public:
    VkShaderModule CreateShaderModule(VkDevice logical_device,std::string filePath);
    void DestroyShaderModule(VkDevice logical_device);
private:
    VkShaderModule mShaderModule=VK_NULL_HANDLE;
    std::vector<char> ReadSpvCode(std::string filePath);

};


#endif //HELLOVULKAN_MVKSHADER_H