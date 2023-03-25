#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTex;
//out put 
out vec2 textCoordinate;

uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position =view*model*vec4(position,1.0f);
    textCoordinate = aTex;
}