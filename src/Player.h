#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Sprite.h"

class Player : public Sprite
{
	public:
		Player();
		virtual void die();
};

#endif//__PLAYER_H__
