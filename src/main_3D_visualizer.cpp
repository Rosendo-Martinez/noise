#include <iostream>
#include "window.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "Noise.h"
#include "SquareRenderer.h"
#include "CuboidRenderer.h"
#include "Image.h"
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

Image_Grayscale* height_map = nullptr;
int height_map_width;
int height_map_height;

enum class Mode
{
    render3D, render2D
} mode;

enum class Noise_Type
{
    value_linear, value_cubic, perlin_hermite
} noise_type;
bool use_octave = false;


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


float get_noise(float x);
float get_noise_octave(float x);
void construct_height_map();
void construct_instances();


bool init()
{
    window = new Window();
    if (!window->init(window_width, window_height))
    {
        return false;
    }

    mode = Mode::render3D;

    line = new LineRenderer();
    square = new SquareRenderer();
    cuboid = new CuboidRenderer();
    cam_ortho = new CameraOrthographic(-2.0f, 22.0f, -2.0f, 2.0f, -1.0f, 1.0f);
    cam_pers = new CameraPerspective(cam_pos, glm::vec3(0.0f, 0.5f, 0.0f), glm::radians(45.0f), window_width, window_height);

    direction_to_light = glm::normalize(glm::vec3(2.0f, 4.0f, 1.0f));

    construct_height_map();
    construct_instances();

    return true;
}


// Rendering functions -----------------------------------------------------------------------------------------------------------


void render2D()
{
    glDisable(GL_DEPTH_TEST);
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
                glm::vec3(x, get_noise_octave(x), 0.0f),
                glm::vec3(x + dx, get_noise_octave(x + dx), 0.0f),
                LINE_WIDTH + 1.25
            );

            line->draw(
                glm::vec3(0.0, 1.0, 0.0),
                glm::vec3(x, get_noise(x), 0.0f),
                glm::vec3(x + dx, get_noise(x + dx), 0.0f),
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
}


void render3D()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cuboid->setProjectionView(cam_pers->get_projection_view_matrix());
    cuboid->setDirectionLight(direction_to_light);
    cuboid->draw_instances();
}


void render()
{
    if (mode == Mode::render2D)
    {
        render2D();
    }
    else
    {
        render3D();
    }

    window->swapBuffers();
}


// Input ------------------------------------------------------------------------


void input()
{
    window->pollEvents();

    if (keys[GLFW_KEY_M].is_pressed && !keys[GLFW_KEY_M].duplicate)
    {
        mode = mode == Mode::render2D ? Mode::render3D : Mode::render2D;
        keys[GLFW_KEY_M].duplicate = true;
    }

    if (keys[GLFW_KEY_SPACE].is_pressed && !keys[GLFW_KEY_SPACE].duplicate)
    {
        if (noise_type == Noise_Type::value_linear)
        {
            noise_type = Noise_Type::value_cubic;
        }
        else if (noise_type == Noise_Type::value_cubic)
        {
            noise_type = Noise_Type::perlin_hermite;
        }
        else
        {
            noise_type = Noise_Type::value_linear;
        }

        if (mode == Mode::render3D)
        {
            construct_height_map();
            construct_instances();
        }

        keys[GLFW_KEY_SPACE].duplicate = true;
    }

    if (keys[GLFW_KEY_N].is_pressed && !keys[GLFW_KEY_N].duplicate)
    {
        keys[GLFW_KEY_N].duplicate = true;

        if (mode == Mode::render3D)
        {
            use_octave = !use_octave;
            construct_height_map();
            construct_instances();
        }
    }
}


// Update -----------------------------------------------------------------------


void update()
{
    if (mode == Mode::render3D)
    {
        cam_pos.z = cos(glfwGetTime() / 2.0f) * 7.0f;
        cam_pos.x = sin(glfwGetTime() / 2.0f) * 7.0f;
        cam_pos.y = sin(glfwGetTime() / 2.0f) * 2.0f + 5.0f;
        cam_pers->move(cam_pos);
    }
}


