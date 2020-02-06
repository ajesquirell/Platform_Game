#include "Dynamics.h"
#include "Items.h"
#include "Assets.h" //One we want to change to match how he does his dependencies


cDynamic_Creature_FakeJerry::cDynamic_Creature_FakeJerry() : cDynamic_Creature("Fake Jerry")
{
	bFriendly = false;
	nHealth = 10;
	nHealthMax = 10;
	fStateTick = 2.0f;

	pEquipedWeapon = (cWeapon*)Assets::get().GetItem("Basic Sword");
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

			if (fDistance < 1.5f)
				PerformAttack();
		}
		else
		{
			vx = 0;
		}

		if (fTargetY < 0 && bObjectOnGround) //Jump
			vy = -12.0f;


		fStateTick += 0.8f;
	}
	//vx += fFaceDir * 5.0f * fElapsedTime;
	vx += fFaceDir * (bObjectOnGround && abs(vx) >= 0 ? 10.0f : bObjectOnGround ? 5.0f : 8.0f)* fElapsedTime;
}

void cDynamic_Creature_FakeJerry::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	pEquipedWeapon->OnUse(this);
}