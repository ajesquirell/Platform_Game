#include "Tiles.h"

using namespace std;


//==================================================================================================================================================================================================
//																					BASE TILE
//==================================================================================================================================================================================================
cTile::cTile(string tileName, bool solid)
{
	sName = tileName;
	this->solid = solid;
	broken = false;
}

//olc::Sprite* cTile::GetCurrentFrame()
//{
//	return animTile.mapStates[animTile.sCurrentState][animTile.nCurrentFrame];
//}


void cTile::DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY) //screen space coordinates
{
	if (!broken)
	{
		olc::GFX2D::Transform2D t;
		t.Translate(screenPosX, screenPosY);
		animTile.DrawSelf(pge, t);
	}
}

//void cTile::DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
//{
//	animTile.DrawSelf(pge, t);
//}


//==================================================================================================================================================================================================
//																					FLOOR
//==================================================================================================================================================================================================

cTile_Floor::cTile_Floor() : cTile("floor", true)
{
	animTile.mapStates["normal"].push_back(Assets::get().GetSprite("Floor"));
	animTile.ChangeState("normal");
}

//==================================================================================================================================================================================================
//																					BRICK
//==================================================================================================================================================================================================
cTile_Brick::cTile_Brick() : cTile("brick", true)
{
	animTile.mapStates["default"].push_back(Assets::get().GetSprite("Brick"));

	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_01"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_02"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_03"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_04"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_05"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_06"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_07"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_08"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_09"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_10"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_11"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_12"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_13"));
	animTile.mapStates["break"].push_back(Assets::get().GetSprite("Brick_Break_14"));

	animTile.ChangeState("default");
}

void cTile_Brick::Update(float fElapsedTime)
{
	//if (animTile.sCurrentState == "break" && animTile.bCompletedAnimation) //If last sprite in animation is blank... don't need this
	//	broken = true;

	if (animTile.sCurrentState == "break")
		animTile.fTimeBetweenFrames = 0.05f;
	
	animTile.Update(fElapsedTime);
}

void cTile_Brick::OnPunch()
{
	animTile.ChangeState("break", true);
	solid = false;
}

//==================================================================================================================================================================================================
//																					SKY
//==================================================================================================================================================================================================
cTile_Sky::cTile_Sky() : cTile("sky", false)
{
}

//==================================================================================================================================================================================================
//																					COLOR TILE
//==================================================================================================================================================================================================
cTile_Color::cTile_Color(olc::Pixel color) : cTile("color_tile", false)
{
	tileColor = color;
}

void cTile_Color::DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY)
{
	pge->FillRect(screenPosX, screenPosY, 22, 22, tileColor); //22 hard coded in, but is a variable in main game function, but this likely won't change...
}

//==================================================================================================================================================================================================
//																					INVISIBLE BOUNDARY
//==================================================================================================================================================================================================
cTile_Invisible_Boundary::cTile_Invisible_Boundary() : cTile("invisble_wall", true)
{
}


