#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aTranslate;
layout (location = 4) in vec3 aScale;

out vec3 color;

uniform mat4 projection_view;
uniform mat4 model;
uniform vec3 direction_to_light;

void main()
{
    gl_Position = projection_view * vec4((aPos * aScale) + aTranslate, 1.0);

    // Shading
    float diffuse = max(dot(aNorm, direction_to_light), 0.2);
    color = aColor * diffuse;
};