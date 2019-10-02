#include "Commands.h"

using namespace std;

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

//======================================================
//=====================   Commands   ===================
//======================================================

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
		bCompleted = true;
	}
}