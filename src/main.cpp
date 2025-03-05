#include "window.h"
#include "Camera.h"
#include "LineRenderer.h"


const float CLEAR_COLOR[4] = {0.6, 0.0, 0.1, 0.0};

Window* window = nullptr;
LineRenderer* line = nullptr;
CameraOrthographic* cam = nullptr;

void render();
void input();

int main() 
{
    window = new Window();
    if (!window->init(700, 700))
    {
        return 0;
    }
    line = new LineRenderer();
    cam = new CameraOrthographic(0, 20, 0, 1.5, -1.0, 1.0);

    while (!window->shouldClose())
    {
        input();
        render();
    }

    window->terminate();
    // Free other shit?
    return 0;
}


void render()
{
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 line_color (1.0f);
    glm::vec3 line_start (0.0f);
    glm::vec3 line_end (20.0f, 1.5f, 0.0f);
    float line_width = 2.0f;

    line->setProjection(cam->get());
    line->draw(line_color, line_start, line_end, line_width);

    window->swapBuffers();
}

void input()
{
    window->pollEvents();
}