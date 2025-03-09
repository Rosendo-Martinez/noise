#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


struct Key
{
    bool is_pressed = false;
    bool duplicate = false;
};

extern Key keys[GLFW_KEY_LAST];

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