#include <glad/glad.h>
#include "SquareRenderer.h"


// Helper functions ---------------------------------------------------------


void gram_schmidt(glm::mat4& matrix, const glm::vec3& new_z)
{
    // CREDIT: Gram-Schmidt process

    glm::vec3 old_x (matrix[0]);
    glm::vec3 old_y (matrix[1]);
    glm::vec3 old_z (matrix[2]);
    
    glm::vec3 new_x;
    glm::vec3 new_y;

    if (new_z != old_x)
    {
        new_y = glm::normalize(glm::cross(new_z, old_x));
        new_x = glm::normalize(glm::cross(new_y, new_z));

    }
    else
    {
        new_x = glm::normalize(glm::cross(old_y, new_z));
        new_y = glm::normalize(glm::cross(new_z, old_x));
    }

    matrix[0] = glm::vec4(new_x, 0.0f);
    matrix[1] = glm::vec4(new_y, 0.0f);
    matrix[2] = glm::vec4(new_z, 0.0f);
}


// SquareRenderer -----------------------------------------------------------


SquareRenderer::SquareRenderer() 
{
    this->init();
}


void SquareRenderer::init()
{
    const char* VERTEX_SHADER = "shaders/square.vs";
    const char* FRAGMENT_SHADER = "shaders/square.fs";

    // square
    const float vertices[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,

        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // Generate IDs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Copy data to buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->VAO = VAO;
    this->shader.compile(VERTEX_SHADER, FRAGMENT_SHADER);
}


// Rotation in degrees
void SquareRenderer::draw(const glm::vec3& center, const glm::vec2& scale, const glm::vec3& normal, float rotation, const glm::vec3& color)
{
    assert(glm::length(normal) == 1.0f);

    // Create model matrix
    glm::mat4 model(1.0f);
    model = glm::translate(model, center);
    gram_schmidt(model, normal);
    model = glm::rotate(model, glm::radians(rotation), normal);
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    // Set uniforms
    this->shader.use();
    this->shader.setVector("color", color);
    this->shader.setMat4("model", model);

    // Draw square
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind VAO, and shader
    glBindVertexArray(0);
    glUseProgram(0);
}


void SquareRenderer::setProjection(const glm::mat4& projection)
{
    this->shader.use();
    this->shader.setMat4("projection", projection);
    glUseProgram(0);
}