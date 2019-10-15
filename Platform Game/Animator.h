#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"
#include <iostream>

class cAnimator
{
public:
	std::map<std::string, std::vector<olc::Sprite*>> mapStates;   //Maybe put this as private so that we can only access through error checking methods?

public:
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.1f; //Default
	float fTimeCounter = 0.0f;
	bool bAnimateOnceState = false;
	bool bCompletedAnimation = false;

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
			if (mapStates[s].size() != 0) //Check that that state has been initialized (prevent access to uninitialized vector (null/garbage data - error))
				sCurrentState = s;
			else
				std::cerr << "Error: could not change animation state to " << s << ". That state does not exist.\n";

			fTimeCounter = 0;
			nCurrentFrame = 0;
			bAnimateOnceState = false;
		}
	}

	void ChangeState(std::string s, bool b)
	{
		if (s != sCurrentState)
		{
			if (mapStates[s].size() != 0) //Check that that state has been initialized (prevent access to uninitialized vector (null/garbage data - error))
				sCurrentState = s;
			else
				std::cerr << "Error: could not change animation state to " << s << ". That state does not exist.\n";

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
					{
						nCurrentFrame = nCurrentFrame - 1;
						bCompletedAnimation = true;
					}
					else
						nCurrentFrame = 0;
				}
			}
	}

	void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D& t)
	{
		//First, ensure we do not try to access a vector that has not been initialized - that's a fatal error
		if (mapStates.size() > 0) 
		{
			if (mapStates[sCurrentState].size() > 0)
				olc::GFX2D::DrawSprite(mapStates[sCurrentState][nCurrentFrame], t);
		}
	}
};