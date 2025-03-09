#include <glad/glad.h>
#include "CuboidRenderer.h"


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

    // Generate IDs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);    
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Attribute: normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),  (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->VAO = VAO;
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
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Unbind
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