#include "Quests.h"
#include "Jerry_Engine.h"

cScriptProcessor* cQuest::g_script = nullptr;
Platformer* cQuest::g_engine = nullptr;

#define CMD(n) g_script->AddCommand(new cCommand_ ## n)


cQuest::cQuest()
{
}

bool cQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName)
{
	return true;
}

bool cQuest::OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	return true;
}

//================================================================================================
//											Test Quest
//================================================================================================
bool cQuest_TestQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName)
{
	if (sMapName == "Level 1")
	{
		if (nPhase == 0)
		{
			cDynamic_Creature* bob = new cDynamic_Creature("Bob");
			bob->px = 20.0f;
			bob->py = 0.0f;
			bob->fFaceDir = -1.0f;
			vecDyns.push_back(bob);
		}
	}

	if (sMapName == "Level 2")
	{
		if (nPhase == 1)
		{
			cDynamic_Creature* bob = new cDynamic_Creature("Bob");
			bob->px = 5.0f;
			bob->py = 0.0f;
			bob->fFaceDir = -1.0f;
			vecDyns.push_back(bob);
		}
	}
	
	return true;
}

bool cQuest_TestQuest::OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	if (target->sName == "Bob")
	{
		if (nPhase == 0)
		{
			CMD(ShowDialog({ "Hello!", "You are in phase 0", "of this quest" }));
			CMD(ShowDialog({ "You will need to", "speak to me through", "the teleporter" }));
			nPhase = 1;
			return true;
		}
		if (nPhase == 1)
		{
			CMD(ShowDialog({ "Hello!", "You are in phase 1", "of this quest" }));
			bCompleted = true;
			return true;
		}
	}

	return false;
}