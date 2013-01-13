#ifndef __BADGUY_H__
#define __BADGUY_H__

#include "Sprite.h"

class Badguy : public ScriptedSprite
{
	public:
		Badguy(std::string _img, int x, int y,
			std::map<std::string, std::string> &_scripts);
		virtual void step();
	private:
		bool isGoingLeft();
		bool isGoingRight();
		bool shouldGoLeft();
		bool shouldGoRight();
};

#endif//__BADGUYS_H__
