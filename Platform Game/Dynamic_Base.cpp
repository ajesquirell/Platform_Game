#include "Dynamics.h"
using namespace std;

Platformer* cDynamic::g_engine = nullptr;
cScriptProcessor* cDynamic::g_script = nullptr;

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
	bObjectOnGround = false;
}

cDynamic::~cDynamic()
{
}
