#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
out float diffuse;

uniform mat4 projection_view;
uniform mat4 model;
uniform vec3 direction_to_light;


void main()
{
    gl_Position = projection_view * model * vec4(aPos, 1.0);
    diffuse = max(dot(aNorm, direction_to_light), 0.2);
};