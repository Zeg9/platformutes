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

#include <string>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "tools.h"
#include "ResourceMgr.h"
#include "Config.h"
#include "Video.h"

Image::Image(std::string filename)
{
	surf = IMG_Load(filename.c_str());
	if (!surf)
		throw std::runtime_error("Couldn't load image: "+filename);
}
Image::Image(SDL_Surface *_surf) : surf(_surf)
{}
Image::~Image()
{
	SDL_FreeSurface(surf);
}

int Image::getWidth()
{
	return surf->w;
}

int Image::getHeight()
{
	return surf->h;
}

vec2 Image::getSize()
{
	return vec2(getWidth(), getHeight());
}


Device::Device() : lastticks(0), fullscreen(false), cursor(true), done(false)
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_ShowCursor(false);
	SDL_WM_SetCaption("Platformutes",0);
	fullscreen = getConfig().getBool("fullscreen");
	if (fullscreen)
		screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,
			VIDEO_BPP,VIDEO_SDL_FLAGS|SDL_FULLSCREEN);
	else
		screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,
			VIDEO_BPP,VIDEO_SDL_FLAGS);
}
Device::~Device()
{
	TTF_Quit();
	SDL_Quit();
}

int Device::getWidth() { return screen->w; }
int Device::getHeight() { return screen->h; }

void Device::toggleFullscreen()
{
	fullscreen = !fullscreen;
	getConfig().setBool("fullscreen",fullscreen);
	if (fullscreen)
		screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,
			VIDEO_BPP,VIDEO_SDL_FLAGS|SDL_FULLSCREEN);
	else
		screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,
			VIDEO_BPP,VIDEO_SDL_FLAGS);
}
void Device::showCursor(bool show)
{
	cursor = show;
}

void Device::drawImage(Image *i, int x, int y)
{
	SDL_Rect dstrect;
	dstrect.x = x; dstrect.y = y;
	dstrect.w = dstrect.h = 0;
	SDL_BlitSurface(i->surf, 0, screen, &dstrect);
}
void Device::drawImage(Image *i, vec2 p)
{
	drawImage(i, p.x, p.y);
}
void Device::drawImage(Image *i, int x, int y, int cx, int cy, int cw, int ch)
{
	SDL_Rect dstrect;
	dstrect.x = x; dstrect.y = y;
	dstrect.w = dstrect.h = 0;
	SDL_Rect srcrect;
	srcrect.x = cx; srcrect.y = cy;
	srcrect.w = cw; srcrect.h = ch;
	SDL_BlitSurface(i->surf, &srcrect, screen, &dstrect);
}

void Device::render()
{
	if (cursor)
	{
		int x, y; SDL_GetMouseState(&x, &y);
		drawImage(getResourceMgr().getImage("common/cursor"), x, y);
	}
	SDL_Flip(screen);
}

void Device::clear()
{
	SDL_FillRect(screen,0,SDL_MapRGB(screen->format,0,0,0));
}

bool Device::run()
{
	int ticks = SDL_GetTicks();
	if (ticks - lastticks < TBF)
		SDL_Delay(TBF-(ticks-lastticks));
	SDL_WM_SetCaption(("Platformutes [FPS="+
		tostring(1000/(int)(SDL_GetTicks()-lastticks))+"]").c_str(),
		"Platformutes");
	lastticks = ticks;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_F4:
						if (e.key.keysym.mod & KMOD_ALT)
							done = true;
						break;
					case SDLK_F11:
						toggleFullscreen();
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		eventStack.push(e);
	}
	return !done;
}

bool Device::hasEvent()
{
	return !eventStack.empty();
}

SDL_Event Device::nextEvent()
{
	SDL_Event e = eventStack.top();
	eventStack.pop();
	return e;
}

void Device::quit()
{
	done = true;
}
void Device::close()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}


Device &getDevice()
{
	static Device device;
	return device;
}

