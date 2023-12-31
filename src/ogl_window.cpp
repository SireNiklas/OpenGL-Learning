#include "ogl_window.hpp"

// std
#include <iostream>
#include <stdexcept>
#include <cassert>

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

        // Setting the OpenGL context version is 4.6 hence Major and Minor versions, and setting the profile to Core. | GLFW_OPENGL_COMPAT_PROFILE can be set, but is also set inplace of core.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Required for the Profile.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create Window
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

        if (!window) {
            std::cout << "Failed to make window!" << std::endl;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);
        
        if (!glfwGetCurrentContext()) {
            assert(!glfwGetCurrentContext() && "Failed to make context current!");
            glfwTerminate();
            glfwDestroyWindow(window);
            return;
        }

        if (glewInit() != GLEW_OK) {
            assert(glewInit() != GLEW_OK && "Failed to attain a GLFWwindow Context!");
            glfwTerminate();
            glfwDestroyWindow(window);
            return;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;
    }

}