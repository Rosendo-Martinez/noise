#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


class Window
{
public:

    Window();
    bool init(int width, int height);
    void loop();

    GLFWwindow* window = nullptr;
};