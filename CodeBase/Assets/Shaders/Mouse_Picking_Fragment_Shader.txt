#version 440 core

out vec4 FragColor;

in vec3 Color_FS_in;

void main()
{
    FragColor = vec4(Color_FS_in, 1.0);
} 