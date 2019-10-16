//Define Statements for using PGE + Extensions with an OOP structure
// and main()


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"


#include "Jerry_Engine.h"
int main()
{
	Platformer game;
	if (game.Construct(264, 242, 4, 4))
	//if (game.Construct(264, 242, 4, 4, false, true)) //VSync
	//if (game.Construct(264, 900, 4, 4))

		game.Start();

	return 0;
}