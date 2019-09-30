#include "Dynamic_Creature.h"

cDynamic_Creature::cDynamic_Creature(std::string n) : cDynamic(n)
{
	nHealth = 10;
	nMaxHealth = 10;
	bPlayerOnGround = false;
	fFaceDir = +1.0f;
	bSquat = false;

	animations.mapStates["idle"].push_back(Assets::get().GetSprite("Jerry_Idle"));

	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_1"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_2"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_3"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_4"));

	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_1"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_2"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_3"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_4"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_5"));

	animations.mapStates["squat"].push_back(Assets::get().GetSprite("Jerry_Squat"));

	animations.mapStates["jump"].push_back(Assets::get().GetSprite("Jerry_Jump_1"));
	animations.mapStates["jump"].push_back(Assets::get().GetSprite("Jerry_Jump_2"));


	animations.mapStates["fall"].push_back(Assets::get().GetSprite("Jerry_Fall"));
}

void cDynamic_Creature::DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) //Screen space offset, since we already have position as data member
{
	olc::GFX2D::Transform2D t;
	t.Translate(oy, oy);
	animations.DrawSelf(pge, t);
}

void cDynamic_Creature::Update(float fElapsedTime)
{
	if (bPlayerOnGround)
	{
		if (fabs(vx) == 0.0f) //Clamp vel to 0 if near 0 to allow player to stop
		{
			animations.ChangeState("idle");

			if (fabs(vx) > 0.01f) //LITERALLY all this does is allow him to "dance" by trying to move when up against a wall, probably shouldn't have this here but I think it's funny
				animations.ChangeState("run");
		}
		else if (fFaceDir == +1.0f && vx < 0 || fFaceDir == -1.0f && vx > 0) //Just changed direction but still moving the opposite way -> braking
		{
			animations.ChangeState("brake");
		}
		else
		{
			animations.ChangeState("run");
		}

		if (bSquat) //This is changed in the Input Handling section
			animations.ChangeState("squat");
	}
	else
	{
		if (vy < 0)
		{
			animations.ChangeState("jump", true);
		}
		else
		{
			animations.ChangeState("fall");
		}
	}


	animations.Update(fElapsedTime); //Update animation frame
}
