#pragma once
#include "Dynamic_Base.h"]
#include "Animator.h"

class Dynamic_Creature : public cDynamic
{
public:
	cDynamic_Creature(std::string n, )

public:
	ChangeState(std::string newState);
private:
	cAnimator spriteAnims; ///Can private members be accessed by the eventual player and enemy classes by using methods of this class like ChagneState??
};

