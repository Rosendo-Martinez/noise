#include "window.h"
#include "Shader.h"

int main() 
{
    Window window;
    if (!window.init(640, 480))
    {
        return 0;
    }

    Shader shader;
    shader.compile("shaders/shader.vs", "shaders/shader.fs");

    window.loop();

    return 0;
}