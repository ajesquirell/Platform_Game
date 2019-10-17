#include "Jerry_Engine.h"

#define CMD(n) m_script.AddCommand(new cCommand_ ## n)

Platformer::Platformer()
{
	sAppName = "LU Platformer";
}


bool Platformer::HandlePickup(wchar_t c) //Function for handling the different pickups without jumbling up the game loop with code for every single pickup
{
	bool success = false; //In case we add a pickup and don't implement it here, it will return false.
	switch (c)			//Oh and this will be the logic to check for pickups, if it's not meant to be picked up and not implemented here, nothing will happen
	{					//That way we don't need to have a long if statement in OnUserUpdate() cluttered with every possible pickup we add to the game
	case (COIN):
		m_pPlayer->nScore += 10;
		olc::SOUND::PlaySample(Assets::get().GetSound("sndSampleB")); // Plays Sample B
		success = true;
		break;

	case (TEST):
		//skyColor = olc::DARK_YELLOW;
		olc::SOUND::PlaySample(Assets::get().GetSound("sndWuWuWu"));
		success = true;
		break;
	}

	return success;
}

bool Platformer::OnUserCreate()
{
	//Only instance of spaghetti code
	//But allows us to easily access all the
	//public utilities from this file - our game engine
	cCommand::g_engine = this;
	cQuest::g_engine = this;
	cDynamic::g_engine = this;

	cMap::g_script = &m_script;
	cQuest::g_script = &m_script;
	cDynamic::g_script = &m_script;

	//Initialize "back buffer" - really just for inventory background
	backBuff = new olc::Sprite(ScreenWidth(), ScreenHeight());

	//Inventory background color
	inventoryColor = olc::BLACK;
	rectColor = olc::WHITE;

	//Load Assets (Sprites, Maps)
	Assets::get().LoadSprites(); //Can get away with loading everything at once because this is a small game
	Assets::get().LoadItems();
	Assets::get().LoadMaps();
	Assets::get().LoadSounds();

	//Animated

		//Money
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_00"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_01"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_02"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_03"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_04"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_05"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_06"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_07"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_08"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_09"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_10"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_11"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_12"));
	animMoney.mapStates["normal"].push_back(Assets::get().GetSprite("Money_13"));

	//Set initial animated states
	animMoney.ChangeState("normal");

	SetPixelMode(olc::Pixel::MASK); //Allow Transparency


	//Sound
	olc::SOUND::InitialiseAudio(44100, 1, 8, 512);

	

	olc::SOUND::PlaySample(Assets::get().GetSound("LitLoop"), true); // Plays Sample C loop

	//Add First Quest
	listQuests.push_front(new cQuest_MainQuest());

	//Player init
	m_pPlayer = new cDynamic_Creature_Jerry(); //For now sprites/ anims are hard coded to be Jerry

	//Initial Map
	ChangeMap("Level 1", 0, 0);

	return true;
}

bool Platformer::OnUserDestroy()
{
	olc::SOUND::DestroyAudio();
	return true;
}

bool Platformer::OnUserUpdate(float fElapsedTime)
{
	//Check for game pause
	if (GetKey(olc::Key::P).bPressed)
	{
		if (!bGamePaused) {
			bGamePaused = true; /*Pause*/
			olc::SOUND::StopSample(Assets::get().GetSound("LitLoop"));
			olc::SOUND::PlaySample(Assets::get().GetSound("sndGetMoney"));
		}
		else {
			bGamePaused = false; /*Unpause*/
			olc::SOUND::PlaySample(Assets::get().GetSound("LitLoop"), true);
		}
	}
	if (bGamePaused)
	{
		DrawString(ScreenWidth() / 2.5f, ScreenHeight() / 2.5f, "Paused");
		return true; //Game loop won't execute
	}

	//Check Game Mode
	switch (nGameMode)
	{
	//case MODE_TITLE:
		//return UpdateTitleScreen(fElapsedTime);
	case MODE_LOCAL_MAP:
		return UpdateLocalMap(fElapsedTime);
	//case MODE_WORLD_MAP:
		//return UpdateWorldMap(fElapsedTime);
	case MODE_INVENTORY:
		return UpdateInventory(fElapsedTime);
	//case MODE_SHOP:
		//return UpdateShop(fElapsedTime);
	}

	return true;
}


