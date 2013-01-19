#include "Video.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "tools.h"

#include "game.h"

void startGame()
{
	Device &d = getDevice();
	PLAYER->enablePhysics(true);
	ENV.allowSprites = true;
	Level &lvl = ENV.lvl;
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
				int dx = x-PPOS.x%BLOCK_WIDTH, dy = y-PPOS.y%BLOCK_HEIGHT;
				if (PPOS.x < d.getWidth()/2 || PPOS.x > lvl.getWidth()*BLOCK_WIDTH-d.getWidth()/2)
					dx = x;
				if (PPOS.y < d.getHeight()/2 || PPOS.y+BLOCK_HEIGHT > lvl.getHeight()*BLOCK_WIDTH-d.getHeight()/2)
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
