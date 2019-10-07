#include "Dynamics.h"

cDynamic_Teleport::cDynamic_Teleport(float x, float y, std::string mapName, float tx, float ty) : cDynamic("Teleport")
{
	px = x;
	py = y;
	fMapPosX = tx;
	fMapPosY = ty;
	sMapName = mapName;
	bSolidVsDynamic = false;
	bSolidVsMap = true;
}

void cDynamic_Teleport::DrawSelf(olc::PixelGameEngine* pge, float ox, float oy)
{
	// Does Nothing
	pge->DrawCircle(((px + 0.5f) - ox) * 22, ((py + 0.5f) - oy) * 22, 0.5f * 22, olc::YELLOW); //For debugging
}
 
void cDynamic_Teleport::Update(float fElapsedTime, cDynamic* player)
{
	// Does Nothing - Static Object
}
