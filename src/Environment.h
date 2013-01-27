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

#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <list>
#include <stack>
#include "Level.h"
#include "Sprite.h"
#include "Player.h"
#include "tools.h"

#define ENV getEnvironment()
#define PLAYER ENV.player
#define PPOS PLAYER->getPos()
#define PVEL PLAYER->getVel()
#define PSIZE PLAYER->getSize()

class Environment
{
	friend Environment &getEnvironment();
	friend class Player;
	public:
		Level lvl;
		void reset();
		bool allowSprites;
		void addSprite(Sprite *s);
		void removeSprite(Sprite *s);
		Player *player;
		void render();
		void step();
		void s2rstep();
		void quit();
	private:
		std::list<Sprite*> sprites;
		std::stack<Sprite*> s2r; // sprites to remove
		Environment();
		~Environment();
};

Environment &getEnvironment();

#endif//__ENVIRONMENT_H__