// Helper functions ---------------------------------------------------------------


float get_noise(float x)
{
    if (noise_type == Noise_Type::value_linear)
    {
        return Noise1D::sample_value_linear(x);
    }
    else if (noise_type == Noise_Type::value_cubic)
    {
        return Noise1D::sample_value_cubic(x);
    }
    else
    {
        return Noise1D::sample_perlin_hermite(x);
    }
}


float get_noise_octave(float x)
{
    if (noise_type == Noise_Type::value_linear)
    {
        return Noise1D::sample_value_octave_linear(x);
    }
    else if (noise_type == Noise_Type::value_cubic)
    {
        return Noise1D::sample_value_octave_cubic(x);
    }
    else
    {
        return Noise1D::sample_perlin_octave_hermite(x);
    }
}


float get_noise(float x, float y)
{
    if (use_octave)
    {
        if (noise_type == Noise_Type::value_linear)
        {
            return Noise2D::sample_value_octave_bilinear(x,y);
        }
        else if (noise_type == Noise_Type::value_cubic)
        {
            return Noise2D::sample_value_octave_bicubic(x, y);
        }
        else
        {
            return Noise2D::sample_perlin_octave_hermite(x,y);
        }
    }
    else
    {
        if (noise_type == Noise_Type::value_linear)
        {
            return Noise2D::sample_value_bilinear(x,y);
        }
        else if (noise_type == Noise_Type::value_cubic)
        {
            return Noise2D::sample_value_bicubic(x, y);
        }
        else
        {
            return Noise2D::sample_perlin_hermite(x,y);
        }
    }
}


void construct_height_map()
{
    if (height_map != nullptr)
    {
        delete height_map;
    }

    height_map_width = 200;
    height_map_height = 200;
    height_map = new Image_Grayscale(height_map_width, height_map_height);
    int SAMPLE_INTERVAL_WIDTH = 10;
    int SAMPLE_INTERVAL_HEIGHT = 10;
    float dx = ((float) SAMPLE_INTERVAL_WIDTH) / ((float) height_map_width);
    float dy = ((float) SAMPLE_INTERVAL_HEIGHT) / ((float) height_map_height);
    float pixel_00_x = dx * 0.5f;
    float pixel_00_y = dy * 0.5f;

    for (int y = 0; y < height_map_height; y++)
    {
        for (int x = 0; x < height_map_width; x++)
        {
            float pixel_x = pixel_00_x + (x * dx);
            float pixel_y = pixel_00_y + (y * dy);

            height_map->setPixel(x, y, get_noise(pixel_x, pixel_y));
        }
    }
    height_map->normalize();
}


void construct_instances()
{
    int instances = height_map_width * height_map_height;
    glm::vec3* color_array = new glm::vec3[instances];
    glm::vec3* translate_array = new glm::vec3[instances];
    glm::vec3* scale_array = new glm::vec3[instances];

    float cuboid_width_x = 0.1f;
    float cuboid_length_z = 0.1f;
    float cuboid_height_scaling = 2.0f;
    float cuboid00_x = cuboid_width_x * 0.5f - (cuboid_width_x * height_map_width * 0.5f);
    float cuboid00_z = cuboid_length_z * 0.5f - (cuboid_length_z * height_map_height * 0.5f);

    int i = 0;
    for (int z = 0; z < height_map_height; z++)
    {
        for (int x = 0; x < height_map_width; x++)
        {
            color_array[i] = glm::vec3(1.0f);
            translate_array[i] = glm::vec3(cuboid00_x + cuboid_width_x * x, 0.0f, cuboid00_z + cuboid_length_z * z);
            scale_array[i] = glm::vec3(cuboid_width_x, (height_map->getPixel(x,z) + 0.1) * cuboid_height_scaling, cuboid_length_z);
            i++;
        }
    }

    cuboid->update_instances(color_array, translate_array, scale_array, instances);
    delete[] color_array;
    delete[] translate_array;
    delete[] scale_array;
}