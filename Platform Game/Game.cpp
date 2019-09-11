#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

#include <iostream>
#include <string>
using namespace std;


class cAnimator
{
public:
	std::map<std::string, std::vector<olc::Sprite*>> mapStates;

public:
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.1f;
	float fTimeCounter = 0.0f;


	void ChangeState(std::string s)
	{
		if (s != sCurrentState)
		{
			sCurrentState = s;
			fTimeCounter = 0;
			nCurrentFrame = 0;
		}
	}

	void Update(float fElapsedTime)
	{
		fTimeCounter += fElapsedTime;
		if (fTimeCounter >= fTimeBetweenFrames)
		{
			fTimeCounter -= fTimeBetweenFrames;
			nCurrentFrame++;
			if (nCurrentFrame >= mapStates[sCurrentState].size())
				nCurrentFrame = 0;
		}
	}

	void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
	{
		olc::GFX2D::DrawSprite(mapStates[sCurrentState][nCurrentFrame], t);
	}

};


class Platformer : public olc::PixelGameEngine
{
public:
	Platformer()
	{
		sAppName = "LU Platformer";
	}

private:
	//Level Storage
	wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	//Player Properties
	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;

	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;

	bool bPlayerOnGround = false;

	//Camera Properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	//Sprite Resources
	olc::Sprite* spriteTiles = nullptr;
	olc::Sprite* spriteGround = nullptr;
	olc::Sprite* spriteCoin = nullptr;
	olc::Sprite* spriteMan = nullptr;
	olc::Sprite* spriteManJump = nullptr;


	//Sprite selection flags
	int nDirModX = 0;
	int nDirModY = 0;

	//Pickup variables
	bool bPickupCollected = false;
	int nPlayerScore = 0;
	string sScoreString;

	//Sound
	int sndSampleA;
	int sndSampleB;
	int sndSampleC;


public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;

		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"..................ooo...........................................";
		sLevel += L".................ooooo..........................................";
		sLevel += L".....#.........................######...........................";
		sLevel += L"................#######...........ooooooooo.....................";
		sLevel += L"...............#.......#.........o..................#...........";
		sLevel += L"..............#.........#...ooooo...............###.#...........";
		sLevel += L"###################################################.#####...####";
		sLevel += L"................................#...................#......#....";
		sLevel += L"................................#...................#.....#.....";
		sLevel += L"................................#.....##############.....#......";
		sLevel += L"................................#........oooooooooo.....#.......";
		sLevel += L"................................############################....";

		spriteTiles = new olc::Sprite("../Sprites/Megaman.png");
		spriteGround = new olc::Sprite("../Sprites/Ground.png");
		spriteCoin = new olc::Sprite("../Sprites/Coin.png");
		spriteMan = new olc::Sprite("../Sprites/Piskel.png");
		spriteManJump = new olc::Sprite("../Sprites/Piskel_Jump.png");

		SetPixelMode(olc::Pixel::MASK); //Allow Transparency


		//Sound
		olc::SOUND::InitialiseAudio(44100, 1, 8, 512);

		sndSampleA = olc::SOUND::LoadAudioSample("../Sounds/SampleA.wav");
		sndSampleB = olc::SOUND::LoadAudioSample("../Sounds/SampleB.wav");
		sndSampleC = olc::SOUND::LoadAudioSample("../Sounds/SampleC.wav");

		olc::SOUND::PlaySample(sndSampleC, true); // Plays Sample C loop

		return true;
	}

	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Utility Lambdas
		auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sLevel[y * nLevelWidth + x] = c;
		};

		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;

		//Handle Input
		if (IsFocused())
		{
			if (GetKey(olc::Key::UP).bHeld)
			{
				fPlayerVelY = -6.0f;
			}

			if (GetKey(olc::Key::DOWN).bHeld)
			{
				fPlayerVelY = 6.0f;
			}

			if (GetKey(olc::Key::LEFT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime; //Player has more control on ground rather than in air
				nDirModY = 1;
			}

			if (GetKey(olc::Key::RIGHT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
				nDirModY = 0;
			}

			if (GetKey(olc::Key::SPACE).bPressed)
			{
				if (fPlayerVelY == 0) //Player not already jumping or falling. Should be true if player on ground
				{						//Maybe could jump again at very top of jump, but theres a way around that...
					fPlayerVelY = -12.0f;
					nDirModX = 1;
					olc::SOUND::PlaySample(sndSampleA); // Plays Sample C loop
				}
			}
		}


		fPlayerVelY += 20.0f * fElapsedTime; //Gravity

		if (bPlayerOnGround) //Add some drag so it doesn't feel like ice
		{
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f) //Clamp vel to 0 if near 0 to allow player to stop
				fPlayerVelX = 0.0f;
		}


		//Clamp Velocity to prevent getting out of control
		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;
		
		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;

		//Calculate potential new position
		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		//Check for pickups!
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
		{
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');
			bPickupCollected = true;
		}

		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
		{
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');
			bPickupCollected = true;
		}

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
		{
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');
			bPickupCollected = true;
		}

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
		{
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');
			bPickupCollected = true;
		}
		
		if (bPickupCollected)
		{
			nPlayerScore += 10;
			bPickupCollected = false; //Reset
			olc::SOUND::PlaySample(sndSampleB); // Plays Sample B
		}


		//Collision
		if (fPlayerVelX <= 0) //Player moving left
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'.')  //0.9f because we're not checking Y direction collision right here, and we don't want that to register a collsion, but we still have to check that bottom left corner of the player
			{																																//And the 0.9f allows player to fit in gaps that are only 1 unit across
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;																					//Basically makes so truncation of tiles doesn't catch us.
				fPlayerVelX = 0;
			}
		}
		else //Player moving Right
		{
			if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'.')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;
			}

		}

		bPlayerOnGround = false; //Clear flag
		if (fPlayerVelY <= 0) //Player moving up
		{
			//Already resolved X-direction collisions, so we can use the new X position and new Y position
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'.')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else //Player moving down
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'.')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true;
				nDirModX = 0;
			}
		}

		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;


		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		//Draw Level
		int nTileWidth = 16;
		int nTileHeight = 16;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

		//Calculate Top-Leftmost visible tile
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

		//Clamp camera to game boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;

		// Get offsets for smooth movement
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

		//Draw visible tile map (overdraw to prevent weird artifacts at screen edges)
		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
				switch (sTileID)
				{
				case L'.': // Sky
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::CYAN);
					break;

				case L'#':
					//FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::RED);
					DrawSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, spriteGround);
					break;
				case L'o':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, nTileWidth, nTileHeight, olc::CYAN);
					DrawSprite(x* nTileWidth - fTileOffsetX, y* nTileHeight - fTileOffsetY, spriteCoin);
				default:
					break;
				}
			}
		}

		//Draw Player
		//FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);
		DrawSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, (nDirModX ? spriteManJump : spriteMan));

		//Draw Score
		sScoreString = "Score: " + to_string(nPlayerScore);
		DrawString(ScreenWidth() * (3 / 4), 0, sScoreString);

		return true;
	}

};


int main()
{
	Platformer game;
	if (game.Construct(256, 240, 4, 4))
		game.Start();

	return 0;
}