#ifndef __BADGUYS_H__
#define __BADGUYS_H__

#include "Sprite.h"

class Mummy : public Sprite
{
	public:
		Mummy(int x, int y);
		virtual void step();
	private:
		bool isGoingLeft();
		bool isGoingRight();
		bool shouldGoLeft();
		bool shouldGoRight();
};

#endif//__BADGUYS_H__
