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
#include "Sprite.h"

//struct Vertex
//{
//	glm::vec3 Pos;
//	glm::vec2 TexCoord;
//};

class Engine
{
public:
	GLFWwindow* window;
	std::string Title;
	short Src_width;
	short Src_height;
	GLuint vbo, vao, ebo;
	//deafult Shader
	static Shader Default_Tex_Shader;
	static Shader Default_Shader;
	//std::array<Vertex, 1000> vertices;
	//std::vector<Sprite*> Cleanup;

	int indexcount = 0;
	//Vertex* v;

public:
	~Engine();
	bool CreateWindow(short width, short height, std::string title = "title",bool CapFps60 = false);
	//Sprite* CreateSprite(std::string, glm::vec2 Pos, glm::vec2 size);
	//Vertex* GenQuad(Vertex* v, int x, int y, glm::vec2 size);
	bool Init();
	bool SetBlend(bool blend);
	void Events();
	void ClearScreen();
	void BackGroundColor(float r , float g , float b , float a);

	static void resizeCallback(GLFWwindow* window, int width, int height);

	//User functions

	virtual bool OnUserCreate();
	virtual bool Render();
	virtual void InputHandling(float deltatime);
	virtual void CreateAssets();

	void Run();
	void Stop();


	//Draw Functions
	void DrawSprite(Sprite& sprite, glm::vec2 Pos, glm::vec2 Size, glm::vec2 axis, float angle = 0 , glm::vec4 tint = glm::vec4(1.0f));
	void DrawRect(glm::vec2 Pos, glm::vec2 Size, glm::vec2 axis, float angle = 0, glm::vec4 Color = glm::vec4(1.0f));
};

