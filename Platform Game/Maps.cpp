#include "Maps.h"
#include <fstream>

using namespace std;

cMap::cMap()
{
	nWidth = 0;
	nHeight = 0;
	//tiles = nullptr;
}

cMap::~cMap()
{
	delete[] tiles;
}

cTile* cMap::GetTile(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
	{
		if (tiles[y * nWidth + x] != nullptr)
			return tiles[y * nWidth + x];
		else
			return staticBoundary; //Just in case something goes wrong and a tile is still nullptr
	}
	else
		return staticBoundary;
}

void cMap::SetTile(int x, int y, cTile* t)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		tiles[y * nWidth + x] = t;
}

bool cMap::GetSolid(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return tiles[y * nWidth + x]->solid;
	else
		return true; //Default will be solid
}

bool cMap::GetBreakable(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return tiles[y * nWidth + x]->solid;
	else
		return false; //Default will be non-breakable
}


bool cMap::Create(std::string fileName, std::string name)
{
	sName = name;

	std::ifstream inFile(fileName, std::ios::in | std::ios::binary);  // ---Use this when eventually move to system of just numbers in a file like in video
	//std::ifstream inFile(fileName, std::ios::in);
	if (inFile.is_open())
	{
		int oldWidth = nWidth;
		int oldHeight = nHeight;

		inFile >> nWidth >> nHeight;
		if (nWidth == 0 || nHeight == 0) //If width and height are not specified (defaulted to 0)
		{
			cout << "Ensure Level Width and Level Height are respectively defined in the first 2 lines of the level file." << endl;
			nWidth = oldWidth;
			nHeight = oldHeight;
			return false;
		}


		//File should be good, read
		tiles = new cTile*[nWidth * nHeight];

		for (int x = 0; x < nWidth * nHeight; x++)
		{
			tiles[x] = nullptr; //Initialize array to nullptr's - So if we don't initialize every tile of the level, we can check for that after file read, and also when accessing this array through GetTile
		}

		//Read into a string first using >> so that we don't get whitespace in the way
		string s;
		for (int y = 0; y < nHeight; y++)
		{
			inFile >> s;
			for (int x = 0; x < nWidth; x++)
			{
				cout << s[x]; //TEST,DEBUG print out what it's doing
				switch (s[x])
				{
				case '.':
					tiles[y * nWidth + x] = new cTile_Sky;
					break;
				case 'F':
					tiles[y * nWidth + x] = new cTile_Floor;;
					break;
				case 'B':
					tiles[y * nWidth + x] = new cTile_Brick;
					break;
				case 'o':
					tiles[y* nWidth + x] = new cTile_Brick;
					break;
				case '1':
					tiles[y * nWidth + x] = new cTile_Brick;
					break;
				default:
					tiles[y * nWidth + x] = new cTile_Invisible_Boundary;
					break;
				}
			}
			cout << endl; //TEST, DEBUG
		}

		//Could implement way to add dynamic things like items to the map from here, so we can put them in when designing level, and not have to input specific coords for every coin, for example
		//Dynamically create new Tile, and add to vTiles vector

		//To implement a KV pair/map for each level based on a file, we could use a Factory method for dynamically allocating types of objects. This code would be shorter, as the case statements would be in the factory method




		//Check entire tiles array was filled
		for (int x = 0; x < nWidth * nHeight; x++)
		{
			if (tiles[x] == nullptr)
			{
				cerr << "Error: Not all elements of tiles array for the level are initialized.";
				return false;
			}
		}

		inFile.close();
		return true;
	}
	else
	{
		cout << "Unable to open file." << endl;
		inFile.close();
		return false;
	}
}


cLevel1::cLevel1()
{
	if (Create("../Levels/level_1.txt", "Level 1"))
		cout << "File loaded successfully";
}