#version 460 core

layout (location = 1) uniform bool colorFlag;
layout (location = 2) uniform vec3 uniformColor;

in vec3 outColor;
out vec4 fragColor;

void main()
{
    if (colorFlag)
        fragColor = vec4(uniformColor, 1.f);
    else
        fragColor = vec4(outColor, 1.f);
} 