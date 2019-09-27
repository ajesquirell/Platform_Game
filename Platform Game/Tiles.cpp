#include "Tiles.h"

using namespace std;

cTile::cTile(string tileName, bool solid, bool breakable)
{
	sName = tileName;
	this->solid = solid;
	this->breakable = breakable;
}

//olc::Sprite* cTile::GetCurrentFrame()
//{
//	return animTile.mapStates[animTile.sCurrentState][animTile.nCurrentFrame];
//}


void cTile::DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY) //screen space coordinates
{
	olc::GFX2D::Transform2D t;
	t.Translate(screenPosX, screenPosY);
	animTile.DrawSelf(pge, t);
}

//void cTile::DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
//{
//	animTile.DrawSelf(pge, t);
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Floor::cTile_Floor() : cTile("floor", true, false)
{
	animTile.mapStates["normal"].push_back(Assets::get().GetSprite("Floor"));
	animTile.ChangeState("normal");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Brick::cTile_Brick() : cTile("brick", true, true)
{
	animTile.mapStates["normal"].push_back(Assets::get().GetSprite("Brick"));
	animTile.ChangeState("normal");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Sky::cTile_Sky() : cTile("sky", false, false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Color::cTile_Color(olc::Pixel color) : cTile("color_tile", false, false)
{
	tileColor = color;
}

void cTile_Color::DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY)
{
	pge->FillRect(screenPosX, screenPosY, 22, 22, tileColor); //22 hard coded in, but is a variable in main game function, but this likely won't change...
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Invisible_Boundary::cTile_Invisible_Boundary() : cTile("invisble_wall", true, false)
{
}


