#include <glad/glad.h>
#include "LineRenderer.h"


const char* VERTEX_SHADER = "shaders/line.vs";
const char* FRAGMENT_SHADER = "shaders/line.fs";


LineRenderer::LineRenderer() 
{
    this->init();    
}


void LineRenderer::init()
{
    const float vertices[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->VAO = VAO;
    
    // Create shader program
    this->shader.compile(VERTEX_SHADER, FRAGMENT_SHADER);
}


void LineRenderer::draw(const glm::vec3& color, const glm::vec3& start, const glm::vec3& end, float width)
{
    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, start);
    model[0] = glm::vec4(end - start, 0.0f);

    // Set uniforms
    this->shader.use();
    this->shader.setVector("color", color);
    this->shader.setMat4("model", model);

    // Draw line
    glBindVertexArray(this->VAO);
    glLineWidth(width);
    glDrawArrays(GL_LINES, 0, 2);

    // Unbind VAO, and program
    glBindVertexArray(0);
    glUseProgram(0);
}


void LineRenderer::setProjection(const glm::mat4& projection)
{
    this->shader.use();
    this->shader.setMat4("projection", projection);
    glUseProgram(0);
}