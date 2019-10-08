#pragma once
#include "Dynamics.h"
#include "Commands.h"

class Platformer;

class cQuest
{
public:
	enum NATURE
	{
		TALK = 0,
		ATTACK = 1,
		KILL = 2,
		WALK = 3
	};

public:
	cQuest();

public:
	virtual bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName);
	virtual bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature);

public:
	std::string sName;
	bool bCompleted = false;
	static cScriptProcessor* g_script;
	static Platformer* g_engine;
};


//================================================================================================
//											Main/Base Quest
//================================================================================================
class cQuest_MainQuest : public cQuest
{
public:
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature) override;

private:
	int nPhase = 0;
};

//================================================================================================
//											Bob's Quest
//================================================================================================
class cQuest_BobsQuest : public cQuest
{
public:
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature) override;

private:
	int nPhase = 0;
};