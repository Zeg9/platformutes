#include "Environment.h"
#include "Tile.h"
#include "Badguys.h"

Mummy::Mummy(int x, int y) : Sprite("mummy", x,y,32,64)
{}

void Mummy::step()
{
	if (p.x+s.x >= PPOS.x
	 && p.x <= PPOS.x + PSIZE.x-1
	 && p.y+s.y >= PPOS.y
	 && p.y <= PPOS.y + PSIZE.y-1)
		PLAYER->setImage("mummy");
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

bool Mummy::isGoingLeft()
{
	return v.x < 0;
}
bool Mummy::isGoingRight()
{
	return v.x > 0;
}

bool Mummy::shouldGoLeft()
{
	return ENV.lvl.get(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+2).isSolid()
		&& !(ENV.lvl.get((p.x-1)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT+1).isSolid()
			|| ENV.lvl.get((p.x-1)/BLOCK_WIDTH,p.y/BLOCK_HEIGHT).isSolid());
}
bool Mummy::shouldGoRight()
{
	return ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT+2).isSolid()
		&& !(ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT+1).isSolid()
			|| ENV.lvl.get(p.x/BLOCK_WIDTH+1,p.y/BLOCK_HEIGHT).isSolid());
}

