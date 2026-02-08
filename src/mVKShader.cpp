//
// Created by 87784 on 2026/2/8.
//

#include "mVKShader.h"
#include <fstream>
#include  <iostream>

VkShaderModule mVKShader::CreateShaderModule(VkDevice logical_device, std::string filePath)
{
    VkShaderModuleCreateInfo createInfo = {};
    //在创建shader module时要注意字节对齐问题，需要重新解释为uint32_t,确保四字节的对齐
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    auto code=this->ReadSpvCode(filePath);
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<uint32_t*>(code.data());//注意字节对齐
    if (!vkCreateShaderModule(logical_device, &createInfo, nullptr, &mShaderModule))
    {
        std::cerr << "Failed to create shader module" << std::endl;
        return VK_NULL_HANDLE;
    }
    return mShaderModule;//本质上是一个handle，永远使用值传递
}

void mVKShader::DestroyShaderModule(VkDevice logical_device)
{
    vkDestroyShaderModule(logical_device,mShaderModule,nullptr);
}

std::vector<char> mVKShader::ReadSpvCode(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary|std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filePath << std::endl;
    }
    size_t fileSize = file.tellg();//计算大小
    std::vector<char> buffer(fileSize);//使用char存储二进制文件
    file.seekg(0);//返回文件头
    file.read(buffer.data(), fileSize);//读取所有字符
    file.close();
    return buffer;
}