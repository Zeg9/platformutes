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
#include "Video.h"
#include "Font.h"
#include "Sound.h"
#include "Menu.h"
#include "Config.h"
#include "game.h"
#include "editor.h"
#include "tools.h"

#include "ResourceMgr.h"

void mainMenu()
{
	Device &d=getDevice();
	SoundManager &s = getSoundManager();
	d.showCursor(false);
	d.clear();
	{
		Image *i = getResourceMgr().getImage("common/pegarprods");
		d.drawImage(i,
			d.getWidth()/2-i->getWidth()/2,
			d.getHeight()/2-i->getHeight()/2);
	}
	d.render();
	s.playSound(getResourceMgr().getSound("common/pegarprods"));
	d.run();
	SDL_Delay(2000);
	d.run();
	while (d.hasEvent())
		d.nextEvent();
	d.showCursor(true);
	Menu m;
	m.add(MenuEntry("game","Start game"));
	m.add(MenuEntry("editor","Level editor"));
	m.add(MenuEntry("erase","Erase my progress"));
	m.add(MenuEntry("fullscreen","Toggle fullscreen"));
	m.add(MenuEntry("quit","Exit :("));
	int menuselect=0;
	SDL_Event e;
	while (d.run())
	{
		d.clear();
		{
			Image *i = getResourceMgr().getImage("common/title");
			d.drawImage(i,
				d.getWidth()/2-i->getWidth()/2,
				d.getHeight()/2-i->getHeight()/2);
		}
		int s(-1);
		m.render();
		d.render();
		while(d.hasEvent())
		{
			e = d.nextEvent();
			s = m.event(e);
		}
		if (s >= 0)
		{
			MenuEntry &entry = m.get();
			s = -1;
			if (entry.name == "game")
				startGame();
			else if (entry.name == "editor")
				LevelEditor();
			else if (entry.name == "erase")
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
			else if (entry.name == "fullscreen")
				d.toggleFullscreen();
			else if (entry.name == "quit")
				d.quit();
			getConfig().setBool("_ingame",false);
			d.showCursor(true);
		}
	}
}

int main(int argc, char ** argv)
{
	std::cout <<
	" _____   _       _____   _____   _____   _____   _____ \n"
	"|  _  | | |     |  _  | |_   _| |  ___| |  _  | |  _  |\n"
	"| |_| | | |     | |_| |   | |   | |_    | | | | | |_| |\n"
	"|  ___| | |     |  _  |   | |   |  _|   | | | | |  _  |\n"
	"| |     | |___  | | | |   | |   | |     | |_| | | || | \n"
	"|_|     |_____| |_| |_|   |_|   |_|     |_____| |_| |_|\n" //TODO finish that lol
	"         _   _   _   _   _____   _____   _____\n"
	"        | |_| | | | | | |_   _| |  ___| |  ___|\n"
	"        |  _  | | | | |   | |   | |_    | |___\n"
	"        | | | | | | | |   | |   |  _|   |___  |\n"
	"        | | | | | |_| |   | |   | |___   ___| |\n"
	"        |_| |_| |_____|   |_|   |_____| |_____|\n"
	"                                                    0.1\n"
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

