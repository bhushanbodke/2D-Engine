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
    glm::vec2 a = glm::vec2(100.0f, 100.0f);
    float angle = 0.0f;
    int Speed = 5.0f;

  
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
            a.y += 10 *dt ;
        }
        if (KeyBoard::key(GLFW_KEY_S))
        {
            a.y -= 10 * dt ;

        }
        if (Mouse::button(GLFW_MOUSE_BUTTON_1))
        {
            std::cout << Mouse::GetMouseX() << "||" << Mouse::GetMouseY()<<std::endl;
        }
    }
    bool Render() override
    {
        BackGroundColor(Color::Black);
        for (int i = 0; i < 960; i++)
        {
            for (int j = 0; j < 600; j++)
            {
                SetPixel(i ,j , Color::Red);
            }
        }
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