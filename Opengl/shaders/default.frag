#version 330 core

out vec4 FragColor;
in vec4 fragcolor;

uniform vec4 Color;

void main()
{
	if(Color == vec4(0.0,0.0,0.0,0.0))
	{
		FragColor = fragcolor;
	}
	else{
		FragColor = Color;
	}
}


