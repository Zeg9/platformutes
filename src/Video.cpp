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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "tools.h"
#include "ResourceMgr.h"
#include "Config.h"
#include "Video.h"

Image::Image(std::string filename)
{
	SDL_Surface *surf = IMG_Load(filename.c_str());
	w = surf->w; h = surf->h;
	if (!surf)
		throw std::runtime_error("Couldn't load image: "+filename);
	tex = SDL_CreateTextureFromSurface(getDevice().renderer, surf);
	SDL_FreeSurface(surf);
}
Image::Image(SDL_Surface *surf)
{
	w = surf->w; h = surf->h;
	tex = SDL_CreateTextureFromSurface(getDevice().renderer, surf);
	SDL_FreeSurface(surf);
}
Image::Image(const char **xpm)
{
	SDL_Surface *surf = IMG_ReadXPMFromArray(const_cast<char**>(xpm));
	w = surf->w; h = surf->h;
	if (!surf)
		throw std::runtime_error("Couldn't load xpm");
	tex = SDL_CreateTextureFromSurface(getDevice().renderer, surf);
	SDL_FreeSurface(surf);
}
Image::~Image()
{
	SDL_DestroyTexture(tex);
}

int Image::getWidth()
{
	return w;
}

int Image::getHeight()
{
	return h;
}

vec2 Image::getSize()
{
	return vec2(getWidth(), getHeight());
}

void Image::setAlpha(Uint8 a) // FIXME: remove me, I'm obsolete
{
	//SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, a);
}

Device::Device() : lastticks(0), cfps(0), lfps(0),
	fullscreen(false), cursor(true), done(false)
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_ShowCursor(false);
	fullscreen = getConfig().getBool("fullscreen");
	int width = getConfig().getInt("width"),
	    height = getConfig().getInt("height");
	if (width==0) {
		width = VIDEO_WIDTH;
		getConfig().setInt("width", width);
	}
	if (height==0) {
		height = VIDEO_HEIGHT;
		getConfig().setInt("height", height);
	}
	window = SDL_CreateWindow("Platformutes",
	                          SDL_WINDOWPOS_UNDEFINED,
	                          SDL_WINDOWPOS_UNDEFINED,
	                          0,0,
	                          SDL_WINDOW_FULLSCREEN_DESKTOP);


	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");  // keep pixelated
	SDL_RenderSetLogicalSize(renderer, 640, 480);
	// FIXME: get fullscreen-toggle back
}
Device::~Device()
{
	TTF_Quit();
	SDL_Quit(); //FIXME destroy window and renderer? maybe?
}

int Device::getWidth() { return VIDEO_WIDTH; }
int Device::getHeight() { return VIDEO_HEIGHT; }

void Device::toggleFullscreen()
{
	// FIXME: repair meh
	/*fullscreen = !fullscreen;
	getConfig().setBool("fullscreen",fullscreen);*/
}
void Device::showCursor(bool show)
{
	cursor = show;
}

void Device::drawImage(Image *i, int x, int y)
{
	SDL_Rect dstrect;
	dstrect.x = x; dstrect.y = y;
	dstrect.w = i->getWidth(); dstrect.h = i->getHeight();
	SDL_RenderCopy(renderer, i->tex, 0, &dstrect);
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
	SDL_RenderCopy(renderer, i->tex, &srcrect, &dstrect);
}

void Device::render()
{
	if (cursor) try
	{
		int x, y; SDL_GetMouseState(&x, &y);
		drawImage(getResourceMgr().getImage("common/cursor"), x, y);
	} catch(...) {
		SDL_ShowCursor(true);
		cursor = false;
	}
	SDL_RenderPresent(renderer);
	int ticks = SDL_GetTicks();
	if (ticks/1000 > lastticks/1000)
	{
		lfps = cfps;
		cfps = 0;
	}
	cfps ++;
}

void Device::clear()
{
	SDL_RenderClear(renderer);
}

bool Device::run()
{
	int ticks = SDL_GetTicks();
	if (ticks - lastticks < TBF)
		SDL_Delay(TBF-(ticks-lastticks));
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

int Device::getDTime()
{
	return SDL_GetTicks()-lastticks;
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

