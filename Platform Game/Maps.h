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
	std::vector<cTile*> vTiles;
	

	cTile* GetTile(int x, int y);
	void SetTile(int x, int y, cTile* t);

	//To be deleted...
	int GetTileIndex(int x, int y);
	bool GetSolid(int x, int y);
	bool GetBreakable(int x, int y);
	////////////////////////
	bool Create(std::string fileName, std::string name);

private:
	char* tileKey = nullptr; //Array of char's which will be the key in a KV pair to determine which cTile to draw
	int* indicies = nullptr; //When implement tiles class, this will be an array of tile names or ID's
	bool* solids = nullptr;
	bool* breakables = nullptr;

	void LoadSpriteVector();
};


class cLevel1 : public cMap
{
public:
	cLevel1();
};