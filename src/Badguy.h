#ifndef __BADGUY_H__
#define __BADGUY_H__

#include <map>
#include "Sprite.h"

class Badguy : public Sprite
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
		std::map<std::string, std::string> scripts;
		bool hasContact;
};

#endif//__BADGUYS_H__
