#include "Dynamics.h"
using namespace std;

cDynamic_Projectile::cDynamic_Projectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, cAnimator animProj, float aggressor_fFaceDir) : cDynamic("projectile")
{
	animProjectile = animProj;
	fFaceDir = aggressor_fFaceDir;
	fDuration = duration;
	px = ox;
	py = oy;
	vx = velx;
	vy = vely;

	bSolidVsDynamic = false;
	bSolidVsMap = true;
	bIsProjectile = true;
	bIsAttackable = false;
	bFriendly = bFriend;
	bGravityApplies = false;
}

void cDynamic_Projectile::DrawSelf(olc::PixelGameEngine* pge, float ox, float oy)
{
	if (fFaceDir == cDynamic_Creature::LEFT)
	{
		utility::DrawInvertedSprite(pge, (px - ox) * 22, (py - oy) * 22, animProjectile.mapStates[animProjectile.sCurrentState][animProjectile.nCurrentFrame]);
	}
	else
	{
		pge->DrawSprite((px - ox) * 22, (py - oy) * 22, animProjectile.mapStates[animProjectile.sCurrentState][animProjectile.nCurrentFrame]);
	}
}

void cDynamic_Projectile::Update(float fElapsedTime, cDynamic* player)
{
	fDuration -= fElapsedTime;
	if (fDuration <= 0.0f)
		bRedundant = true; // To be deleted
}