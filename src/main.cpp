#include <iostream>
#include "window.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "Noise.h"

const float CLEAR_COLOR[4] = {0.6, 0.0, 0.1, 0.0};

Window* window = nullptr;
LineRenderer* line = nullptr;
CameraOrthographic* cam = nullptr;

bool init();
void render();
void input();


int main() 
{
    if (!init())
    {
        return 0;
    }

    while (!window->shouldClose())
    {
        input();
        render();
    }

    window->terminate();
    return 0;
}


// Function defintions -----------------------------------------------------------------------


bool init()
{
    window = new Window();
    if (!window->init(700, 700))
    {
        return false;
    }

    line = new LineRenderer();
    cam = new CameraOrthographic(-2.0f, 22.0f, -2.0f, 2.0f, -1.0f, 1.0f);

    return true;
}


void render()
{
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    int x_first = 0;
    int x_last = 20;
    int count = 200;
    std::vector<glm::vec2> noise_samples = Value_Noise_1D::sample(x_first, x_last, count);

    line->setProjection(cam->get());
    for (int i = 0; i < noise_samples.size() - 1; i++)
    {
        assert(i < noise_samples.size());
        assert(i + 1 < noise_samples.size());

        glm::vec3 color (1.0f);
        float width = 1.0f;

        line->draw(
            color, 
            glm::vec3(noise_samples[i], 0.0f), 
            glm::vec3(noise_samples[i+1], 0.0f), 
            width
        );
    }

    window->swapBuffers();
}


void input()
{
    window->pollEvents();
}