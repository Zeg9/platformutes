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

#include <list>
#include <stack>
#include <algorithm>
#include "Sprite.h"
#include "Environment.h"
#include "Script.h"
#include "Tile.h"
#include "Player.h"

#include <iostream>

Player::Player() : Sprite("common.character", 32, 64) {}

void Player::doHit()
{
	SPRITE_FOR_CONTACT_TILES
	{
		vec2 t = vec2(x,y).toTile();
		runScript(ENV.lvl.get(t).getScripts()["on_hit"],0,t);
	}
	for (std::list<Sprite*>::iterator i = ENV.sprites.begin();
		i != ENV.sprites.end(); i++)
	{
		vec2 t = (*i)->getPos();
		if (hasContact(t))
			(*i)->hit();
	}
}

void Player::die()
{
	ENV.lvl.reload();
}

void Player::step()
{
	if (!physics) { Sprite::step(); return; }
	// check if player shall die
	if (PPOS.y > ENV.lvl.getHeight()*TILE_HEIGHT)
		die();
	// run tiles on_contact scripts
	// TODO maybe this needs some more work...
	std::stack<vec2> tokeep;
	SPRITE_FOR_CONTACT_TILES
	{
		vec2 t = vec2(x,y).toTile();
		bool c = false;
		for (std::list<vec2>::iterator i = contact.begin(); i != contact.end(); i++)
			if ((*i) == t)
				c = true;
		if (!c && !ENV.lvl.get(t.x,t.y).isSolid())
			runScript(ENV.lvl.get(t.x,t.y).getScripts()["on_contact"],0,t);
		tokeep.push(t);
	}
	contact.clear();
	while (!tokeep.empty())
	{
		contact.push_back(tokeep.top());
		tokeep.pop();
	}
	Sprite::step();
}

