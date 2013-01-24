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
#include "Font.h"
#include "Sound.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "Config.h"
#include "game.h"
#include "editor.h"
#include "tools.h"

#include "ResourceMgr.h"

struct menuentry {
	menuentry(std::string r, std::string d) :
		real(r), display(d) {}
	std::string real;
	std::string display;
};

void mainMenu()
{
	Device &d=getDevice();
	bool menuaccept=false;
	std::vector<menuentry> menulist;
	menulist.push_back(menuentry("game","Start game"));
	menulist.push_back(menuentry("editor","Level editor"));
	menulist.push_back(menuentry("erase","Erase my progress"));
	menulist.push_back(menuentry("fullscreen","Toggle fullscreen"));
	menulist.push_back(menuentry("quit","Exit :("));
	int menuselect=0;
	SDL_Event e;
	while (d.run())
	{
		d.drawImage(getResourceMgr().getImage("common/title"));
		int menux = d.getWidth()/2;
		int menuy = d.getHeight()/2 - (40*menulist.size())/2;
		for (unsigned int i = 0; i < menulist.size(); i++)
		{
			uint8_t grey = 128;
			if (menuselect == (int)i)
				grey = 255;
			getResourceMgr().getFont("common/FreeSans&16")->render(
				menulist[i].display, grey,grey,grey,
				menux, menuy+i*40+16,
				ALIGN_CENTER, ALIGN_MIDDLE);
			// TODO: Store the generated text images somewhere
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
				case SDL_MOUSEMOTION:
					if (e.motion.y > menuy && e.motion.y < menuy+menulist.size()*40
					 && e.motion.x > menux-128 && e.motion.x < menux+128)
					{
						int n = (e.motion.y-menuy)/40;
						if (n >= 0 && n < (int)menulist.size())
							menuselect = n;
					} else menuselect = -1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_LEFT)
						menuaccept = true;
					break;
				default:
					break;
			}
		}
		if (menuaccept)
		{
			menuaccept = false;
			if (menuselect == -1) continue;
			if (menulist[menuselect].real == "game")
				startGame();
			else if (menulist[menuselect].real == "editor")
				startEditor();
			else if (menulist[menuselect].real == "erase")
			{
				Font*font = getResourceMgr().getFont("common/FreeMono&20");
				bool done=false;
				while(d.run() &&!done)
				{
					d.clear();
					font->render(
					"Press Y to erase your progress\n"
					"Warning: you can't revert this action !\n"
					"Press any other key to cancel.\n",
					255,255,255,d.getWidth()/2,d.getHeight()/2,ALIGN_CENTER,ALIGN_MIDDLE);
					d.render();
					while (d.hasEvent())
					{
						e = d.nextEvent();
						if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_y)
						{
							getConfig().setString("current_level","");
							while (d.run() && !done)
							{
								d.clear();
								font->render(
								"Progress deleted.\n"
								"You'll go back to first level.\n"
								"Press any key to continue.\n",
								255,255,255,d.getWidth()/2,d.getHeight()/2,ALIGN_CENTER,ALIGN_MIDDLE);
								d.render();
								while (d.hasEvent())
								{
									e = d.nextEvent();
									if (e.type == SDL_KEYDOWN
									 || e.type == SDL_MOUSEBUTTONUP)
										done = true;
								}
							}
						}
						else if (e.type == SDL_KEYDOWN
						      || e.type == SDL_MOUSEBUTTONUP)
						{
							while (d.run() && !done)
							{
								d.clear();
								font->render(
								"Canceled.\n"
								"Press any key to continue.\n",
								255,255,255,d.getWidth()/2,d.getHeight()/2,ALIGN_CENTER,ALIGN_MIDDLE);
								d.render();
								while (d.hasEvent())
								{
									e = d.nextEvent();
									if (e.type == SDL_KEYDOWN
									 || e.type == SDL_MOUSEBUTTONUP)
										done = true;
								}
							}
						}
					}
				}
			}
			else if (menulist[menuselect].real == "fullscreen")
				d.toggleFullscreen();
			else if (menulist[menuselect].real == "quit")
				d.quit();
			getConfig().setBool("_ingame",false);
			d.showCursor(true);
		}
	}
}

int main(int argc, char ** argv)
{
	std::cout <<
	" _____   _       _____   _____   _____   _____   _____\n"
	"|  _  | | |     |  _  | |_   _| |  ___| |  _  | |  _  |\n"
	"| |_| | | |     | |_| |   | |   | |_    | | | | | |_| |\n"
	"|  ___| | |     |  _  |   | |   |  _|   | | | | |  _  |\n"
	"| |     | |___  | | | |   | |   | |     | |_| | | || |\n"
	"|_|     |_____| |_| |_|   |_|   |_|     |_____| |_| |_|\n" //TODO finish that lol
	<< std::flush;
	getConfig(); // load settings
	getDevice(); // init video
	getSoundManager(); // init sound system
	try {
		mainMenu();
	} catch (std::exception &e) {
		std::cerr << "There was an error..." << std::endl << e.what() << std::endl;
	}
	getResourceMgr().unloadAll();
	getDevice().close();
	return 0;
}

