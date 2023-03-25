#define OLC_PGE_APPLICATION
#include "GameEngine.h"
#include <string>

class Game :public olc::PixelGameEngine
{
public:
	std::string sAppname;
	olc::Sprite* sprite = nullptr;
	olc::Decal* decal = nullptr;

public:
	Game()
	{
		sAppname = "decal";
	}

	bool OnUserCreate() override
	{
		sprite = new olc::Sprite("tree.png");
		decal = new olc::Decal(sprite);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);
		olc::vf2d mouse = { float((GetMouseX())),float((GetMouseY())) };
		DrawDecal(mouse, decal, {0.2f,0.2f});
		return true;
	}

};

int main() {
	Game game;
	if (game.Construct(640, 400, 2, 2))
		game.Start();
}