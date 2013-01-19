#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Sound.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "tools.h"

#include "ResourceMgr.h"
#warning remove this include ^

int main(int argc, char ** argv)
{
	std::string lvlname("pyramid/lvl1");
	if (argc == 2)
		lvlname = argv[1];
	Device &d = getDevice();
	getSoundManager(); // init sound mgr
	Level &lvl = getEnvironment().lvl;
	lvl.load(lvlname);
	bool pause;
	unsigned int editor_currenttile(0), editor_fade(BLOCK_WIDTH), editor_fadetime(0); // level editor TODO move this
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
		// level editor (TODO move)
		std::vector<int> tiles = lvl.getTileset()->getValidTiles();
		for (int i = -5; i <= 5; i++)
		{
			d.drawImage(getResourceMgr().getImage("common/editor/slot"),
				d.getWidth()-BLOCK_WIDTH+editor_fade,
				d.getHeight()/2-BLOCK_HEIGHT/2+i*BLOCK_HEIGHT);
			int ci = (int)editor_currenttile + i;
			if (ci >= (int)tiles.size()) continue;
			Tile &t = lvl.getTileset()->get(tiles[ci]);
			if (!t.isAir())
			{
				d.drawImage(t.getImage(),
					d.getWidth()-BLOCK_WIDTH+editor_fade,
					d.getHeight()/2-BLOCK_HEIGHT/2+i*BLOCK_HEIGHT);
			}
		}
		d.drawImage(getResourceMgr().getImage("common/editor/select"),
			d.getWidth()-BLOCK_WIDTH+editor_fade,
			d.getHeight()/2-BLOCK_HEIGHT/2);
		if (SDL_GetTicks() > editor_fadetime && editor_fade < BLOCK_WIDTH)
			editor_fade+=2;
		int x, y;
		SDL_GetMouseState(&x, &y);
		d.drawImage(lvl.getTileset()->get(tiles[editor_currenttile]).getImage(), x-BLOCK_WIDTH/2, y-BLOCK_HEIGHT/2);
		// handle events (TODO maybe this should be moved)
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					vec2 p = getRealPos(vec2(e.button.x, e.button.y));
					switch (e.button.button)
					{
						case SDL_BUTTON_LEFT:
							lvl.set(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT,
								lvl.getTileset()->getValidTiles()[editor_currenttile]);
							break;
						case SDL_BUTTON_RIGHT:
							lvl.set(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT, 0);
							break;
						case SDL_BUTTON_WHEELUP:
							if (editor_currenttile > 0)
								editor_currenttile --;
							editor_fade = 0;
							editor_fadetime = SDL_GetTicks()+1000;
							break;
						case SDL_BUTTON_WHEELDOWN:
							if (editor_currenttile < tiles.size()-1)
								editor_currenttile ++;
							editor_fade = 0;
							editor_fadetime = SDL_GetTicks()+1000;
							break;
						default:
							break;
					}
				} break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
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
		// we're done, let's render
		d.render();
	}
	// Bye !
	getDevice().close();
	return 0;
}

