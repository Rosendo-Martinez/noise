#include <glad/glad.h>
#include "CuboidRenderer.h"
#include <iostream>

#define CUBOID_VERTICES_COUNT 36


// Helper functions -------------------------------------------------------------------------


void upload_to_buffer(unsigned int vbo, unsigned int size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


unsigned int create_buffer()
{
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    return vbo;
}


// Assumes VAO is bound already
// stride should be zero for buffers that only contain 1 attribute (i.e not interleaved attributes)
void create_attribute(int index, int vbo, int number_of_floats, int stride, int offset, int divisor)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, number_of_floats, GL_FLOAT, GL_FALSE, stride, (void*) offset);
    glVertexAttribDivisor(index, divisor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Cuboid Renderer ---------------------------------------------------------------------------


CuboidRenderer::CuboidRenderer()
{
    this->init();
}


void CuboidRenderer::init()
{
    const float vertices[] = {
        // Front face
        -0.5f, 1.0f,  0.5f,   0.0f, 0.0f,  1.0f,  // Triangle 1
        0.5f, 1.0f,  0.5f,   0.0f, 0.0f,  1.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,  1.0f,

        -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,  1.0f,  // Triangle 2
        0.5f, 1.0f,  0.5f,   0.0f, 0.0f,  1.0f,
        0.5f, 0.0f,  0.5f,   0.0f, 0.0f,  1.0f,

        // Back face
        -0.5f, 1.0f, -0.5f,   0.0f, 0.0f, -1.0f,  // Triangle 3
        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, -1.0f,
        0.5f, 1.0f, -0.5f,   0.0f, 0.0f, -1.0f,

        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, -1.0f,  // Triangle 4
        0.5f, 1.0f, -0.5f,   0.0f, 0.0f, -1.0f,
        0.5f, 0.0f, -0.5f,   0.0f, 0.0f, -1.0f,

        // Left face
        -0.5f, 1.0f, -0.5f,  -1.0f, 0.0f,  0.0f,  // Triangle 5
        -0.5f, 0.0f, -0.5f,  -1.0f, 0.0f,  0.0f,
        -0.5f, 1.0f,  0.5f,  -1.0f, 0.0f,  0.0f,

        -0.5f, 0.0f, -0.5f,  -1.0f, 0.0f,  0.0f,  // Triangle 6
        -0.5f, 0.0f,  0.5f,  -1.0f, 0.0f,  0.0f,
        -0.5f, 1.0f,  0.5f,  -1.0f, 0.0f,  0.0f,

        // Right face
        0.5f, 1.0f, -0.5f,   1.0f, 0.0f,  0.0f,  // Triangle 7
        0.5f, 1.0f,  0.5f,   1.0f, 0.0f,  0.0f,
        0.5f, 0.0f, -0.5f,   1.0f, 0.0f,  0.0f,

        0.5f, 0.0f, -0.5f,   1.0f, 0.0f,  0.0f,  // Triangle 8
        0.5f, 1.0f,  0.5f,   1.0f, 0.0f,  0.0f,
        0.5f, 0.0f,  0.5f,   1.0f, 0.0f,  0.0f,

        // Top face
        -0.5f, 1.0f, -0.5f,   0.0f, 1.0f,  0.0f,  // Triangle 9
        -0.5f, 1.0f,  0.5f,   0.0f, 1.0f,  0.0f,
        0.5f, 1.0f, -0.5f,   0.0f, 1.0f,  0.0f,

        0.5f, 1.0f, -0.5f,   0.0f, 1.0f,  0.0f,  // Triangle 10
        -0.5f, 1.0f,  0.5f,   0.0f, 1.0f,  0.0f,
        0.5f, 1.0f,  0.5f,   0.0f, 1.0f,  0.0f,

        // Bottom face
        -0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,  // Triangle 11
        0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,

        0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,  // Triangle 12
        0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,
    };

    
    // VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Cube
    unsigned int cube_vbo = create_buffer();
    upload_to_buffer(cube_vbo, sizeof(vertices), vertices);
    create_attribute(0, cube_vbo, 3, 6 * sizeof(float), 0, 0); // position
    create_attribute(1, cube_vbo, 3, 6 * sizeof(float), 3 * sizeof(float), 0); // normal

    // Instancing buffers
    color_vbo = create_buffer();
    create_attribute(2, color_vbo, 3, 0, 0, 1);
    translate_vbo = create_buffer();
    create_attribute(3, translate_vbo, 3, 0, 0, 1);
    scale_vbo = create_buffer();
    create_attribute(4, scale_vbo, 3, 0, 0, 1);

    glBindVertexArray(0);

    this->shader.compile("shaders/cuboid.vs", "shaders/cuboid.fs");
}


void CuboidRenderer::draw(const glm::vec3& color, const glm::vec3& translate, const glm::vec3& scale)
{
    // Create model matrix
    glm::mat4 model(1.0f);
    model[3] = glm::vec4(translate, 1.0f); // translate
    model[0] = model[0] * scale.x; // scale
    model[1] = model[1] * scale.y;
    model[2] = model[2] * scale.z;

    // Set uniforms
    this->shader.use();
    this->shader.setVector("color", color);
    this->shader.setMat4("model", model);

    // Draw cuboid
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, CUBOID_VERTICES_COUNT);
    glBindVertexArray(0);
}


void CuboidRenderer::setProjectionView(const glm::mat4& projection_view)
{
    this->shader.use();
    this->shader.setMat4("projection_view", projection_view);
}


void CuboidRenderer::setDirectionLight(const glm::vec3& direction_to_light)
{
    this->shader.use();
    this->shader.setVector("direction_to_light", direction_to_light);
}


void CuboidRenderer::update_instances(const glm::vec3* color, const glm::vec3* translate, const glm::vec3* scale, int size)
{   
    glBindVertexArray(this->vao);
    upload_to_buffer(color_vbo, sizeof(glm::vec3) * size, &color[0]);
    upload_to_buffer(translate_vbo, sizeof(glm::vec3) * size, &translate[0]);
    upload_to_buffer(scale_vbo, sizeof(glm::vec3) * size, &scale[0]);
    glBindVertexArray(0);

    this->instances = size;
}


void CuboidRenderer::draw_instances()
{
    this->shader.use();

    glBindVertexArray(this->vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, CUBOID_VERTICES_COUNT, this->instances);
    glBindVertexArray(0);
}