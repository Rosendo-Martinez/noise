#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"


// Helper functions ------------------------------------------------------------------


std::string readFile(const char* path)
{
    std::string file;
    std::ifstream ifs (path); // open file

    if (!ifs.is_open()) // error check
    {
        std::cout << "Error: couldn't read file " << path << std::endl;
        return file;
    }

    // load file to buffer
    std::stringstream ss; 
    ss << ifs.rdbuf();

    // close file
    ifs.close();

    // return file as string
    return ss.str();
}


bool checkCompileStatus(unsigned int shaderID, char* infoLog, int length)
{
    int success;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, length, NULL, infoLog);
        return false;
    }
    else 
    {
        return true;
    }
}


bool checkLinkStatus(unsigned int programID, char* infoLog, int length)
{
    int success;

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(programID, length, NULL, infoLog);
        return false;
    }
    else
    {
        return true;
    }
}


unsigned int createShader(const char* source, bool isVertex)
{
    unsigned int shader;
    if (isVertex)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else // fragment
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    return shader;
}


// Shader ---------------------------------------------------------------------------


Shader::Shader() : ID(0) {}


// Compiles and links shader program.
// Logs errors.
void Shader::compile(const char* vsPath, const char* fsPath)
{
    std::string strVertexShader = readFile(vsPath);
    std::string strFragmentShader = readFile(fsPath);

    // Create shaders
    unsigned int vertexShader = createShader(strVertexShader.c_str(), true);
    unsigned int fragmentShader = createShader(strFragmentShader.c_str(), false);

    // Check compile errors
    int length = 512;
    char infoLog[length];
    if (!checkCompileStatus(vertexShader, infoLog, length))
    {
        std::cout << "Error: vertex compilation failed:\n  " << infoLog << std::endl;
    }
    if (!checkCompileStatus(fragmentShader, infoLog, length))
    {
        std::cout << "Error: fragment compilation failed:\n  " << infoLog << std::endl;
    }

    // Create program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check link errors
    if (!checkLinkStatus(shaderProgram, infoLog, length))
    {
        std::cout << "Error: shader linking failed:\n  " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->ID = shaderProgram;
}


void Shader::use()
{
    glUseProgram(this->ID);
}


// Shader: set uniforms ------------------------------------------------------------------------------


void Shader::setVector(const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}


void Shader::setMat4(const char* name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID,name), 1, GL_FALSE, &matrix[0][0]);
}


void Shader::setVector(const char* name, const glm::vec3& vector)
{
    glUniform3fv(glGetUniformLocation(this->ID, name), 1, &vector[0]);  
}