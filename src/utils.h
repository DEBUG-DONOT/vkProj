#pragma once
#include <vulkan/vulkan.h>
#include <optional>
class MyQueueFamilyIndices {
public:
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    inline bool isComplete() {
        //return graphicsFamily.has_value() ;
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
