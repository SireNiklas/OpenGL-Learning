#include "ogl_window.hpp"

// std
#include <iostream>
#include <stdexcept>

namespace ogl {

    // Constructor
    OglWindow::OglWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{name} {
        initWindow();
    }

    OglWindow::~OglWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void OglWindow::initWindow() {

        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create Window
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

        if (!window) {
            std::cout << "Failed to make window!" << std::endl;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        
        if (!glfwGetCurrentContext()) {
            std::cout << "Failed to make context current!" << std::endl;
            glfwTerminate();
            glfwDestroyWindow(window);
            return;
        }

        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to attain a GLFWwindow Context!" << std::endl;
            glfwTerminate();
            glfwDestroyWindow(window);
        }

        std::cout << glGetString(GL_VERSION) << std::endl;
    }

}