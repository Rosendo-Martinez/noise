#include "window.h"
#include <iostream>


void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << '\n';
}


// Window ---------------------------------------------------------


Window::Window() {}


// Returns false if some error occurred, else true.
bool Window::init(int width, int height)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, "TITLE", NULL, NULL);
    if (!this->window)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: failed to initialize GLAD" << '\n';
        return false;
    }

    return true;
}