#include "window.h"
#include <iostream>


Key keys[GLFW_KEY_LAST];


void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << '\n';
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        keys[key].is_pressed = true;
        keys[key].duplicate = false;
    }
    else if (action == GLFW_RELEASE)
    {
        keys[key].is_pressed = false;
        keys[key].duplicate = false;
    }
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

    glfwSetKeyCallback(window, key_callback);

    return true;
}


void Window::pollEvents()
{
    glfwPollEvents();
}


bool Window::shouldClose()
{
    return glfwWindowShouldClose(this->window);
}


void Window::swapBuffers()
{
    glfwSwapBuffers(this->window);
}


void Window::terminate()
{
    glfwTerminate();
}