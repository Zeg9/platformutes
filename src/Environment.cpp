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

#include <algorithm>
#include "Sprite.h"
#include "Player.h"
#include "Environment.h"

Environment::Environment() :
	allowSprites(true),
	player(new Player())
{}
Environment::~Environment()
{
	for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
		delete (*i);
	delete player;
}

void Environment::reset()
{
	if (allowSprites) for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
			s2r.push((*i));
	PLAYER->setPos(32,64);
	PLAYER->setVel(0,0);
	PLAYER->setImage("common.character");
}

void Environment::addSprite(Sprite *s) {
	if (allowSprites)
		sprites.push_back(s);
}
void Environment::removeSprite(Sprite *s)
{
	s2r.push(s);
}

void Environment::render()
{
	lvl.render();
	if (!allowSprites) return;
	for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
		(*i)->render();
	player->render();
}

void Environment::step()
{
	if (allowSprites) for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
			(*i)->step();
	player->step();
	if (allowSprites) while (!s2r.empty())
	{
		if (std::find(sprites.begin(), sprites.end(), s2r.top()) == sprites.end())
		{
			s2r.pop();
			continue;
		}
		sprites.remove(s2r.top());
		delete s2r.top();
		s2r.pop();
	}
}

Environment &getEnvironment()
{
	static Environment environment;
	return environment;
}

