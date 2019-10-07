#pragma once

#include "Dynamics.h"
#include <list>

//Forwardly declare the game engine itself
//Tells compiler that this symbol exists before it reaches
//the line declaring static g_engine.
//But it doesn't really exist because of this, so we need to give it
//some kind of implementation in the cpp file
class Platformer;

class cCommand
{
public:
	cCommand() {}
	virtual ~cCommand() {}

	bool bStarted = false;
	bool bCompleted = false;

	virtual void Start() {}
	virtual void Update(float fElapsedTime) {}

	static Platformer* g_engine;
};

class cScriptProcessor
{
public:
	cScriptProcessor();

public:
	void AddCommand(cCommand* cmd);
	void ProcessCommands(float fElapsedTime);
	void CompleteCommand();

public:
	bool bPlayerControlEnabled;

private:
	std::list<cCommand*> listCommands;
};


/*+========================================================+
  +=================   Derived Commands   =================+
  +========================================================+ */

class cCommand_MoveTo : public cCommand
{
public:
	cCommand_MoveTo(cDynamic* object, float x, float y, float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	cDynamic* m_pObject;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	float m_fDuration;
	float m_fTimeSoFar;
};

class cCommand_ShowDialog : public cCommand
{
public:
	cCommand_ShowDialog(std::vector<std::string> lines, olc::Pixel color = olc::DARK_BLUE);
	void Start() override;

private:
	std::vector<std::string> vecLines;
	olc::Pixel dialogColor;
};

class cCommand_ChangeMap : public cCommand
{
public:
	cCommand_ChangeMap(std::string mapName, float mapPosX, float mapPosY);
	void Start() override;

private:
	std::string sMapName;
	float fMapPosX;
	float fMapPosY;
};