#include "Environment.h"
#include "Tile.h"
#include "Script.h"
#include "Badguy.h"

#include <iostream>

Badguy::Badguy(std::string _img, int x, int y,
	std::map<std::string, std::string> &_scripts) :
		Sprite(_img, x,y), scripts(_scripts)
{}

void Badguy::step()
{
	if (p.x+getSize().x >= PPOS.x
	 && p.x <= PPOS.x + PSIZE.x-1
	 && p.y+getSize().y >= PPOS.y
	 && p.y <= PPOS.y + PSIZE.y-1)
	{
		if (!hasContact)
		{
			runScript(scripts["on_contact"], this);
			hasContact = true;
		}
	}
	else if (hasContact)
		hasContact = false;
	if (!(isGoingRight() && shouldGoRight()) && shouldGoLeft())
	{
		setState("stand_l");
		setVel(-1,0);
	}
	if (!(isGoingLeft() && shouldGoLeft()) && shouldGoRight())
	{
		setState("stand_r");
		setVel(1,0);
	}
	Sprite::step();
}

bool Badguy::isGoingLeft()
{
	return v.x < 0;
}
bool Badguy::isGoingRight()
{
	return v.x > 0;
}

bool Badguy::shouldGoLeft()
{
	return ENV.lvl.get(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+2).isSolid()
		&& !(ENV.lvl.get((p.x-1)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+1).isSolid()
			|| ENV.lvl.get((p.x-1)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT).isSolid());
}
bool Badguy::shouldGoRight()
{
	return ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT+2).isSolid()
		&& !(ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT+1).isSolid()
			|| ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT).isSolid());
}

