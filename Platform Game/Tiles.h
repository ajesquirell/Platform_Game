#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
//#include "olcPGEX_Sound.h"
#include "Animator.h"

class cTile
{
public:
	cTile(std::string tileName);
	cTile();

public:
	std::string sName;
	bool solid = false;     //Make some of these private?
	bool breakable = false;

	cAnimator animTile;
	//bool modified = false;
	int nTileHeight = 22;
	int nTileWidth = 22;
	
public:
	olc::Sprite* GetCurrentFrame();
	virtual void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t); //If modified and want to draw a modified sprite (rotated, scaled, etc) set the modified flag
	//virtual void DrawSelf(olc::PixelGameEngine* pge, float sx, float sy);

};

class cTile_Floor : public cTile
{
public:
	cTile_Floor();
};

class cTile_Brick : public cTile
{
public:
	cTile_Brick();
};

class cTile_Sky : public cTile
{
public:
	cTile_Sky();
	void DrawSelf(olc::PixelGameEngine* pge, float sx, float sy, olc::Pixel skyColor);
};