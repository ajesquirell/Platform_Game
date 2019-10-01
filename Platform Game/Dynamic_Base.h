#pragma once

#include "olcPixelGameEngine.h"

class cDynamic //Pure Virtual
{
public:
	cDynamic(std::string n);
	~cDynamic();

public:
	float px, py;
	float vx, vy;
	bool bSolidVsMap;
	bool bSolidVsDynamic;
	bool bFriendly;
	std::string sName;

	bool bObjectOnGround; //Probably only for Dynamic Creatures and Items

public:
	virtual void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) = 0; //Screen space offset, since we already have position as data member
	virtual void Update(float fElapsedTime) {}

};

