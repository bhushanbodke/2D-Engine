#include "io/Mouse.h"
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



class Game :public Engine
{
public:
    Sprite tree;
    std::vector<glm::vec2> Points;
    int index = 0;
  
    Game() {
        
    };
    bool OnUserCreate() override
    {
        return true;
    }
    void CreateAssets() override
    {
        tree = Sprite("tree.png");
        Points.resize(1000);
    }
    void InputHandling(float dt) override
    {
        if (Mouse::button(GLFW_MOUSE_BUTTON_1))
        {
            int x = Mouse::GetMouseX();
            int y = Mouse::GetMouseY();
            if (Mouse::GetDX != 0 && Mouse::GetDY() != 0)
            {
                Points[index] = glm::vec2(x, y);
                index++;
            }
        }
    }
    bool Render(int fps) override
    {
        BackGroundColor(Color::Navy_Blue);
        std::cout<<Points.size()<<std::endl;
        uint16_t x1 = 0;
        uint16_t y1 = 0;
        for (auto& point : Points)
        { 
            if (x1 == 0)
                DrawPixel(point.x, point.y, Color::White);
            else
                DrawLine( x1, y1, point.x, point.y, Color::White);

            x1 = point.x;
            y1 = point.y;
        }
        //DrawLine(100, 100, 200, 200, Color::Yellow);
        return true;
    }

};

int main(void)
{
    
    Game game;
    game.CreateWindow(960, 600, "bhushan");
    game.Init();
    game.Run();
   
}