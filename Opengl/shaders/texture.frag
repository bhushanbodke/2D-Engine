#version 330 core
out vec4 FragColor;

// input 

in vec2 textCoordinate;

uniform sampler2D m_Texture;
uniform vec4 Color;


void main()
{
    vec4 textCol = texture(m_Texture, textCoordinate);
    FragColor = textCol * Color; 
}