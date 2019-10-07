/*

Tiles.h
-Each type of tile inherits from the base class cTile
-There is no longer a bool breakable
   *Instead we just handle what a tile should do if it is punched on a tile by tile basis

-For destroying tiles, we are simply not drawing them instead of replacing them with something like the Sky tile
	*You can either set the bool broken flag, and it won't be drawn, or just have the last frame in the animation be just transparent pixels
	*Either way you also have to set bool solid to false


*/

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
//#include "olcPGEX_Sound.h"
#include "Animator.h"


//==================================================================================================================================================================================================
//																					BASE TILE
//==================================================================================================================================================================================================
class cTile
{
public:
	cTile(std::string tileName, bool solid);

public:
	std::string sName;
	bool solid = true;

	bool broken = false; //Prevents tile from being drawn

	cAnimator animTile;
	

public:
	virtual void DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY);	//--Default behavior defined in .cpp, but can be overridden if need to be drawn differently
	//virtual void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t);			//----ex. Tiles that draw sprites (animated or not) vs tiles that might just be an RGB color using FillRect fcn

	//void tileRotate(float fTheta); --> If want to do stuff like this with manipulating the tiles, add a olc::GFX2D::Transform2D t object as a private member of the tile class, that can be manipulated on a per tile basis and then used in DrawTile/DrawSelf
	
	virtual void Update(float fElapsedTime) { animTile.Update(fElapsedTime); };
	virtual void OnPunch() {} //Return whether block goes away when hit/broken (ex. brick - true, Question mark tile - false) //Could not have breakable flag and just have this return true or false if it's breakable. Every tile would implement Break() and it would be called on every tile.
};

//==================================================================================================================================================================================================
//																					FLOOR
//==================================================================================================================================================================================================
class cTile_Floor : public cTile
{
public:
	cTile_Floor();
};

//==================================================================================================================================================================================================
//																					BRICK
//==================================================================================================================================================================================================
class cTile_Brick : public cTile
{
public:
	cTile_Brick();
	void OnPunch() override;
	void Update(float fElapsedTime) override;
};

//==================================================================================================================================================================================================
//																					SKY
//==================================================================================================================================================================================================
class cTile_Sky : public cTile //Essentially acts like a blank tile
{
public:
	cTile_Sky();
};

//==================================================================================================================================================================================================
//																					COLOR TILE
//==================================================================================================================================================================================================
class cTile_Color : public cTile
{
public:
	cTile_Color(olc::Pixel color);
	virtual void DrawSelf(olc::PixelGameEngine* pge, float screenPosX, float screenPosY) override;
private:
	olc::Pixel tileColor;
};

//==================================================================================================================================================================================================
//																					INVISIBLE BOUNDARY
//==================================================================================================================================================================================================
class cTile_Invisible_Boundary : public cTile //Returned by GetTile function if accessing something outside of map boundaries - Makes objects not able to fall out of world
{
public:
	cTile_Invisible_Boundary();
};