#version 330 core
out vec4 FragColor;

// input 

in vec2 textCoordinate;
in vec4 fragcolor;


uniform sampler2D m_Texture;
uniform vec4 Color;
uniform int text;


void main()
{
    if(text == 0){
        vec4 textCol = texture(m_Texture, textCoordinate);
        FragColor = textCol * Color;
    }
    else if(text == 1)
    {
        FragColor = Color;
    }
    else if(text == 2)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(m_Texture, textCoordinate).r);
        FragColor = Color * sampled;
    }
    else if(text ==3)
    {
        FragColor = fragcolor;
    }
}

