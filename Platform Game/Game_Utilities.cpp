#include "Game_Utilities.h"
#include "Jerry_Engine.h"
#include "olcPixelGameEngine.h"


void utility::DrawInvertedSprite(olc::PixelGameEngine* pge, float px, float py, olc::Sprite* spr)
{
	olc::Sprite* sprToDraw = new olc::Sprite(spr->width, spr->height);

	for (int x = spr->width; x > 0; x--)
	{
		for (int y = 0; y < spr->height; y++)
		{
			sprToDraw->SetPixel(x - 1, y, spr->GetPixel(spr->width - x, y));
		}
	}

	pge->DrawSprite(px, py, sprToDraw);
	delete sprToDraw;
}