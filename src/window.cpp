#include "window.h"
#include <iostream>

const float CLEAR_COLOR[4] = {0.6, 0.0, 0.1, 0.0};


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


void Window::loop()
{
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);

    while (!glfwWindowShouldClose(this->window))
    {
        // Input
        glfwPollEvents();

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(this->window);
    }
}