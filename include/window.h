#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


class Window
{
public:

    Window();
    bool init(int width, int height);

    bool shouldClose();
    void terminate();
    void swapBuffers();
    void pollEvents();

    GLFWwindow* window = nullptr;
};