#include "Dynamics.h"
#include "Items.h"
#include "Jerry_Engine.h"

//================================================================================================
//											Dynamic Item
//================================================================================================

cDynamic_Item::cDynamic_Item(float x, float y, cItem* i) : cDynamic("pickup")
{
	px = x;
	py = y;
	item = i;
	bSolidVsDynamic = false;
	bSolidVsMap = true;
	bFriendly = true;
	bCollected = false;
}

void cDynamic_Item::DrawSelf(olc::PixelGameEngine* pge, float ox, float oy)
{
	if (bCollected)
		return;
	
	olc::GFX2D::Transform2D t;
	t.Translate(-22 / 2, -22 / 2); //Align sprite to 0,0 to do affine transformations

	//Do any transformations here

	t.Translate((px - ox) * 22 + 22 / 2, (py - oy) * 22 + 22 / 2); //If we want to change resolution later, need to make a global constant instead of 22

	item->animItem.DrawSelf(pge, t);
}

void cDynamic_Item::OnInteract(cDynamic* player)
{
	if (bCollected)
		return;

	if (item->OnInteract(player))
	{
		// Add item to inventory
		g_engine->GiveItem(item);
	}
	bCollected = true;
}
