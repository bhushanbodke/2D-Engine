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
    Sprite left;
    Sprite up;
    Sprite right;
    Sprite down;
    Sprite tree;
    int is_up = 0;
    glm::vec2 player_pos = { 100,515 };
public:
    Game() {
    };
    bool OnUserCreate() override
    {
        return true;
    }
    void CreateAssets() override
    {
        left = Sprite("sprites/character/left.png");
        up = Sprite("sprites/character/up.png");
        right = Sprite("sprites/character/right.png");
        down = Sprite("sprites/character/down.png");
        tree = Sprite("sprites/tree.png");
    }
    void InputHandling(float dt) override
    {
        if (KeyBoard::key(GLFW_KEY_W))
        {
            is_up = 1;
            player_pos.y--;
        }
        if (KeyBoard::key(GLFW_KEY_A))
        {
            is_up = 0;
            player_pos.x--;
        }
        if (KeyBoard::key(GLFW_KEY_D))
        {
            is_up = 2;
            player_pos.x++;
        }
        if (KeyBoard::key(GLFW_KEY_S))
        {
            is_up = 3;
            player_pos.y++;

        }
    }
    bool Render(int fps) override
    {
        BackGroundColor({0.5f,0.5f,0.0f,1.0f});
        DrawSprite(tree, 300, 400, 100, 120);
        DrawSprite(tree, 400, 400, 100, 120);
        DrawSprite(tree, 350, 400, 100, 120);
        DrawSprite(tree, 500, 400, 100, 120);
        DrawSprite(tree, 200, 400, 100, 120);
        DrawSprite(tree, 100, 400, 100, 120);
        DrawSprite(tree, 250, 400, 100, 120);
        DrawSprite(tree, 550, 400, 100, 120);
        if(is_up == 0 )
            DrawSprite(left, player_pos.x, player_pos.y, 100, 100);
        else if (is_up == 1 )
            DrawSprite(up, player_pos.x, player_pos.y, 100, 100);
        else if (is_up == 2)
            DrawSprite(right, player_pos.x, player_pos.y, 100, 100);
        else if (is_up == 3)
            DrawSprite(down, player_pos.x, player_pos.y, 100, 100);
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