bool Platformer::UpdateLocalMap(float fElapsedTime)
{

	//Update script
	m_script.ProcessCommands(fElapsedTime);

	if (m_script.bPlayerControlEnabled)
	{

		//Handle Input
		if (IsFocused())
		{
			if (GetKey(olc::Key::UP).bHeld)
			{
				m_pPlayer->vy = -6.0f;
			}

			m_pPlayer->bSquat = false; //Reset flag
			if (GetKey(olc::Key::DOWN).bHeld)
			{
				m_pPlayer->vy = 6.0f;
				m_pPlayer->bSquat = true;
			}

			if (GetKey(olc::Key::LEFT).bHeld && !GetKey(olc::Key::RIGHT).bHeld) //LEFT (and ONLY left - otherwise b/c of my velocity mechanics you can accelerate while in "braking" positon if you hold down both buttons
			{
				if (!GetKey(olc::Key::DOWN).bHeld) //Stop movement if crouching/squatting
					m_pPlayer->vx += (m_pPlayer->bObjectOnGround && m_pPlayer->vx <= 0 ? -25.0f : m_pPlayer->bObjectOnGround ? -8.0f : -14.0f) * fElapsedTime; //If on ground accelerating / else if on ground braking+turning around/ else in air
																																	//Player has more control on ground rather than in air, and when turning around it goes a little slower, feels more like og Mario			
				m_pPlayer->fFaceDir = -1.0f; //When drawing, we will scale player with this to give him correct facing							//14.0f is perfect in-air number - when running and jumping you can't quite make it back to the same block you started on
				//fFaceDir = bPlayerOnGround ? -1.0f : fFaceDir; //More like original NES Mario - can only change direction when on ground
			}

			if (GetKey(olc::Key::RIGHT).bHeld && !GetKey(olc::Key::LEFT).bHeld) //RIGHT
			{
				if (!GetKey(olc::Key::DOWN).bHeld) //Stop movement if crouching/squatting
					m_pPlayer->vx += (m_pPlayer->bObjectOnGround && m_pPlayer->vx >= 0 ? 25.0f : m_pPlayer->bObjectOnGround ? 8.0f : 14.0f) * fElapsedTime;

				m_pPlayer->fFaceDir = +1.0f;
				//fFaceDir = bPlayerOnGround ? +1.0f : fFaceDir;
			}

			if (GetKey(olc::Key::A).bReleased) //Access Inventory
			{
				inventoryColor.a = 0; //Reset alpha to do fading
				nInvSelectX = 0;
				nInvSelectY = 0;
				vecInvSelect.clear();
				vecInvSelect.push_back(new InvSelect(nInvSelectX, nInvSelectY, 255));
				nGameMode = MODE_INVENTORY;
			}

			if (GetKey(olc::Key::SPACE).bPressed)
			{
				if (m_pPlayer->bObjectOnGround)
				{
					m_pPlayer->vy = -12.0f;
					//olc::SOUND::PlaySample(sndSampleA); // Plays Sample A
					olc::SOUND::PlaySample(Assets::get().GetSound("sndBoo"));
				}
			}
			if (!GetKey(olc::Key::SPACE).bHeld) //Allows for variable jump height depending on how long space is pressed
			{
				if (m_pPlayer->vy < 0) //Is currently jumping
				{
					m_pPlayer->vy += 35.0f * fElapsedTime;
				}
			}

			if (GetKey(olc::Key::Z).bPressed) //TEST/DEBUG
			{
				CMD(MoveTo(m_pPlayer, 0, 9, 1.0f));
				//CMD(MoveTo(vecDynamics[1], 1, 9, 2.0f));
				//CMD(MoveTo(vecDynamics[2], 1, 9, 2.0f));
				//CMD(ShowDialog({ "Oh silly Jerry" }));
				//CMD(ShowDialog({ "I think OOP", "is really useful" }, olc::RED));
				CMD(MoveTo(m_pPlayer, 7, 9, 1.0f));
				//CMD(ChangeMap("Level 2", 0.0f, 0.0f));
			}

			if (GetKey(olc::F).bPressed) // Interaction
			{
				// Grab a point from the direction the player is facing and check for interactions
				float fTestX, fTestY;

				if (m_pPlayer->fFaceDir == 1.0f)
				{
					fTestX = m_pPlayer->px + 1.5f; //Test probe in center of adjacent tile
					fTestY = m_pPlayer->py + 0.5f;
				}
				if (m_pPlayer->fFaceDir == -1.0f)
				{
					fTestX = m_pPlayer->px - 0.5f;
					fTestY = m_pPlayer->py + 0.5f;
				}

				// Check if test point has hit a dynamic object
				bool bInteraction = false;
				for (auto dyns : vecDynamics)
				{
					if (fTestX > dyns->px && fTestX < (dyns->px + 1.0f)
						&& fTestY > dyns->py && fTestY < (dyns->py + 1.0f))
					{
						if (dyns->bFriendly)
						{
							// Iterate through stack until something responds
							// --Base quest should capture interactions not specified by other quests
							
							// First check if it's quest related
							for (auto &quest : listQuests)
								if (quest->OnInteraction(vecDynamics, dyns, cQuest::TALK))
								{
									bInteraction = true;
									break;
								}

							if (!bInteraction)
							{
								// Some objects just do stuff when you interact with them
								if (dyns->OnInteract(m_pPlayer)) //ADD TALK NATURE
									bInteraction = true;
							}

							if (!bInteraction)
							{
								// Then check if it's map related
								if (pCurrentMap->OnInteraction(vecDynamics, dyns, cMap::TALK))
									bInteraction = true;
							}
						}
						else
						{
							// Interaction not friendly - only enemies - so Attack
						}
					}
				}
			}
		}
	}
	else // Script processor has control 
	{
		// Simply adjust facing direction here.Other objects will do this in behavior --HANDLED IN CREATURE CLASS NOW FOR ALL CREATURES UNDER CONTROL OF SCRIPT
		//m_pPlayer->fFaceDir = (m_pPlayer->vx < 0 ? -1.0f : m_pPlayer->vx > 0 ? 1.0f : m_pPlayer->fFaceDir);

		if (bShowDialog)
		{
			if (GetKey(olc::Key::F).bPressed)
			{
				bShowDialog = false;
				m_script.CompleteCommand();
			}
		}
	}



	//Loop through dynamic objects (creatures?)
	for (auto& object : vecDynamics)
	{
		if (object->bGravityApplies)
			object->vy += 20.0f * fElapsedTime; //Gravity

		if (object->bObjectOnGround)
		{
			object->vx += -3.0f * object->vx * fElapsedTime; //Add some drag so it doesn't feel like ice
			if (fabs(object->vx) < 0.05f) //Clamp vel to 0 if near 0 to allow player to stop
			{
				if (object == m_pPlayer) {
					if (!GetKey(olc::Key::RIGHT).bHeld && !GetKey(olc::Key::LEFT).bHeld) //In release mode, fps is so high that because of fElapsedTime scaling acceleration																		//it wouldn't be able to get past this stopping threshold, leaving player unable to move - if statement is soln
						object->vx = 0.0f;
				}
				//else object not player, and method to stop
				//should be defined in behavior
			}
		}


		////Animation overrides
		//if (bSquat)
		//	animPlayer.ChangeState("squat");

		//Clamp Velocity to prevent getting out of control
		if (object->vx > 10.0f)
			object->vx = 10.0f;

		if (object->vx < -10.0f)
			object->vx = -10.0f;

		if (object->vy > 100.0f)
			object->vy = 100.0f;

		if (object->vy < -100.0f)
			object->vy = -100.0f;


		//Calculate potential new position
		float fNewObjectPosX = object->px + object->vx * fElapsedTime;
		float fNewObjectPosY = object->py + object->vy * fElapsedTime;


		//Collision
		if (object->bSolidVsMap)
		{
			if (object->vx <= 0) //Player moving left
			{
				if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, object->py + 0.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 0.0f, object->py + 0.9f)->solid)  //0.9f because we're not checking Y direction collision right here, and we don't want that to register a collsion, but we still have to check that bottom left corner of the player
				{																																//And the 0.9f allows player to fit in gaps that are only 1 unit across
					fNewObjectPosX = (int)fNewObjectPosX + 1;																					//Basically makes so truncation of tiles doesn't catch us.
					object->vx = 0;
				}
			}
			else if (object->vx > 0) //Player moving Right
			{
				if (pCurrentMap->GetTile(fNewObjectPosX + 1.0f, object->py + 0.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 1.0f, object->py + 0.9f)->solid)
				{
					fNewObjectPosX = (int)fNewObjectPosX;
					object->vx = 0;
				}

			}

			object->bObjectOnGround = false; //Clear flag

			if (object->vy <= 0) //Player moving up
			{
				//Already resolved X-direction collisions, so we can use the new X position and new Y position
				if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 0.99999f, fNewObjectPosY + 0.0f)->solid)
				{
					/***Check for breakable blocks (putting here allows for collision AND breaking)***/  //We could get rid of breakable flag and just use return from OnBreak()
					if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->solid && pCurrentMap->GetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f)->solid) //Needs to be first in if statement(checked first)
					{
						pCurrentMap->GetTile(fNewObjectPosX + 0.5f, fNewObjectPosY + 0.0f)->OnPunch(); //Only break one block at a time
					}

					else if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->solid)
					{
						pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->OnPunch();
					}

					else if (pCurrentMap->GetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f)->solid)
					{
						pCurrentMap->GetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f)->OnPunch();
					}

					/***********************************************************************************/

					fNewObjectPosY = (int)fNewObjectPosY + 1;
					object->vy = 0;
				}
			}
			else //Player moving down
			{
				//Allow to fit in 1 wide gap going down
				if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f + 0.15f, fNewObjectPosY + 1.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 0.99999f - 0.15f, fNewObjectPosY + 1.0f)->solid)
				{
					fNewObjectPosY = (int)fNewObjectPosY;
					object->vy = 0;
					object->bObjectOnGround = true;
				}
				//Still allow to snap into 1 wide opening in wall going left or right
				else if ((GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::RIGHT).bHeld) && !GetKey(olc::Key::DOWN).bHeld) //Change cause doesn't apply to npc's
				{
					if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 1.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 0.99999f, fNewObjectPosY + 1.0f)->solid)
					{
						fNewObjectPosY = (int)fNewObjectPosY;
						object->vy = 0;
						object->bObjectOnGround = true;
					}
				}
			}
		}


		float fDynamicObjectPosX = fNewObjectPosX;
		float fDynamicObjectPosY = fNewObjectPosY;

		// Dynamic Object Collision
		for (auto& dyn : vecDynamics)
		{
			if (dyn != object) // Don't check object against itself
			{
				// If objects are solid then they must not overlap
				if (dyn->bSolidVsDynamic && object->bSolidVsDynamic)
				{
					if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px
						&& object->py < (dyn->py + 1.0f) && (object->py + 1.0f) > dyn->py)
					{
						// First check horizontally - Left first
						if (object->vx <= 0)
							fDynamicObjectPosX = dyn->px + 1.0f;
						else
							fDynamicObjectPosX = dyn->px - 1.0f;

						object->vx = 0;
					}

					if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px
						&& fDynamicObjectPosY < (dyn->py + 1.0f) && (fDynamicObjectPosY + 1.0f) > dyn->py)
					{
						//Then check vertically - Up first
						if (object->vy <= 0)
							fDynamicObjectPosY = dyn->py + 1.0f;
						else
						{
							fDynamicObjectPosY = dyn->py - 1.0f;
							object->bObjectOnGround = true;
						}

						object->vy = 0;
					}

				}
				else
				{
					if (object == m_pPlayer)
					{
						bool bInteraction = false;
						//Object is the player and can interact with things
						if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px
							&& fDynamicObjectPosY < (dyn->py + 1.0f) && (fDynamicObjectPosY + 1.0f) > dyn->py)
						{
							if (!bInteraction)
							{
								// Check if interaction is map related
								pCurrentMap->OnInteraction(vecDynamics, dyn, cMap::WALK);
							}

							if (!bInteraction)
							{
								//Finally just check the object - (for items, non-important characters, etc)
								dyn->OnInteract(object);
							}
						}
					}
				}
			}
		}




		object->px = fDynamicObjectPosX;
		object->py = fDynamicObjectPosY;


		//Update dynamic objects
		object->Update(fElapsedTime, m_pPlayer);

		// Remove quests that have been completed
		auto i = remove_if(listQuests.begin(), listQuests.end(), [](const cQuest* d) {return d->bCompleted; });
		if (i != listQuests.end())
			listQuests.erase(i);
	}

		//Update tile animations
		for (int x = 0; x < pCurrentMap->nWidth; x++)
		{
			for (int y = 0; y < pCurrentMap->nHeight; y++)
			{
				pCurrentMap->GetTile(x, y)->Update(fElapsedTime);
			}
		}


	fCameraPosX = m_pPlayer->px;
	fCameraPosY = m_pPlayer->py;

	//Draw Level 
	int nTileWidth = 22;
	int nTileHeight = 22;
	int nVisibleTilesX = ScreenWidth() / nTileWidth;
	int nVisibleTilesY = ScreenHeight() / nTileHeight;

	//Calculate Top-Leftmost visible tile
	float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
	float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

	//Clamp camera to game boundaries
	if (fOffsetX < 0) fOffsetX = 0;
	if (fOffsetY < 0) fOffsetY = 0;
	if (fOffsetX > pCurrentMap->nWidth - nVisibleTilesX) fOffsetX = pCurrentMap->nWidth - nVisibleTilesX;
	if (fOffsetY > pCurrentMap->nHeight - nVisibleTilesY) fOffsetY = pCurrentMap->nHeight - nVisibleTilesY;

	// Get offsets for smooth movement
	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

	SetDrawTarget(backBuff);

	//Draw visible tile map (overdraw to prevent weird artifacts at screen edges)
	for (int x = -1; x < nVisibleTilesX + 1; x++)
	{
		for (int y = -1; y < nVisibleTilesY + 1; y++)
		{
			FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, pCurrentMap->skyColor); //Background fill -- "sky"
			pCurrentMap->GetTile(x + fOffsetX, y + fOffsetY)->DrawSelf(this, x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);
		}
	}


	

	// Draw Object
	olc::GFX2D::Transform2D t;
	for (auto& object : vecDynamics)
			object->DrawSelf(this, fOffsetX, fOffsetY);

	m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY); // May be a bit wasteful, and could just iterate backwards through ranged for loop above so that player is drawn last... figure out later

	SetDrawTarget(nullptr);
	DrawSprite(0, 0, backBuff);

	// Draw Score
	sScoreString = "Flames Cash: " + to_string(m_pPlayer->nScore);
	DrawString(1, 1, sScoreString, olc::RED);

	// Draw Health
	DrawString(1, 11, "HP: ", olc::DARK_BLUE);
	for (int x = 0; x < m_pPlayer->nHealth; x++)
		FillRect(25 + (x * 6), 10, 5, 8, olc::DARK_BLUE);
	for (int x = 0; x < m_pPlayer->nHealthMax; x++)
		DrawRect(25 + (x * 6), 10, 4, 7, olc::DARK_BLUE);

	// Draw any dialog being displayed
	if (bShowDialog)
		DisplayDialog(vecDialogToShow, 20, 20);


	/*
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Debug+Testing
	if (GetKey(olc::Key::D).bPressed)
		if (showDebug)
			showDebug = false;
		else
			showDebug = true;

	if (showDebug)
	{
		DrawString(0, 15, "Debug: (D to hide)");
		//DrawString(0, 24, "Time Between Animation: " + to_string(animPlayer.fTimeBetweenFrames));
		DrawString(1, 33, "X-Velocity: " + to_string(m_pPlayer->vx) + "\nY-Velocity: " + to_string(m_pPlayer->vy));
		DrawString(1, 53, to_string(fElapsedTime));
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/

	DrawString(140, 1, "Jerryyyyyyyy", olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
	DrawString(0, ScreenHeight() - 25, "MOVE: <- ->, JUMP: Space\nInteract: F, Inventory: A\nPAUSE: P", olc::DARK_MAGENTA);

	//Game end (for now of course)
	if (m_pPlayer->nScore >= 370)
	{
		//m_pPlayer->nScore = 0;

		//LoadLevel(2);
	}

	//Play random Jerry Sounds????

	return true;
}

void Platformer::ShowDialog(vector<string> vecLines, olc::Pixel color)
{
	vecDialogToShow = vecLines;
	bShowDialog = true;
	dialogColor = color;
}

void Platformer::DisplayDialog(vector<string> vecLines, int x, int y)
{
	// Display dialog in a nice looking way
	int nMaxLineLength = 0;
	int nLines = vecLines.size();

	for (auto l : vecLines) if (l.size() > nMaxLineLength) nMaxLineLength = l.size();

	// Draw Box (Default Font size is 8x8, so that constant is used here)
	FillRect(x - 1, y - 1, nMaxLineLength * 8 + 2, nLines * 8 + 2, dialogColor);
	DrawRect(x - 2, y - 2, nMaxLineLength * 8 + 3, nLines * 8 + 3);

	for (int l = 0; l < vecLines.size(); l++)
	{
		DrawString(x, y + l * 8, vecLines[l], olc::WHITE);
	}
}

void Platformer::ChangeMap(string sMapName, float x, float y)
{
	cMap* newMap = Assets::get().GetMap(sMapName);
	if (newMap != nullptr) //Won't work if name passed in is wrong
	{
		// Destroy all dynamics - LATER USE delete FOR OBJECTS ON HEAP
		vecDynamics.clear();
		vecDynamics.push_back(m_pPlayer);

		// Set current map
		pCurrentMap = newMap;

		// Update player location
		m_pPlayer->px = x;
		m_pPlayer->py = y;

		// Create new dynamics from map
		pCurrentMap->PopulateDynamics(vecDynamics);

		// Create new dynamics from quests
		for (auto q : listQuests)
			q->PopulateDynamics(vecDynamics, pCurrentMap->sName);
	}
}

void Platformer::AddQuest(cQuest* quest)
{
	listQuests.push_front(quest);
}

bool Platformer::GiveItem(cItem* item)
{
	listItems.push_back(item);
	return true;
}

bool Platformer::TakeItem(cItem* item)
{
	if (item != nullptr)
	{
		listItems.erase(find(listItems.begin(), listItems.end(), item));
		return true;
	}
	else
		return false;
}

bool Platformer::HasItem(cItem* item)
{
	if (item != nullptr)
	{
		return find(listItems.begin(), listItems.end(), item) != listItems.end();
	}
	else
		return false;
}

bool Platformer::UpdateInventory(float fElapsedTime)
{
	fInvStateTick += fElapsedTime;
	if (fInvStateTick >= 0.02f) //Draw things that use fading
	{
		if (inventoryColor.a + 35 < 255)
			inventoryColor.a += 35;
		else
			inventoryColor.a = 255;

		// ===== Drawing background and fade overlay NOT every frame because improves fps, and for an inventory you don't even notice =====
		//Draw Background
		DrawSprite(0, 0, backBuff);

		//Draw Fade-in Background overlay
		SetPixelMode(olc::Pixel::ALPHA);
		SetPixelBlend(0.7f);
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), inventoryColor);
		SetPixelBlend(1.0f);
		SetPixelMode(olc::Pixel::MASK);
		//==================================================================================================================================

		//Draw selection rectangle (Fade)
		for (int i = 0; i < vecInvSelect.size(); i++)
		{
			if (vecInvSelect[i]->x != nInvSelectX || vecInvSelect[i]->y != nInvSelectY)
			{
				//Fade
				if (vecInvSelect[i]->alpha - 15 > 0)
					vecInvSelect[i]->alpha -= 15;
				else
					vecInvSelect[i]->alpha = 0;
			}
		
			rectColor.a = vecInvSelect[i]->alpha;
			SetPixelMode(olc::Pixel::ALPHA);
			DrawRect(6 + vecInvSelect[i]->x * 25, 24 + vecInvSelect[i]->y * 25, 25, 25, rectColor);
			SetPixelMode(olc::Pixel::MASK);
		}

		fInvStateTick -= 0.02f;
	}

	DrawString(4, 4, "INVENTORY", olc::WHITE, 2);

	int i = 0;
	cItem* highlighted = nullptr;

	// Draw Consumables
	for (auto& item : listItems)
	{
		int x = i % 4;
		int y = i / 4;
		i++;

		olc::GFX2D::Transform2D t;
		t.Translate(8 + x * 25, 26 + y * 25);
		item->DrawSelf(this, t);

		if (nInvSelectX == x && nInvSelectY == y)
			highlighted = item;

		//Update dynamic objects in case items have animation
		item->Update(fElapsedTime); // (Not anymore)This WILL continue to update animations in game, but for what we have now you don't notice. We could always just save current frame and return to it

	}

	if (GetKey(olc::LEFT).bPressed || GetKey(olc::RIGHT).bPressed || GetKey(olc::UP).bPressed || GetKey(olc::DOWN).bPressed)
	{
		if (GetKey(olc::LEFT).bPressed) nInvSelectX--;
		if (GetKey(olc::RIGHT).bPressed) nInvSelectX++;
		if (GetKey(olc::UP).bPressed) nInvSelectY--;
		if (GetKey(olc::DOWN).bPressed) nInvSelectY++;

		if (nInvSelectX < 0) nInvSelectX = 3;
		if (nInvSelectX >= 4) nInvSelectX = 0;
		if (nInvSelectY < 0) nInvSelectY = 3;
		if (nInvSelectY >= 4) nInvSelectY = 0;


		vecInvSelect.insert(vecInvSelect.begin(), new InvSelect(nInvSelectX, nInvSelectY, 255));
	}

	//Remove the selector rects that have faded away
	auto v = remove_if(vecInvSelect.begin(), vecInvSelect.end(), [](const InvSelect* element) {return element->alpha == 0; });
	if (v != vecInvSelect.end())
		vecInvSelect.erase(v);


	if (GetKey(olc::A).bReleased)
	{
		nGameMode = MODE_LOCAL_MAP;
	}

	if (highlighted != nullptr)
	{
		DrawString(8, 160, "SELECTED:");
		DrawString(8, 170, highlighted->sName);

		DrawString(8, 190, "DESCRIPTION:");
		DrawString(8, 200, highlighted->sDescription);

		if (!highlighted->bKeyItem)
		{
			DrawString(80, 160, "(Press F to use)");
		}

		if (GetKey(olc::F).bPressed)
		{
			//Use selected item
			if (!highlighted->bKeyItem)
			{
				if (highlighted->OnUse(m_pPlayer))
				{
					// Item has signaled it has been (fully) used, so remove it
					TakeItem(highlighted);
				}
			}
			else
			{
				
			}
		}
	}

	DrawString(128, 20, "LOCATION:");
	DrawString(128, 28, pCurrentMap->sName);

	DrawString(128, 44, "HEALTH:" + to_string(m_pPlayer->nHealth));
	DrawString(128, 52, "MAX HEALTH:" + to_string(m_pPlayer->nHealthMax));

	return true;
}