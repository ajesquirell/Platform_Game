#include "Assets.h"

using namespace std;

Assets::Assets()
{
}

Assets::~Assets()
{
}

void Assets::LoadSprites() //Single one time load of all sprite resources
{
	auto load = [&](string name, string filename)
	{
		mapSprites[name] = new olc::Sprite(filename);
	};

	load("Floor", "../Sprites/Floor.png");
	load("Brick", "../Sprites/Brick.png");

	//Jerry
	load("Jerry_Idle", "../Sprites/Jerry_Idle.png");

	load("Jerry_Run_1", "../Sprites/Jerry_Run_1.png");
	load("Jerry_Run_2", "../Sprites/Jerry_Run_2.png");
	load("Jerry_Run_3", "../Sprites/Jerry_Run_3.png");
	load("Jerry_Run_4", "../Sprites/Jerry_Run_4.png");

	load("Jerry_Brake_1", "../Sprites/Jerry_Brake_1.png");
	load("Jerry_Brake_2", "../Sprites/Jerry_Brake_2.png");
	load("Jerry_Brake_3", "../Sprites/Jerry_Brake_3.png");
	load("Jerry_Brake_4", "../Sprites/Jerry_Brake_4.png");
	load("Jerry_Brake_5", "../Sprites/Jerry_Brake_5.png");

	load("Jerry_Squat", "../Sprites/Jerry_Squat.png");

	load("Jerry_Jump_1", "../Sprites/Jerry_Jump_1.png");
	load("Jerry_Jump_2", "../Sprites/Jerry_Jump_2.png");

	load("Jerry_Fall", "../Sprites/Jerry_Fall.png");

	//Money

}
