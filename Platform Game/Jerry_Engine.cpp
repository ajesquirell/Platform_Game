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
		nPlayerScore += 10;
		olc::SOUND::PlaySample(sndSampleB); // Plays Sample B
		success = true;
		break;

	case (TEST):
		//skyColor = olc::DARK_YELLOW;
		olc::SOUND::PlaySample(sndWuWuWu);
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
	cMap::g_script = &m_script;

	//Load Assets (Sprites, Maps)
	Assets::get().LoadSprites(); //Can get away with loading everything at once because this is a small game
	Assets::get().LoadMaps();

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

	sndSampleA = olc::SOUND::LoadAudioSample("../Sounds/SampleA.wav");
	sndSampleB = olc::SOUND::LoadAudioSample("../Sounds/SampleB.wav");
	sndSampleC = olc::SOUND::LoadAudioSample("../Sounds/SampleC.wav");
	sndBoo = olc::SOUND::LoadAudioSample("../Sounds/Boo.wav");
	sndWuWuWu = olc::SOUND::LoadAudioSample("../Sounds/WuWuWu.wav");
	sndGetMoney = olc::SOUND::LoadAudioSample("../Sounds/GetMoney.wav");

	olc::SOUND::PlaySample(sndSampleC, true); // Plays Sample C loop

	//Player init
	m_pPlayer = new cDynamic_Creature("Jerry"); //For now sprites/ anims are hard coded to be Jerry

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
			olc::SOUND::StopSample(sndSampleC);
			olc::SOUND::PlaySample(sndGetMoney);
		}
		else {
			bGamePaused = false; /*Unpause*/
			olc::SOUND::PlaySample(sndSampleC, true);
		}
	}
	if (bGamePaused)
	{
		DrawString(ScreenWidth() / 2.5f, ScreenHeight() / 2.5f, "Paused");
		return true; //Game loop won't execute
	}

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

			if (GetKey(olc::Key::SPACE).bPressed)
			{
				if (m_pPlayer->bObjectOnGround)
				{
					m_pPlayer->vy = -12.0f;
					//olc::SOUND::PlaySample(sndSampleA); // Plays Sample A
					olc::SOUND::PlaySample(sndBoo);
				}
			}
			if (!GetKey(olc::Key::SPACE).bHeld) //Allows for variable jump height depending on how long space is pressed
			{
				if (m_pPlayer->vy < 0) //Is currently jumping
				{
					m_pPlayer->vy += 35.0f * fElapsedTime;
				}
			}

			if (GetKey(olc::Key::Z).bReleased)
			{
				CMD(MoveTo(m_pPlayer, 0, 9, 1.0f));
				CMD(MoveTo(vecDynamics[1], 1, 9, 2.0f));
				CMD(MoveTo(vecDynamics[2], 1, 9, 2.0f));
				//CMD(ShowDialog({ "Oh silly Jerry" }));
				//CMD(ShowDialog({ "I think OOP", "is really useful" }));
				//CMD(MoveTo(m_pPlayer, 7, 9, 2.5f));
				//CMD(ChangeMap("Level 2", 0.0f, 0.0f));

			}
		}
	}
	else // Script processor has control 
	{
		// Simply adjust facing direction here.Other objects will do this in behavior
		m_pPlayer->fFaceDir = (m_pPlayer->vx < 0 ? -1.0f : m_pPlayer->vx > 0 ? 1.0f : m_pPlayer->fFaceDir);

		if (bShowDialog)
		{
			if (GetKey(olc::Key::SPACE).bReleased)
			{
				bShowDialog = false;
				m_script.CompleteCommand();
			}
		}
	}



	//Loop through dynamic objects (creatures?)
	for (auto& object : vecDynamics)
	{
		object->vy += 20.0f * fElapsedTime; //Gravity

		if (object->bObjectOnGround)
		{
			object->vx += -3.0f * object->vx * fElapsedTime; //Add some drag so it doesn't feel like ice
			if (fabs(object->vx) < 0.05f) //Clamp vel to 0 if near 0 to allow player to stop
			{
				//if (object == m_pPlayer)
				//{
				if (!GetKey(olc::Key::RIGHT).bHeld && !GetKey(olc::Key::LEFT).bHeld) //In release mode, fps is so high that because of fElapsedTime scaling acceleration
				{																		//it wouldn't be able to get past this stopping threshold, leaving player unable to move - if statement is soln
					object->vx = 0.0f;
				}
				//}
				//else if not the player
				//something else so it doesn't get stuck
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

		//Check for pickups! 
		/*if (HandlePickup(pCurrentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f)))
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)))
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f)))
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f)))
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');*/


		//Collision
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
					pCurrentMap->GetTile(fNewObjectPosX + 0.5f, fNewObjectPosY + 0.0f)->OnPunch();
					//pCurrentMap->SetTile(fNewObjectPosX + 0.5f, fNewObjectPosY + 0.0f, new cTile_Sky); //Only break one block at a time
				}

				else if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->solid)
				{
					pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f)->OnPunch();
					//pCurrentMap->SetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 0.0f, new cTile_Sky);
				}

				else if (pCurrentMap->GetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f)->solid)
				{
					pCurrentMap->GetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f)->OnPunch();
					//pCurrentMap->SetTile(fNewObjectPosX + 1.0f, fNewObjectPosY + 0.0f, new cTile_Sky);
				}

				/***********************************************************************************/

				fNewObjectPosY = (int)fNewObjectPosY + 1;
				object->vy = 0;
			}
		}
		else //Player moving down
		{ 
				if (pCurrentMap->GetTile(fNewObjectPosX + 0.0f, fNewObjectPosY + 1.0f)->solid || pCurrentMap->GetTile(fNewObjectPosX + 0.99999f, fNewObjectPosY + 1.0f)->solid)
				{
					fNewObjectPosY = (int)fNewObjectPosY;
					object->vy = 0;
					object->bObjectOnGround = true;
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

				}
				else
				{
					if (object == m_pPlayer)
					{
						//Object is the player and can interact with things
						if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px
							&& fDynamicObjectPosY < (dyn->py + 1.0f) && (fDynamicObjectPosY + 1.0f) > dyn->py)
						{
							// Check if interaction is map related
							pCurrentMap->OnInteraction(vecDynamics, dyn, cMap::WALK);
						}
					}
				}
			}
		}




		object->px = fNewObjectPosX;
		object->py = fNewObjectPosY;


		//Update dynamic objects
		object->Update(fElapsedTime, m_pPlayer );

		//Update tile animations
		for (int x = 0; x < pCurrentMap->nWidth; x++)
		{
			for (int y = 0; y < pCurrentMap->nHeight; y++)
			{
				pCurrentMap->GetTile(x, y)->Update(fElapsedTime);
			}
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

	//Draw visible tile map (overdraw to prevent weird artifacts at screen edges)
	for (int x = -1; x < nVisibleTilesX + 1; x++)
	{
		for (int y = -1; y < nVisibleTilesY + 1; y++)
		{
			FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, pCurrentMap->skyColor); //Background fill -- "sky"
			pCurrentMap->GetTile(x + fOffsetX, y + fOffsetY)->DrawSelf(this, x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);

			//switch (sTileID)
			//{
			//case L'.': // Sky
			//	FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, skyColor);
			//	break;

			//case "floor": //Floor
			//	FillRect(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::CYAN);
			//	DrawSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, spriteFloor);
			//	break;

			//case L'B':
			//	//FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::RED);
			//	DrawSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, spriteBrick);
			//	break;

			//case L'b':
			//{
			//	FillRect(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, skyColor);
			//	
			//	tile.animTile.Update(fElapsedTime);
			//	//olc::GFX2D::Transform2D brickTrans;
			//	//brickTrans.Translate(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY);
			//	//animMoney.DrawSelf(this, brickTrans);
			//	DrawSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, tile.GetCurrentFrame());
			//	break;
			//}
			//	
			//case COIN:
			//{ //Brackets indicate scope lives only within this case statement (important for declaration of variables)
			//	FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, skyColor);
			//	olc::GFX2D::Transform2D moneyTrans;
			//	moneyTrans.Translate(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);
			//	animMoney.DrawSelf(this, moneyTrans);
			//	break;
			//}

			//case TEST:
			//{ //Brackets indicate scope lives only within this case statement (important for declaration of variables)
			//	FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, skyColor);
			//	olc::GFX2D::Transform2D moneyTrans;
			//	moneyTrans.Translate(-nTileWidth / 2, -nTileHeight / 2);
			//	moneyTrans.Rotate(fPlayerVelY);
			//	moneyTrans.Scale(fFaceDir, 1.0f);
			//	moneyTrans.Translate(x * nTileWidth - fTileOffsetX + nTileWidth / 2, y * nTileHeight - fTileOffsetY + nTileHeight / 2);

			//	animMoney.DrawSelf(this, moneyTrans);
			//	break;
			//}

			//case L'P':
			//	fPlayerPosX = x + fOffsetX;
			//	fPlayerPosY = y + fOffsetY;
			//	SetTile(x + fOffsetX, y + fOffsetY, L'.');

			//default:
			//	FillRect(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::BLACK);
			//	break;
			//}
		}
	}

	// Draw Object
	for (auto& object : vecDynamics)
		object->DrawSelf(this, fOffsetX, fOffsetY);

	m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY); // May be a bit wasteful, and could just iterate backwards through ranged for loop above so that player is drawn last... figure out later

	// Draw any dialog being displayed
	if (bShowDialog)
		DisplayDialog(vecDialogToShow, 20, 20);

	//Draw Score
	sScoreString = "Flames Cash: " + to_string(nPlayerScore);
	DrawString(0, 0, sScoreString, olc::RED);

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

	DrawString(140, 0, "Jerryyyyyyyy", olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
	DrawString(0, ScreenHeight() - 20, "MOVE: <- ->, JUMP: Space, \nPAUSE: P", olc::DARK_BLUE);

	//Game end (for now of course)
	if (nPlayerScore >= 370)
	{
		nPlayerScore = 0;

		//LoadLevel(2);
	}

	//Play random Jerry Sounds????

	return true;
}

void Platformer::ShowDialog(vector<string> vecLines)
{
	vecDialogToShow = vecLines;
	bShowDialog = true;
}

void Platformer::DisplayDialog(vector<string> vecLines, int x, int y)
{
	// Display dialog in a nice looking way
	int nMaxLineLength = 0;
	int nLines = vecLines.size();

	for (auto l : vecLines) if (l.size() > nMaxLineLength) nMaxLineLength = l.size();

	// Draw Box (Default Font size is 8x8, so that constant is used here)
	FillRect(x - 1, y - 1, x + nMaxLineLength * 8 + 1, y + nLines * 8 + 1, olc::DARK_BLUE);
	DrawLine(x - 2, y - 2, x - 2, y + nLines * 8 + 1);
	DrawLine(x + nMaxLineLength * 8 + 1, y - 2, x + nMaxLineLength * 8 + 1, y + nLines * 8 + 1);
	DrawLine(x - 2, y - 2, x + nMaxLineLength * 8 + 1, y - 2);
	DrawLine(x - 2, y + nLines * 8 + 1, x + nMaxLineLength * 8 + 1, y + nLines * 8 + 1);

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
	}
}