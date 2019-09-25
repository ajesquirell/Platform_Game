#include "Maps.h"
#include <fstream>

using namespace std;

cMap::cMap()
{
	nWidth = 0;
	nHeight = 0;
	solids = nullptr;
	indicies = nullptr;
	breakables = nullptr;
}

cMap::~cMap()
{
	delete[] solids;
	delete[] indicies;
	delete[] breakables;
}

cTile* cMap::GetTile(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return vTiles[y * nWidth + x];
	else
		return nullptr;
}

void cMap::SetTile(int x, int y, cTile* t)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		vTiles[y * nWidth + x] = t;
}

//To be deleted...
int cMap::GetTileIndex(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return indicies[y * nWidth + x];
	else
		return 0;
}

bool cMap::GetSolid(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return vTiles[y * nWidth + x]->solid;
	else
		return true;
}
//////////////////////////////////////

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
		indicies = new int[nWidth * nHeight];
		solids = new bool[nWidth * nHeight];
		breakables = new bool[nWidth * nHeight];

		char buffer[1];
		int x = 0;
		//for (int x = 0; x < nWidth * nHeight; x++)
		while(!inFile.eof())
		{
			//if(inFile.peek() != '\n' || '\r')
				inFile.read(buffer, 1);
			switch (*buffer)
			{
			case L'.':
				indicies[x] = 0;
				solids[x] = 0;
				breakables[x] = 0;
				vTiles.push_back(new cTile_Sky);
				break;
			case L'F':
				indicies[x] = 1;
				solids[x] = 1;
				breakables[x] = 0;
				vTiles.push_back(new cTile_Floor);
				break;
			case L'B':
				indicies[x] = 2;
				solids[x] = 1;
				breakables[x] = 1;
				vTiles.push_back(new cTile_Brick);
				break;

				//Could implement way to add dynamic things like items to the map from here, so we can put them in when designing level, and not have to input specific coords for every coin, for example
				//Dynamically create new Tile, and add to vTiles vector
			}
			x++;
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
