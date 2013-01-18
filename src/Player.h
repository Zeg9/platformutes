#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <list>
#include "tools.h"
#include "Sprite.h"

class Player : public Sprite
{
	public:
		Player();
		virtual void doHit();
		virtual void die();
		virtual void step();
	private:
		std::list<vec2> contact;
};

#endif//__PLAYER_H__
