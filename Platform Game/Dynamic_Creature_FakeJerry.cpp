#include "Dynamics.h"

cDynamic_Creature_FakeJerry::cDynamic_Creature_FakeJerry() : cDynamic_Creature("Fake Jerry")
{
	bFriendly = false;
	nHealth = 10;
	nMaxHealth = 10;
	fStateTick = 2.0f;

	fDirectionX = 0.0f;
	fDirectionY = 0.0f;
}

void cDynamic_Creature_FakeJerry::Behavior(float fElapsedTime, cDynamic* player)
{
	// Check if player is nearby
	float fTargetX = player->px - px;
	float fTargetY = player->py - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	//fDirectionX //Playing around with making some of these variables static does some pretty interesting things
	
	

	fStateTick -= fElapsedTime;
	if (fStateTick <= 0.0f)
	{
		if (fDistance < 8.0f && fDistance != 0) // Avoid divide by 0 error that freezes program
		{
			if (fTargetX < 0.0f)
				fDirectionX = -1.0f;
			else
				fDirectionX = 1.0f;
		}
		else
		{
			vx = 0;
		}

		if (fTargetY < 0 && bObjectOnGround)
			vy = -12.0f;


		fStateTick += 0.8f;
	}
	vx += fDirectionX * 5.0f * fElapsedTime;
}