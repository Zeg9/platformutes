/*
 Platformutes
 Copyright (C) 2012-2013 Zeg9 <dazeg9@gmail.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	return ENV.lvl.get(p.x/TILE_WIDTH,p.y/TILE_HEIGHT+2)->isSolid()
		&& !(ENV.lvl.get((p.x-1)/TILE_WIDTH,p.y/TILE_HEIGHT+1)->isSolid()
			|| ENV.lvl.get((p.x-1)/TILE_WIDTH,p.y/TILE_HEIGHT)->isSolid());
}
bool Badguy::shouldGoRight()
{
	return ENV.lvl.get((p.x+getSize().x)/TILE_WIDTH,p.y/TILE_HEIGHT+2)->isSolid()
		&& !(ENV.lvl.get((p.x+getSize().x)/TILE_WIDTH,p.y/TILE_HEIGHT+1)->isSolid()
			|| ENV.lvl.get((p.x+getSize().x)/TILE_WIDTH,p.y/TILE_HEIGHT)->isSolid());
}

