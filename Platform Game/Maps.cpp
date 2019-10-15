#include "Maps.h"
#include "Assets.h"
#include <fstream>
using namespace std;

cScriptProcessor* cMap::g_script = nullptr;

#define CMD(n) g_script->AddCommand(new cCommand_ ## n)

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
					tiles[y * nWidth + x] = new cTile_Sky;
					vecPersistentItems.push_back(new cDynamic_Item(x, y, Assets::get().GetItem("Flames Cash")));
					break;
				case '1':

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


cMap_Level1::cMap_Level1()
{
	if (Create("../Levels/Level_1.txt", "Level 1"))
		cout << "File loaded successfully\n";
}

bool cMap_Level1::PopulateDynamics(vector<cDynamic*>& vecDyns)
{
	//Add Teleporters
	vecDyns.push_back(new cDynamic_Teleport(7.0f, 9.0f, "Level 2", 0.0f, 0.0f));

	//Add Items
	vecDyns.push_back(new cDynamic_Item(1, 9, Assets::get().GetItem("Small Health")));
	vecDyns.push_back(new cDynamic_Item(2, 9, Assets::get().GetItem("Health Boost")));


	//Add Items stored in level
	for (auto item : vecPersistentItems)
		vecDyns.push_back(item);

	for (int i = 0; i < 3; i++)
	{
		cDynamic* g = new cDynamic_Creature_FakeJerry();
		vecDyns.push_back(g);
		g->px = rand() % 10 + 5.0f;
		g->py = 0.0f;
	}

	return true;
}

bool cMap_Level1::OnInteraction(vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport" && nature == cMap::WALK)
	{
		// I imagine casting is okay because we are ensuring that only teleport object make it to these lines
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}

	return false;
}



cMap_Level2::cMap_Level2()
{
	if (Create("../Levels/Level_2.txt", "Level 2"))
		cout << "File loaded successfully\n";

	skyColor = olc::DARK_RED;
}

bool cMap_Level2::PopulateDynamics(vector<cDynamic*>& vecDyns)
{
	//Add Teleporters
	vecDyns.push_back(new cDynamic_Teleport(10.0f, 5.0f, "Level 1", 0.0f, 0.0f));

	return true;
}

bool cMap_Level2::OnInteraction(vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport" && nature == cMap::WALK)
	{
		// I imagine casting is okay because we are ensuring that only teleport object make it to these lines
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}

	if (target->sName == "Bob" && nature == cMap::TALK)
	{
		if (vecDyns[0]->px < target->px) //[0] is supposed to be always the player
			((cDynamic_Creature*)target)->fFaceDir = -1.0f;
		else
			((cDynamic_Creature*)target)->fFaceDir = 1.0f;

		//Temporarily becoming not solid vs anything during move command

		//Interaction 1
		if (target->px != 14)
		{
			CMD(ShowDialog({ "[Bob]", "", "Hello!", "I'm Bob!" }));
			CMD(ShowDialog({ "[Bob]","", "Follow me!!!" }, olc::RED));
			CMD(MoveTo(target, 14, 2, 1.5f));
		}

		//Interaction 2
		if (target->px == 14)
		{
			CMD(ShowDialog({ "[Bob]", "", "Down here!" }));
			CMD(MoveTo(target, 16, 2, 0.5f));
		}

		
	}
	return false;
}