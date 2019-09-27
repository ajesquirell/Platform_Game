#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

#include "Animator.h"
#include "Maps.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;


class Platformer : public olc::PixelGameEngine
{
public:
	Platformer()
	{
		sAppName = "LU Platformer";
	}

private:
	//Level Storage
	cMap* currentMap = nullptr;

	//Player Properties
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;

	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;

	bool bPlayerOnGround = false;
	//cDynamic_Creature* m_pPlayer = nullptr;

	//Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	//Sprite Resources
	olc::Sprite* spriteFloor = nullptr;
	olc::Sprite* spriteBrick = nullptr;
	olc::Sprite* spriteTEST = nullptr;

	//Sprite selection flags
	//int nDirModX = 0;
	//int nDirModY = 0;
	float fFaceDir = 1.0f;
	int nPlayerWidth;
	int nPlayerHeight;
	bool bSquat;

	//Sprite Animation Class
	cAnimator animPlayer;
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
	bool HandlePickup(wchar_t c) //Function for handling the different pickups without jumbling up the game loop with code for every single pickup
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

	bool OnUserCreate() override
	{
		//Load Sprites

		spriteBrick = new olc::Sprite("../Sprites/Brick.png");
		spriteFloor = new olc::Sprite("../Sprites/Floor.png");
		spriteTEST = new olc::Sprite("../Sprites/Piskel.png");

		//Animated
			//Jerry
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("../Sprites/Jerry_Idle.png"));

