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
//											Main/Base Quest
//================================================================================================
bool cQuest_MainQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName)
{
	if (sMapName == "Level 1")
	{
		if (nPhase == 0)
		{
			cDynamic_Creature* bob = new cDynamic_Creature("Bob");
			bob->px = 25.0f;
			bob->py = 0.0f;
			bob->fFaceDir = -1.0f;
			vecDyns.push_back(bob);
		}
			cDynamic_Creature* nasser = new cDynamic_Creature("Nasser");
			nasser->px = 20.0f;
			nasser->py = 0.0f;
			nasser->fFaceDir = -1.0f;
			vecDyns.push_back(nasser);
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

bool cQuest_MainQuest::OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	if (target->sName == "Bob")
	{
		if (nPhase == 0)
		{
			CMD(ShowDialog({ "[Bob]", "Hello!", "I have a quest", "for you!" }));
			CMD(ShowDialog({ "You will need to", "speak to me through", "the teleporter" }));
			CMD(ShowDialog({ "Also,", "talk to Nasser to see", "his dialog has changed." }));
			CMD(AddQuest(new cQuest_BobsQuest));
			nPhase = 1;
			return true;
		}
		/*if (nPhase == 1)
		{
			CMD(ShowDialog({ "Hello!", "Thank you for doing", "my quest!" }));
			bCompleted = true;
			return true;
		}*/
	}

	if (target->sName == "Nasser")
	{
		CMD(ShowDialog({ "[Nasser]", "You have no additional", "quests" }));
		return true;
	}

	return false;
}


//================================================================================================
//											Bob's Quest
//================================================================================================
bool cQuest_BobsQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMapName)
{
	return true;
}

bool cQuest_BobsQuest::OnInteraction(std::vector<cDynamic*>& vecDyns, cDynamic* target, NATURE nature)
{
	if (target->sName == "Nasser")
	{
		CMD(ShowDialog({ "[Nasser]", "You are doing Bob's Quest" }));
		return true;
	}

	if (target->sName == "Bob")
	{
		CMD(ShowDialog({ "Hello!", "Thank you for doing", "my quest!" }));
		bCompleted = true;
		return true;
	}
	
	return false;
}
