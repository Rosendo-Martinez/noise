#include <iostream>
#include "window.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "Noise.h"
#include "SquareRenderer.h"
#include "CuboidRenderer.h"
#include <cmath>

const float CLEAR_COLOR[4] = {0.6, 0.0, 0.1, 0.0};

int window_width = 800;
int window_height = 800;

glm::vec3 cam_pos (0.0f);

glm::vec3 direction_to_light;

Window* window = nullptr;
LineRenderer* line = nullptr;
SquareRenderer* square = nullptr;
CuboidRenderer* cuboid = nullptr;
CameraOrthographic* cam_ortho = nullptr;
CameraPerspective* cam_pers = nullptr;

bool init();
void render();
void update();
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
        update();
        render();
    }

    window->terminate();
    return 0;
}


// Helper functions: -----------------------------------------------------------------------


bool init()
{
    window = new Window();
    if (!window->init(window_width, window_height))
    {
        return false;
    }

    line = new LineRenderer();
    square = new SquareRenderer();
    cuboid = new CuboidRenderer();
    cam_ortho = new CameraOrthographic(-2.0f, 22.0f, -2.0f, 2.0f, -1.0f, 1.0f);
    cam_pers = new CameraPerspective(cam_pos, glm::vec3(0.0f, 0.5f, 0.0f), glm::radians(45.0f), window_width, window_height);

    direction_to_light = glm::normalize(glm::vec3(2.0f, 4.0f, 1.0f));

    return true;
}


// Rendering functions -----------------------------------------------------------------------------------------------------------


void render2D()
{
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw square background
    square->setProjection(cam_ortho->get_projection_view_matrix());
    square->draw(glm::vec3(10.0f,0.0f,0.0f), glm::vec2(20.0f, 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(0.4f));

    glm::vec3 LINE_COLOR (1.0f);
    float LINE_WIDTH = 1.0f;
    line->setProjection(cam_ortho->get_projection_view_matrix());

    // Rendering: with list 
    // {
    //     int x_first = 0;
    //     int x_last = 20;
    //     int count = 200;
    //     line->setProjection(cam_ortho->get_projection_view_matrix());
    //     std::vector<glm::vec2> noise_samples = Noise1D::sample(x_first, x_last, count);
    //     for (int i = 0; i < noise_samples.size() - 1; i++)
    //     {
    //         assert(i < noise_samples.size());
    //         assert(i + 1 < noise_samples.size());


    //         line->draw(
    //             LINE_COLOR, 
    //             glm::vec3(noise_samples[i], 0.0f), 
    //             glm::vec3(noise_samples[i+1], 0.0f), 
    //             LINE_WIDTH
    //         );
    //     }
    // }

    // Rendering: w/o list
    {
        float x_first = 0.0f;
        float x_last = 20.0f;
        int count = 500;
        float dx = (x_last - x_first) / ((float) (count - 1));

        float x = x_first;
        for (int i = 0; i < count - 1; i++)
        {
            line->draw(
                LINE_COLOR,
                glm::vec3(x, Noise1D::sample_value_octave_linear(x), 0.0f),
                glm::vec3(x + dx, Noise1D::sample_value_octave_linear(x + dx), 0.0f),
                LINE_WIDTH + 1.25
            );

            line->draw(
                glm::vec3(0.0, 1.0, 0.0),
                glm::vec3(x, Noise1D::sample_value_linear(x), 0.0f),
                glm::vec3(x + dx, Noise1D::sample_value_linear(x + dx), 0.0f),
                LINE_WIDTH + 2.5f
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


void render3D()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cuboid->setProjectionView(cam_pers->get_projection_view_matrix());
    cuboid->setDirectionLight(direction_to_light);
    cuboid->draw(glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));

    window->swapBuffers();
}


void render()
{
    render3D();
}


// Input ------------------------------------------------------------------------


void input()
{
    window->pollEvents();
}


// Update -----------------------------------------------------------------------


void update()
{
    cam_pos.z = cos(glfwGetTime() / 4.0f) * 3.0f;
    cam_pos.x = sin(glfwGetTime() / 4.0f) * 3.0f;
    cam_pos.y = sin(glfwGetTime() / 4.0f) * 2.5f;
    cam_pers->move(cam_pos);
}