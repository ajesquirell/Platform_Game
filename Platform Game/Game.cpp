#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <string>
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

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;

		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L".....#.........................######...........................";
		sLevel += L"................#######.........................................";
		sLevel += L"...............#.......#............................#...........";
		sLevel += L"..............#.........#.......................###.#...........";
		sLevel += L"###################################################.######..####";
		sLevel += L"................................#...................#......#....";
		sLevel += L"................................#...................#.....#.....";
		sLevel += L"................................#.....##############.....#......";
		sLevel += L"................................#.......................#.......";
		sLevel += L"................................############################....";


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
				sLevel[y * nLevelHeight + x] = c;
		};

		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;

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
				fPlayerVelX = -6.0f;
			}

			if (GetKey(olc::Key::RIGHT).bHeld)
			{
				fPlayerVelX = 6.0f;
			}
		}

		fPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		fPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;


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

		//Draw visible tile map
		for (int x = 0; x < nVisibleTilesX; x++)
		{
			for (int y = 0; y < nVisibleTilesY; y++)
			{
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
				switch (sTileID)
				{
				case L'.':
					FillRect(x * nTileWidth, y * nTileHeight, nTileWidth, nTileHeight, olc::CYAN);
					break;

				case L'#':
					FillRect(x * nTileWidth, y * nTileHeight, nTileWidth, nTileHeight, olc::RED);
					break;
				default:
					break;
				}
			}
		}

		//Draw Player
		FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileHeight, nTileWidth, nTileHeight, olc::GREEN);

		//for (int x = 0; x < ScreenWidth(); x++)
		//	for (int y = 0; y < ScreenHeight(); y++)
		//		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
		return true;
	}

};


int main()
{
	Platformer game;
	if (game.Construct(160, 120, 8, 8))
		game.Start();

	return 0;
}