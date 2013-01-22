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

#include "Video.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "tools.h"

#include "game.h"

void startGame(bool loadLevel)
{
	Device &d = getDevice();
	d.showCursor(false);
	PLAYER->enablePhysics(true);
	ENV.allowSprites = true;
	Level &lvl = ENV.lvl;
	if (loadLevel)
		lvl.load(FIRST_LEVEL);
	SDL_Event e;
	bool pause(false), end(false);
	while (d.run() && !end)
	{
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-PPOS.x%TILE_WIDTH, dy = y-PPOS.y%TILE_HEIGHT;
				if (PPOS.x < d.getWidth()/2 || PPOS.x > lvl.getWidth()*TILE_WIDTH-d.getWidth()/2)
					dx = x;
				if (PPOS.y < d.getHeight()/2 || PPOS.y+TILE_HEIGHT > lvl.getHeight()*TILE_WIDTH-d.getHeight()/2)
					dy = y;
				d.drawImage(lvl.getBackground(),dx,dy);
			}
		}
		// update everything
		if (!pause) getEnvironment().step();
		// render
		getEnvironment().render();
		// we're done, let's render
		d.render();
		// handle events
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							end = true;
							break;
						case SDLK_RETURN:
							pause = !pause;
							break;
						default:
							break;
					}
					if (!pause) switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							PLAYER->setVel(-2,PLAYER->getVel().y);
							PLAYER->setState("stand_l");
							break;
						case SDLK_RIGHT:
							PLAYER->setVel(2,PLAYER->getVel().y);
							PLAYER->setState("stand_r");
							break;
						case SDLK_SPACE:
							PLAYER->jump();
							break;
						case SDLK_LCTRL:
						case SDLK_UP:
							PLAYER->doHit();
							break;
						case SDLK_BACKSPACE:
							getEnvironment().player->die();
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							if (PLAYER->getVel().x < 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						case SDLK_RIGHT:
							if (PLAYER->getVel().x > 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}
}
