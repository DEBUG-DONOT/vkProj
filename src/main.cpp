#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "vkApp.h"

int main() {

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