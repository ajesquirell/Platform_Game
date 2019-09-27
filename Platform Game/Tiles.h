#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
//#include "olcPGEX_Sound.h"
#include "Animator.h"
#include "Assets.h"

class cTile
{
public:
	cTile(std::string tileName, bool solid, bool breakable);

public:
	std::string sName;
	bool solid = true;     //Make some of these private and use corresponding Getter's?
	bool breakable = false;

	cAnimator animTile;
	

public:
	virtual void DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY);	//--Default behavior defined in .cpp, but can be overridden if need to be drawn differently
	//virtual void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t);			//----ex. Tiles that draw sprites (animated or not) vs tiles that might just be an RGB color using FillRect fcn

	//void tileRotate(float fTheta); --> If want to do stuff like this with manipulating the tiles, add a olc::GFX2D::Transform2D t object as a private member of the tile class, that can be manipulated on a per tile basis and then used in DrawTile/DrawSelf

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

class cTile_Sky : public cTile //Essentially acts like a blank tile
{
public:
	cTile_Sky();
};

class cTile_Color : public cTile
{
public:
	cTile_Color(olc::Pixel color);
	virtual void DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY) override;
private:
	olc::Pixel tileColor;
};

class cTile_Invisible_Boundary : public cTile //Returned by GetTile function if accessing something outside of map boundaries - Makes objects not able to fall out of world
{
public:
	cTile_Invisible_Boundary();
};