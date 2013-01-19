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

#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Sound.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "game.h"
#include "editor.h"
#include "tools.h"

#include "ResourceMgr.h"

int main(int argc, char ** argv)
{
	Device &d = getDevice();
	getSoundManager(); // init sound system
	bool menuaccept=false;
	std::vector<std::string> menulist;
	menulist.push_back("game");
	menulist.push_back("editor");
	menulist.push_back("options");
	menulist.push_back("quit");
	int menuselect=0;
	SDL_Event e;
	while (d.run())
	{
		d.drawImage(getResourceMgr().getImage("common/title"));
		int starty = d.getHeight()/2 - (40*menulist.size())/2+4;
		for (unsigned int i = 0; i < menulist.size(); i++)
		{
			d.drawImage(getResourceMgr().getImage("common/menu/"+menulist[i]),
				d.getWidth()/2-128, starty+i*40);
			if (menuselect == (int)i)
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
							menuaccept = true;
							break;
						case SDLK_DOWN:
							if (menuselect < (int)menulist.size()-1)
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
				/*case SDL_MOUSEMOTION:
					menuselect = e.motion.y/32%menulist.size();
					break;*/
			}
		}
		if (menuaccept)
		{
			menuaccept = false;
			if (menulist[menuselect] == "game")
				startGame();
			else if (menulist[menuselect] == "editor")
				startEditor();
			else if (menulist[menuselect] == "quit")
				getDevice().quit();
		}
	}
	// Bye !
	getDevice().close();
	return 0;
}

