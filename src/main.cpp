#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Image.h"
#include "Level.h"
#include "Tileset.h"
#include "Sprite.h"
#include "ResourceMgr.h"
#include "Environment.h"
#include "tools.h"

#define BW BLOCK_WIDTH
#define BH BLOCK_HEIGHT

int main(int argc, char ** argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [<level name>]" << std::endl;
		return 1;
	}
	std::string lvlname("pyramid/lvl1");
	if (argc == 2)
		lvlname = argv[1];
	Device &d = getDevice();
	Level &lvl = getEnvironment().lvl;
	lvl.load(lvlname);
	SDL_Event e;
	while (d.run())
	{
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-PPOS.x%BW, dy = y-PPOS.y%BH;
				if (PPOS.x < d.getWidth()/2 || PPOS.x > lvl.getWidth()*BW-d.getWidth()/2)
					dx = x;
				if (PPOS.y < d.getHeight()/2 || PPOS.y+BH > lvl.getHeight()*BW-d.getHeight()/2)
					dy = y;
				d.drawImage(lvl.getBackground(),dx,dy);
			}
		}
		// draw tiles
		// TODO move this ?
		lvl.render();
		// draw sprites and character
		if (
			PPOS.y > lvl.getHeight()*BH ||
			lvl.get(PPOS.x/BW, (PPOS.y-1+BH*2)/BH).getHurt() ||
			lvl.get(PPOS.x/BW+1, (PPOS.y-1+BH*2)/BH).getHurt()
			)
		{ getEnvironment().player->die(); }
		getEnvironment().step();
		getEnvironment().render();
		// handle events (TODO move this too)
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							PLAYER->setVel(-2,PLAYER->getVel().y);
							PLAYER->setState("stand_l");
							break;
						case SDLK_RIGHT:
							PLAYER->setVel(2,PLAYER->getVel().y);
							PLAYER->setState("stand_r");
							break;
						case SDLK_UP:
							if (lvl.get(PPOS.x/BW,PPOS.y/BW+2).isSolid()
							 || lvl.get((PPOS.x-1)/BW+1,PPOS.y/BW+2).isSolid())
								PLAYER->setVel(PLAYER->getVel().x,-10);
							break;
						case SDLK_p:
							std::cout << PPOS.x << ',' << PPOS.y << std::endl;
							break;
						case SDLK_r:
							PLAYER->die();
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
		// we're done, let's render
		d.render();
	}
	// Bye !
	return 0;
}

