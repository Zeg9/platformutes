#include "Environment.h"
#include "Tile.h"
#include "Sprite.h"
#include "Badguy.h"

#include <iostream>

Badguy::Badguy(std::string _img, int x, int y,
	std::map<std::string, std::string> &_scripts) :
		ScriptedSprite(_img, x,y, _scripts)
{}

void Badguy::step()
{
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
	ScriptedSprite::step();
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
	return ENV.lvl.get((p.x+getSize().x)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+2).isSolid()
		&& !(ENV.lvl.get((p.x+getSize().x)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+1).isSolid()
			|| ENV.lvl.get((p.x+getSize().x)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT).isSolid());
}

