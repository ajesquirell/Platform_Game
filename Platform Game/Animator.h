#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class cAnimator
{
public:
	std::map<std::string, std::vector<olc::Sprite*>> mapStates;

public:
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.1f; //Default
	float fTimeCounter = 0.0f;
	bool bAnimateOnceState = false;

	cAnimator()
	{

	}
	cAnimator(float fTimeBetweenFrames) //Custom Constructor
	{
		this->fTimeBetweenFrames = fTimeBetweenFrames;
	}

	void ChangeState(std::string s)
	{
		if (s != sCurrentState)
		{
			sCurrentState = s;
			fTimeCounter = 0;
			nCurrentFrame = 0;
			bAnimateOnceState = false;
		}
	}

	void ChangeState(std::string s, bool b)
	{
		if (s != sCurrentState)
		{
			sCurrentState = s;
			fTimeCounter = 0;
			nCurrentFrame = 0;
			bAnimateOnceState = b;
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
			{
				if (bAnimateOnceState)
					nCurrentFrame = nCurrentFrame - 1;
				else
					nCurrentFrame = 0;
			}

		}
	}

	void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
	{
		olc::GFX2D::DrawSprite(mapStates[sCurrentState][nCurrentFrame], t);
	}
};