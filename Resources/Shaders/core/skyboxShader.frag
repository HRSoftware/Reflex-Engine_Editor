#version 440 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube mapCubeMap;

void main()
{
  FragColor=texture(mapCubeMap,TexCoords);
}

