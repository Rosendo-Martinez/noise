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


    glm::vec3 LINE_COLOR (1.0f);
    float LINE_WIDTH = 1.0f;

    // Rendering: with list 
    {
        int x_first = 0;
        int x_last = 20;
        int count = 200;
        line->setProjection(cam->get());
        std::vector<glm::vec2> noise_samples = Value_Noise_1D::sample(x_first, x_last, count);
        for (int i = 0; i < noise_samples.size() - 1; i++)
        {
            assert(i < noise_samples.size());
            assert(i + 1 < noise_samples.size());


            line->draw(
                LINE_COLOR, 
                glm::vec3(noise_samples[i], 0.0f), 
                glm::vec3(noise_samples[i+1], 0.0f), 
                LINE_WIDTH
            );
        }
    }

    // Rendering: w/o list
    {
        float x_first = 0.0f;
        float x_last = 20.0f;
        int count = 400;
        float dx = (x_last - x_first) / ((float) (count - 1));
        float freq = 1.2f;

        float x = x_first;
        line->setProjection(cam->get());
        for (int i = 0; i < count - 1; i++)
        {
            line->draw(
                LINE_COLOR,
                glm::vec3(x, Value_Noise_1D::sample(x, freq), 0.0f),
                glm::vec3(x + dx, Value_Noise_1D::sample(x + dx, freq), 0.0f),
                LINE_WIDTH
            );

            x = x + dx;
        }
    }

    // left edge
    line->draw(
        LINE_COLOR,
        glm::vec3(0,-1,0),
        glm::vec3(0,1,0),
        LINE_WIDTH
    );
    // right edge
    line->draw(
        LINE_COLOR,
        glm::vec3(20,-1,0),
        glm::vec3(20,1,0),
        LINE_WIDTH
    );
    // top edge
    line->draw(
        LINE_COLOR,
        glm::vec3(0,1,0),
        glm::vec3(20,1,0),
        LINE_WIDTH
    );
    // bottom edge
    line->draw(
        LINE_COLOR,
        glm::vec3(0,-1,0),
        glm::vec3(20,-1,0),
        LINE_WIDTH
    );

    window->swapBuffers();
}


void input()
{
    window->pollEvents();
}