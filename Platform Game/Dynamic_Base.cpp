#include "Dynamic_Base.h"

using namespace std;

cDynamic::cDynamic(string n)
{
	sName = n;
	px = 0.0f;
	py = 0.0f;
	vx = 0.0f;
	vy = 0.0f;
	bSolidVsMap = true;
	bSolidVsDynamic = true;
	bFriendly = true;
}

cDynamic::~cDynamic()
{
}
