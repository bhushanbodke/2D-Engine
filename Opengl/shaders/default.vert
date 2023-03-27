#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec4 color;
//out put 
out vec2 textCoordinate;
out vec4 fragcolor;

uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position =view*model*vec4(position,1.0f);
    textCoordinate = aTex;
    fragcolor = color;
}