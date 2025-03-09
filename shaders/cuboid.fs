#version 330 core
in float diffuse;
out vec4 FragColor;

uniform vec3 color;


void main()
{
    FragColor = vec4(color * diffuse, 1.0);
};