		animPlayer.mapStates["run"].push_back(new olc::Sprite("../Sprites/Jerry_Run_1.png"));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("../Sprites/Jerry_Run_2.png"));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("../Sprites/Jerry_Run_3.png"));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("../Sprites/Jerry_Run_4.png"));

		animPlayer.mapStates["brake"].push_back(new olc::Sprite("../Sprites/Jerry_Brake_1.png"));
		animPlayer.mapStates["brake"].push_back(new olc::Sprite("../Sprites/Jerry_Brake_2.png"));
		animPlayer.mapStates["brake"].push_back(new olc::Sprite("../Sprites/Jerry_Brake_3.png"));
		animPlayer.mapStates["brake"].push_back(new olc::Sprite("../Sprites/Jerry_Brake_4.png"));
		animPlayer.mapStates["brake"].push_back(new olc::Sprite("../Sprites/Jerry_Brake_5.png"));

		animPlayer.mapStates["squat"].push_back(new olc::Sprite("../Sprites/Jerry_Squat.png"));

		animPlayer.mapStates["jump"].push_back(new olc::Sprite("../Sprites/Jerry_Jump_1.png"));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("../Sprites/Jerry_Jump_2.png"));

		
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("../Sprites/Jerry_Fall.png"));
			
			//Money
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_00.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_01.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_02.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_03.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_04.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_05.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_06.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_07.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_08.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_09.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_10.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_11.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_12.png"));
		animMoney.mapStates["normal"].push_back(new olc::Sprite("../Sprites/Money/Money_13.png"));

		//Set initial animated states
		animPlayer.ChangeState("idle");
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


		//Map
		currentMap = new cLevel1;


		return true;
	}

	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
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

		nPlayerWidth = animPlayer.mapStates[animPlayer.sCurrentState][animPlayer.nCurrentFrame]->width;
		nPlayerHeight = animPlayer.mapStates[animPlayer.sCurrentState][animPlayer.nCurrentFrame]->height;
		animPlayer.Update(fElapsedTime);
		animMoney.Update(fElapsedTime);

		// Utility Lambdas
		/*auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sCurrentLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sCurrentLevel[y * nLevelWidth + x] = c;
		};
*/
		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;

		//Handle Input
		if (IsFocused())
		{
			if (GetKey(olc::Key::UP).bHeld)
			{
				fPlayerVelY = -6.0f;
			}

			bSquat = false; //Reset flag
			if (GetKey(olc::Key::DOWN).bHeld)
			{
				fPlayerVelY = 6.0f;
				bSquat = true;
			}

			if (GetKey(olc::Key::LEFT).bHeld && !GetKey(olc::Key::RIGHT).bHeld) //LEFT (and ONLY left - otherwise b/c of my velocity mechanics you can accelerate while in "braking" positon if you hold down both buttons
			{
				if (!GetKey(olc::Key::DOWN).bHeld) //Stop movement if crouching/squatting
					fPlayerVelX += (bPlayerOnGround && fPlayerVelX <= 0 ? -25.0f : bPlayerOnGround ? -8.0f : -14.0f) * fElapsedTime; //If on ground accelerating / else if on ground braking+turning around/ else in air
																																	//Player has more control on ground rather than in air, and when turning around it goes a little slower, feels more like og Mario			
				fFaceDir = -1.0f; //When drawing, we will scale player with this to give him correct facing							//14.0f is perfect in-air number - when running and jumping you can't quite make it back to the same block you started on
				//fFaceDir = bPlayerOnGround ? -1.0f : fFaceDir; //More like original NES Mario - can only change direction when on ground
			}

			if (GetKey(olc::Key::RIGHT).bHeld && !GetKey(olc::Key::LEFT).bHeld) //RIGHT
			{
				if (!GetKey(olc::Key::DOWN).bHeld) //Stop movement if crouching/squatting
					fPlayerVelX += (bPlayerOnGround && fPlayerVelX >= 0 ? 25.0f : bPlayerOnGround ? 8.0f : 14.0f) * fElapsedTime; 

				fFaceDir = +1.0f;
				//fFaceDir = bPlayerOnGround ? +1.0f : fFaceDir;
			}

			if (GetKey(olc::Key::SPACE).bPressed)
			{
				if (bPlayerOnGround)
				{
					fPlayerVelY = -12.0f;
					//olc::SOUND::PlaySample(sndSampleA); // Plays Sample A
					olc::SOUND::PlaySample(sndBoo);
				}
			}
		}


		fPlayerVelY += 20.0f * fElapsedTime; //Gravity

		if (bPlayerOnGround) 
		{
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime; //Add some drag so it doesn't feel like ice
			if (fabs(fPlayerVelX) < 0.05f) //Clamp vel to 0 if near 0 to allow player to stop
			{
				if (!GetKey(olc::Key::RIGHT).bHeld && !GetKey(olc::Key::LEFT).bHeld) //In release mode, fps is so high that because of fElapsedTime scaling acceleration
				{																		//it wouldn't be able to get past this stopping threshold, leaving player unable to move - if statement is soln
					fPlayerVelX = 0.0f;
				}
				animPlayer.ChangeState("idle");

				if (fabs(fPlayerVelX) > 0.01f) //LITERALLY all this does is allow him to "dance" by trying to move when up against a wall, probably shouldn't have this here but I think it's funny
					animPlayer.ChangeState("run");
			}
			else if (fFaceDir == +1.0f && fPlayerVelX < 0 || fFaceDir == -1.0f && fPlayerVelX > 0) //Just changed direction but still moving the opposite way -> braking
			{
					animPlayer.ChangeState("brake");
			}
			else
			{
				animPlayer.ChangeState("run");
			}

			if (bSquat) //This is changed in the Input Handling section
				animPlayer.ChangeState("squat");
		}
		else
		{
			if (fPlayerVelY < 0)
			{
				animPlayer.ChangeState("jump", true);
			}
			else
			{
				animPlayer.ChangeState("fall");
			}
		}

		////Animation overrides
		//if (bSquat)
		//	animPlayer.ChangeState("squat");

		//Clamp Velocity to prevent getting out of control
		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;
		
		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;


		//Change running animation speed based on fPlayerVelX
		if (animPlayer.sCurrentState == "run")
			animPlayer.fTimeBetweenFrames = 0.1f * (10.0f / fabs(fPlayerVelX));
		else if (animPlayer.sCurrentState == "brake")
			animPlayer.fTimeBetweenFrames = 0.05f;
		else
			animPlayer.fTimeBetweenFrames = 0.1f;


		//Calculate potential new position
		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		//Check for pickups! 
		/*if (HandlePickup(currentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f)))
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)))
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f)))
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

		if (HandlePickup(GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f)))
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');*/


		//Collision
		if (fPlayerVelX <= 0) //Player moving left
		{
			if (currentMap->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f)->solid || currentMap->GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f)->solid)  //0.9f because we're not checking Y direction collision right here, and we don't want that to register a collsion, but we still have to check that bottom left corner of the player
			{																																//And the 0.9f allows player to fit in gaps that are only 1 unit across
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;																					//Basically makes so truncation of tiles doesn't catch us.
				fPlayerVelX = 0;
			}
		}
		else if (fPlayerVelX > 0) //Player moving Right
		{
			if (currentMap->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f)->solid || currentMap->GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f)->solid)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;
			}

		}

		bPlayerOnGround = false; //Clear flag
		if (fPlayerVelY <= 0) //Player moving up
		{
			//Already resolved X-direction collisions, so we can use the new X position and new Y position
			if (currentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f)->solid || currentMap->GetTile(fNewPlayerPosX + 0.99999f, fNewPlayerPosY + 0.0f)->solid)
			{
				/***Check for breakable blocks (putting here allows for collision AND breaking)***/
				//if (currentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'B' && currentMap->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'B') //Needs to be first in if statement(checked first)
				//{
				//	SetTile(fNewPlayerPosX + 0.5f, fNewPlayerPosY + 0.0f, L'b');
				//	//SetTile(fNewPlayerPosX + 0.5f, fNewPlayerPosY + 0.0f, L'.'); //Only break one block at a time
				//}

				//else if (currentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'B')
				//{
				//	SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'b');
				//	//SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');
				//}

				//else if (currentMap->GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'B')
				//{
				//	SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'b');
				//	//SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');
				//}

				/***********************************************************************************/

				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else //Player moving down
		{
			if (currentMap->GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f)->solid || currentMap->GetTile(fNewPlayerPosX + 0.99999f, fNewPlayerPosY + 1.0f)->solid)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true;
			}
		}




		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;


		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

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
		if (fOffsetX > currentMap->nWidth - nVisibleTilesX) fOffsetX = currentMap->nWidth - nVisibleTilesX;
		if (fOffsetY > currentMap->nHeight - nVisibleTilesY) fOffsetY = currentMap->nHeight - nVisibleTilesY;

		// Get offsets for smooth movement
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

		//Draw visible tile map (overdraw to prevent weird artifacts at screen edges)
		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				FillRect(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, currentMap->skyColor); //Background fill -- "sky"
				currentMap->GetTile(x + fOffsetX, y + fOffsetY)->DrawSelf(this, x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY);

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

		//Draw Player
		//FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);
		//DrawSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, (nDirModX ? spriteManJump : spriteMan));

		// Draw Player

		olc::GFX2D::Transform2D t;
		t.Translate(-nPlayerWidth / 2, -nPlayerHeight / 2); //Align player sprite to 0,0 to do affine transformations
		t.Scale(fFaceDir, 1.0f); //BUG WITH THIS??? CUTS OFF A RIGHT COLUMN OF PIXELS WHEN REFLECTED? Yeah bug is in the PGEX/Scaling transformation somewhere. Could just double the png's used and switch like that instead of scaling

		//t.Rotate(fPlayerPosX);
		t.Translate((fPlayerPosX - fOffsetX)* nTileWidth + nPlayerWidth / 2, (fPlayerPosY - fOffsetY) * nTileHeight + nPlayerHeight / 2);

		//SetPixelMode(olc::Pixel::ALPHA);
		animPlayer.DrawSelf(this, t);
		//SetPixelMode(olc::Pixel::NORMAL);

		//Draw Score
		sScoreString = "Flames Cash: " + to_string(nPlayerScore);
		DrawString(0, 0, sScoreString, olc::RED);

		//Debug+Testing
		if (GetKey(olc::Key::D).bPressed)
			if (showDebug)
				showDebug = false;
			else
				showDebug = true;

		if (showDebug)
		{
			DrawString(0, 15, "Debug: (D to hide)");
			DrawString(0, 24, "Time Between Animation: " + to_string(animPlayer.fTimeBetweenFrames));
			DrawString(1, 33, "X-Velocity: " + to_string(fPlayerVelX) + "\nY-Velocity: " + to_string(fPlayerVelY));
			DrawString(1, 53, to_string(fElapsedTime));
		}

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

};


int main()
{
	Platformer game;
	if (game.Construct(264, 242, 4, 4))
	//if (game.Construct(264, 242, 4, 4, false, true)) //VSync
	//if (game.Construct(264, 900, 4, 4))

		game.Start();

	return 0;
}
