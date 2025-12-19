#define GLFW_INCLUDE_NONE

#include "volk.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "vkApp.h"

int main() {
    if (volkInitialize() != VK_SUCCESS) {
        std::cerr << "Failed to initialize volk!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Volk initialized successfully." << std::endl;
    VulkanApp app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}