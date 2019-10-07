#include "Commands.h"
#include "Jerry_Engine.h"
#include "Quests.h"

using namespace std;

//This still just shuts compiler up, but it won't for long because
//it still needs a body, so we include the game engine file above.
//Otherwise, compiler says "incomplete class type not allowed"
Platformer* cCommand::g_engine = nullptr;


cScriptProcessor::cScriptProcessor()
{
	bPlayerControlEnabled = true;
}

void cScriptProcessor::AddCommand(cCommand* cmd)
{
	listCommands.push_back(cmd);
}

void cScriptProcessor::ProcessCommands(float fElapsedTime)
{
	// If command is available, halt user input
	bPlayerControlEnabled = listCommands.empty();

	if (!listCommands.empty())
	{
		// A command is available
		if (!listCommands.front()->bCompleted)
		{
			if (!listCommands.front()->bStarted)
			{
				listCommands.front()->Start(); // Event - only occurs once
				listCommands.front()->bStarted = true;
			}
			else // Command has been started, so process it
				listCommands.front()->Update(fElapsedTime);
		}
		else
		{
			// Command has been completed
			delete listCommands.front();
			listCommands.pop_front();
		}
	}
}

// Sets currently active command as complete, from external source
void cScriptProcessor::CompleteCommand()
{
	if (!listCommands.empty())
	{
		listCommands.front()->bCompleted = true;
	}
}

//======================================================
//=====================   Commands   ===================
//======================================================

/*--------------------   Move To   -------------------*/

cCommand_MoveTo::cCommand_MoveTo(cDynamic* object, float x, float y, float duration)
{
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = max(duration, 0.001f);
	m_pObject = object;
}
//cCommand_MoveTo::cCommand_MoveTo --- Alternate constructor with single vel parameter instead of duration

void cCommand_MoveTo::Start()
{
	m_fStartPosX = m_pObject->px;
	m_fStartPosY = m_pObject->py;

	m_pObject->bSolidVsDynamic = false; // HACK IN - So things don't push everything else around
	m_pObject->bSolidVsMap = false;
}

void cCommand_MoveTo::Update(float fElapsedTime) //Could eventually do path planning using solid tiles! A* ?
{
	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration; //Normalized time value
	if (t > 1.0f) t = 1.0f;

	m_pObject->px = (m_fTargetPosX - m_fStartPosX) * t + m_fStartPosX;
	m_pObject->py = (m_fTargetPosY - m_fStartPosY) * t + m_fStartPosY;
	m_pObject->vx = (m_fTargetPosX - m_fStartPosX) / m_fDuration;
	m_pObject->vy = (m_fTargetPosY - m_fStartPosY) / m_fDuration; // vx,vy simply for animation sake I guess


	if (m_fTimeSoFar >= m_fDuration)
	{
		// Object has reached destination, so stop
		m_pObject->px = m_fTargetPosX;
		m_pObject->py = m_fTargetPosY;
		m_pObject->vx = 0.0f;
		m_pObject->vy = 0.0f;

		m_pObject->bSolidVsDynamic = true; // Restore to true - assuming that it was true to start with. Should restore to actual value but this is quick hack for now
		m_pObject->bSolidVsMap = true;

		bCompleted = true;
	}
}

/*--------------------   Show Dialog   -------------------*/
cCommand_ShowDialog::cCommand_ShowDialog(vector<string> lines, olc::Pixel color)
{
	vecLines = lines;
	dialogColor = color;
}

void cCommand_ShowDialog::Start() 
{
	//Can probably do all this without the static g_engine pointer,
	//since we don't use the DrawBigText function,
	//and instead use the pge function DrawString, by passing in pge.
	//This is the only spaghetti code, but we could probably not to this at all

	// Need communication with game engine, since it is responsible for drawing to screen
	g_engine->ShowDialog(vecLines, dialogColor);
}

/*--------------------   Change Map   -------------------*/
cCommand_ChangeMap::cCommand_ChangeMap(std::string mapName, float mapPosX, float mapPosY)
{
	sMapName = mapName;
	fMapPosX = mapPosX;
	fMapPosY = mapPosY;
}

void cCommand_ChangeMap::Start()
{
	g_engine->ChangeMap(sMapName, fMapPosX, fMapPosY);
	bCompleted = true;
}

/*--------------------   Add Quest   -------------------*/
cCommand_AddQuest::cCommand_AddQuest(cQuest* quest)
{
	this->quest = quest;
}

void cCommand_AddQuest::Start()
{
	g_engine->AddQuest(quest);
	bCompleted = true;
}