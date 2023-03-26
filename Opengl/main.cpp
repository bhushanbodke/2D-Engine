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
#include "Engine.h"
#include "io/KeyBoard.h"
#include "io/Mouse.h"




class Game :public Engine
{
public:
    Sprite tree;
    Sprite tree1;
    Sprite tree2;
    Sprite tree3;
    Sprite tree4;
    Sprite tree5;
    
    glm::vec4 c = Color::Red;
    float angle = 0.0f;
  
    Game() {
        
    };
    bool OnUserCreate() override
    {
        return true;
    }
    void CreateAssets() override
    {
        tree = Sprite("tree.png");
    }
    void InputHandling(float dt) override
    {
        if (KeyBoard::key(GLFW_KEY_W))
        {
            angle += 3.5f * dt;
        }
        if (KeyBoard::key(GLFW_KEY_S))
        {
            angle -= 3.5f * dt;
        }
    }
    bool Render() override
    {
        BackGroundColor(0.7f, 0.2f, 0.5f, 1.0f);
        DrawSprite(tree, { 0.0f,0.0f }, { 200.0f,200.0f },0);
        DrawRect({ 100.0f,100.0f }, { 200.0f,200.0f },0, Color::White);
        DrawLine({ 200.0f,200.0f }, 500.0f, angle, { 1.0f,1.0f,1.0f,1.0f });
        SetPixel(500, 500, Color::White);
        return true;
    }

};

int main(void)
{
    Game game;
    game.CreateWindow(960, 600, "bhusah");
    game.Init();
    game.Run();
   
}