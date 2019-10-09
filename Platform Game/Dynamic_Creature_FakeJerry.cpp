#include "Dynamics.h"

cDynamic_Creature_FakeJerry::cDynamic_Creature_FakeJerry() : cDynamic_Creature("Fake Jerry")
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	fStateTick = 2.0f;
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
		if (fDistance < 8.0f && fDistance != 0) // Avoid divide by 0 error that freezes program (when we had vx = (fTargetX / fDistance) * 2.0f;)
		{
			if (fTargetX < 0.0f)
				fFaceDir = LEFT; // -1.0f
			else
				fFaceDir = RIGHT; // 1.0f
		}
		else
		{
			vx = 0;
		}

		if (fTargetY < 0 && bObjectOnGround)
			vy = -12.0f;


		fStateTick += 0.8f;
	}
	//vx += fFaceDir * 5.0f * fElapsedTime;
	vx += fFaceDir * (bObjectOnGround && vx >= 0 ? 10.0f : bObjectOnGround ? 5.0f : 8.0f)* fElapsedTime;
}