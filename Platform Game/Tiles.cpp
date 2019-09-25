#include "Tiles.h"

using namespace std;

cTile::cTile(string tileName)
{
	sName = tileName;
}

olc::Sprite* cTile::GetCurrentFrame()
{
	return animTile.mapStates[animTile.sCurrentState][animTile.nCurrentFrame];
}


void cTile::DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
{
	animTile.DrawSelf(pge, t);
}
//void cTile::DrawSelf(olc::PixelGameEngine* pge, float sx, float sy) //sx and sy in screen space
//{
//	pge->DrawSprite(sx, sy, cTile::GetCurrentFrame());
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Floor::cTile_Floor() : cTile("floor")
{
	animTile.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Floor.png"));
	animTile.ChangeState("normal");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Brick::cTile_Brick() : cTile("brick")
{
	animTile.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Brick.png"));
	animTile.ChangeState("normal");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cTile_Sky::cTile_Sky() : cTile("sky")
{
}

void cTile_Sky::DrawSelf(olc::PixelGameEngine* pge, float sx, float sy, olc::Pixel skyColor)
{
	pge->FillRect(sx, sy, nTileWidth, nTileHeight, skyColor);
}