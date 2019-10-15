#pragma once
#include "olcPixelGameEngine.h"
#include "Tiles.h"
#include "Dynamics.h"
#include "Commands.h"

class cMap
{
public:
	cMap();
	~cMap();

public:

	enum NATURE
	{
		TALK,
		WALK
	};

public:
	static cScriptProcessor* g_script;

	int nWidth; //Of Map
	int nHeight;

	olc::Pixel skyColor = olc::CYAN;
	std::string sName;
	
public:
	cTile* GetTile(int x, int y);
	void SetTile(int x, int y, cTile* t);

	bool Create(std::string fileName, std::string name);

	virtual bool PopulateDynamics(std::vector<cDynamic*> &vecDyns)
	{
		return false;
	}

	virtual bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
	{
		return false;
	}


protected:
	std::vector<cDynamic_Item*> vecPersistentItems; //Items loaded into map at launch, won't respawn every change of map
	//?Reset items to original "loaded-from-file" state function for when we come back to the "world" or whatever and want to replay?

private:
	cTile** tiles = nullptr;	//Going to be an array of pointers to tiles

	cTile_Invisible_Boundary* staticBoundary = new cTile_Invisible_Boundary; //Need to return this to prevent memory leak if accessing out of bounds, b/c before
												//we were returning a new cTile_Invisible_Boundary every time the edge of the screen was
												//being drawn (Probably because we 'lazily' overdraw the screen to prevent artifacts.
												//This was causing memory to rapidly increase whenever the camera was on the edge of the screen
};


class cMap_Level1 : public cMap
{
public:
	cMap_Level1();

	bool PopulateDynamics(std::vector<cDynamic*> &vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature) override;
};

class cMap_Level2 : public cMap
{
public:
	cMap_Level2();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature) override;
};