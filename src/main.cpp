#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Sound.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "editor.h"
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
	bool menu=true;
	std::vector<std::string> menulist;
	menulist.push_back("game");
	menulist.push_back("editor");
	menulist.push_back("options");
	int menuselect=0;
	SDL_Event e;
	while (d.run() && menu)
	{
		d.drawImage(getResourceMgr().getImage("common/title"));
		int starty = d.getHeight()/2 - (40*menulist.size())/2+4;
		for (unsigned int i = 0; i < menulist.size(); i++)
		{
			d.drawImage(getResourceMgr().getImage("common/menu/"+menulist[i]),
				d.getWidth()/2-128, starty+i*40);
			if (menuselect == i)
				d.drawImage(getResourceMgr().getImage("common/menu/select"),
					d.getWidth()/2-128, starty+i*40);
		}
		d.render();
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch (e.type)
			{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_RETURN:
						case SDLK_KP_ENTER:
							menu = false;
							break;
						case SDLK_DOWN:
							if (menuselect < menulist.size()-1)
								menuselect++;
							else menuselect = 0;
							break;
						case SDLK_UP:
							if (menuselect > 0)
								menuselect--;
							else menuselect = menulist.size()-1;
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					menuselect = e.motion.y/32%menulist.size();
					break;
			}
		}
	}
	Level &lvl = getEnvironment().lvl;
	lvl.load(lvlname);
	if (menulist[menuselect] == "editor") startEditor();
	// TODO move this ↓ to game.cpp
	bool pause;
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
	// Bye !
	getDevice().close();
	return 0;
}

