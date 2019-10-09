#pragma once

//#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
//#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
//#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

#include "Assets.h"
#include "Animator.h" //Don't think I need this when finally get rid of all left over stuff
#include "Maps.h"
#include "Dynamics.h"
#include "Commands.h"
#include "Quests.h"
#include "Items.h"

class Platformer : public olc::PixelGameEngine
{
public:
	Platformer();

private:
	//Level Storage
	cMap* pCurrentMap = nullptr;

	//Player Properties
	cDynamic_Creature* m_pPlayer = nullptr;

	vector<cDynamic*> vecDynamics;

	cScriptProcessor m_script;

	list<cQuest*> listQuests;
	list<cItem*> listItems;

	//Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	enum
	{
		MODE_TITLE,
		MODE_LOCAL_MAP,
		MODE_WORLD_MAP,
		MODE_INVENTORY,
		MODE_SHOP
	};

	int nGameMode = MODE_LOCAL_MAP;

	/*==============INVENTORY===============*/
	int8_t nInvSelectX = 0; //NEEDS to be signed (so it can go to -1 in the inventory update fcn)
	int8_t nInvSelectY = 0;
	struct InvSelect
	{
		InvSelect(uint8_t x, uint8_t y, uint8_t a) { this->x = x; this->y = y, alpha = a; }
		uint8_t x;
		uint8_t y;
		uint8_t alpha;
	};
	std::vector<InvSelect*> vecInvSelect;

	olc::Pixel inventoryColor;
	olc::Pixel rectColor;

	float fInvStateTick; //For inventory background fading
	/*======================================*/

	olc::Sprite* backBuff; //Save state of current frame for background of transparent inventory screen

	//Sprite Resources
	olc::Sprite* spriteFloor = nullptr;
	olc::Sprite* spriteBrick = nullptr;
	olc::Sprite* spriteTEST = nullptr;

	//Sprite selection flags
	//int nObjectWidth = 22; //Maybe make these global constants in the future in case we want to change resolution, or do other things?
	//int nObjectHeight = 22;

	//Sprite Animation Class
	//cAnimator animPlayer;
	cAnimator animMoney;

	//Pickups
#define COIN L'o'
#define TEST L'1'

	//Pickup variables
	//bool bPickupCollected = false;
	int nPlayerScore = 0;
	string sScoreString;

	//Sound
	int sndSampleA;
	int sndSampleB;
	int sndSampleC;
	int sndBoo;
	int sndWuWuWu;
	int sndGetMoney;

	bool bGamePaused = false;

	//Debug + Testing
	bool showDebug = true;
	


public:
	bool HandlePickup(wchar_t c); //Function for handling the different pickups without jumbling up the game loop with code for every single pickup

	bool OnUserCreate() override;
	bool OnUserDestroy();
	bool OnUserUpdate(float fElapsedTime) override;

	//bool UpdateTitleScreen(float fElapsedTime);
	bool UpdateLocalMap(float fElapsedTime);
	//bool UpdateWorldMap(float fElapsedTime);
	bool UpdateInventory(float fElapsedTime);
	//bool UpdateShop(float fElapsedTime);

	void ShowDialog(vector<string> vecLines, olc::Pixel color = olc::DARK_BLUE);
	void DisplayDialog(vector<string> vecLines, int x, int y);
	void ChangeMap(string sMapName, float x, float y);
	void AddQuest(cQuest* quest);

	//Inventory handling convenience
	bool GiveItem(cItem* item);
	bool TakeItem(cItem* item);
	bool HasItem(cItem* item);

public:
	vector<string> vecDialogToShow;
	bool bShowDialog = false;
	float fDialogX = 0.0f;
	float fDialogY = 0.0f;
	olc::Pixel dialogColor = olc::DARK_BLUE;
};
