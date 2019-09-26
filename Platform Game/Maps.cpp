#include "Maps.h"
#include <fstream>

using namespace std;

cMap::cMap()
{
	nWidth = 0;
	nHeight = 0;
	tiles = nullptr;
}

cMap::~cMap()
{
	delete[] tiles;
}

cTile* cMap::GetTile(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return tiles[y * nWidth + x];
	else
		return nullptr;
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
		tiles = new cTile*[(uint8_t)nWidth * nHeight];

		char buffer[1];
		int x = 0;
		/*for (int x = 0; x < nWidth * nHeight; x++)
		//while(!inFile.eof())
		{
			//if(inFile.peek() != '\n' || '\r')
				inFile.read(buffer, 1);
			switch (*buffer)
			{
			case L'.':
				tiles[x] = new cTile_Sky;
				break;
			case L'F':
				tiles[x] = new cTile_Floor;;
				break;
			case L'B':
				tiles[x] = new cTile_Brick;
				break;

				//Could implement way to add dynamic things like items to the map from here, so we can put them in when designing level, and not have to input specific coords for every coin, for example
				//Dynamically create new Tile, and add to vTiles vector

				//To implement a KV pair/map for each level based on a file, we could use a Factory method for dynamically allocating types of objects. This code would be shorter, as the case statements would be in the factory method
			}
			//x++;
		}*/

		//Loop for testing
		for (int x = 0; x < nHeight; x++)
		{
			for (int y = 0; y < nWidth; y++)
			{
				tiles[y * nWidth + x] = new cTile_Sky;
			}
		}

		return true;
	}
	else
	{
		cout << "Unable to open file." << endl;
		return false;
	}
}


cLevel1::cLevel1()
{
	Create("../Levels/level_1.txt", "Level 1");
}
