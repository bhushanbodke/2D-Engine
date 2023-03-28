#pragma once
#include<iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include<fstream>
#include <iostream>
#include <sstream>
#include "ShaderClass.h"
#include  "Sprite.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw_gl3.h>
#include "io/KeyBoard.h"
#include "io/Mouse.h"
#include <vector>
#include <array>
#include <map>
#include "Sprite.h"

namespace Color {
	const glm::vec4 White(1.0, 1.0, 1.0, 1.0),
		Black(0.0, 0.0, 0.0, 1.0),
		Red(1.0, 0.0, 0.0, 1.0),
		Green(0.0, 1.0, 0.0, 1.0),
		Blue(0.0, 0.0, 1.0, 1.0),
		Yellow(1.0, 1.0, 0.0, 1.0),
		Cyan(0.0, 1.0, 1.0, 1.0),
		Magenta(1.0, 0.0, 1.0, 1.0),
		Orange(1.0, 0.5, 0.0, 1.0),
		Gray(0.5, 0.5, 0.5, 1.0),
		Light_Gray(0.8, 0.8, 0.8, 1.0),
		Dark_Gray(0.2, 0.2, 0.2, 1.0),
		Pink(1.0, 0.75, 0.8, 1.0),
		Purple(0.5, 0.0, 0.5, 1.0),
		Brown(0.6, 0.4, 0.2, 1.0),
		Navy_Blue(0.0, 0.0, 0.5, 1.0),
		Teal(0.0, 0.5, 0.5, 1.0),
		Olive(0.5, 0.5, 0.0, 1.0),
		Maroon(0.5, 0.0, 0.0, 1.0),
		Lime(0.0, 1.0, 0.0, 1.0),
		Aqua(0.0, 1.0, 1.0, 1.0),
		Fuchsia(1.0, 0.0, 1.0, 1.0),
		Silver(0.75, 0.75, 0.75, 1.0),
		Gold(1.0, 0.84, 0.0, 1.0);
};

struct Character {
	uint32_t TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	uint32_t advance;
};

enum mode
{
	texture = 0 ,
	solid = 1,
	text = 2,
	Point =3,
};

class Engine
{
public:
	GLFWwindow* window;
	std::string Title;
	short Src_width;
	short Src_height;
	uint32_t vbo, vao, ebo;
	uint32_t cvbo, cebo, cvao;
	uint32_t pvao, pvbo;
	uint32_t tvao, tvbo;
	int cindicesSize;
	int no_points= 0 ;
	//deafult Shader
	static Shader Default_Tex_Shader;
	static Shader Default_Shader;

	int arrayPos = 0; 
	float * Points;
	int dataSend = 0;
	int dataSet = 0;

	std::map<char, Character> Characters;

public:
	~Engine();
	bool CreateWindow(short width, short height, std::string title = "title",bool CapFps60 = false);
	bool Init();
	void TextInit();
	bool SetBlend(bool blend);
	void Events();
	void ClearScreen();
	void BackGroundColor(glm::vec4 Col);

	static void resizeCallback(GLFWwindow* window, int width, int height);

	//User functions

	virtual bool OnUserCreate();
	virtual bool Render(int fps);
	virtual void InputHandling(float deltatime);
	virtual void CreateAssets();

	void Run();
	void Stop();


	//Draw Functions
	void DrawSprite(Sprite& sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height, glm::vec4 tint = glm::vec4(1.0f), float angle = 0);
	void DrawRect(uint16_t x, uint16_t y, uint16_t width , uint16_t height, glm::vec4 Color = glm::vec4(1.0f), float angle = 0);
	void DrawPixel(int x, int y, glm::vec4 Color = glm::vec4(1.0f));
	void RenderPixels();
	void DrawLine(uint16_t x, uint16_t y, float length, glm::vec4 Color = glm::vec4(1.0f), float angle = 0 );
	void DrawLine(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, glm::vec4 Color);
	void DrawCircle(uint16_t x, uint16_t y, float radius, glm::vec4 Color = glm::vec4(1.0f));
	void RenderText(std::string text, float x, float y, float scale, glm::vec4 Color);
};

