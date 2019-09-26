#pragma once
#include "olcPixelGameEngine.h"
#include "Tiles.h"

class cMap
{
public:
	cMap();
	~cMap();

public:
	int nWidth;
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
};


class cLevel1 : public cMap
{
public:
	cLevel1();
};