#version 440 core
layout(location=0)in vec3 vertPosition;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vertPosition;
    vec4 pos = projection * view * vec4( vertPosition, 1.);
    gl_Position=pos.xyzz;
}

