#pragma once
#include "olcPixelGameEngine.h"
#include "Tiles.h"

class cMap
{
public:
	cMap();
	~cMap();

public:
	int nWidth; //Of Map
	int nHeight;

	olc::Pixel skyColor = olc::CYAN;
	std::string sName;
	

	cTile* GetTile(int x, int y);
	void SetTile(int x, int y, cTile* t);
	bool GetSolid(int x, int y);
	bool GetBreakable(int x, int y);

	bool Create(std::string fileName, std::string name);

private:
	cTile** tiles = nullptr;	//Going to be an array of pointers to tiles

	cTile_Invisible_Boundary* staticBoundary = new cTile_Invisible_Boundary; //Need to return this to prevent memory leak if accessing out of bounds, b/c before
												//we were returning a new cTile_Invisible_Boundary every time the edge of the screen was
												//being drawn (Probably because we 'lazily' overdraw the screen to prevent artifacts.
												//This was causing memory to rapidly increase whenever the camera was on the edge of the screen
};


class cLevel1 : public cMap
{
public:
	cLevel1();
};