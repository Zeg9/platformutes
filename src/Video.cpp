#include <SDL/SDL.h>
#include "Image.h"
#include "Video.h"
#include "tools.h"

Device::Device() : lastticks(0), done(false)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Platformutes",0);
	screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,VIDEO_BPP,VIDEO_SDL_FLAGS);
}
Device::~Device()
{
	SDL_Quit();
}

int Device::getWidth() { return screen->w; }
int Device::getHeight() { return screen->h; }

void Device::drawImage(Image *i, int x, int y)
{
	SDL_Rect dstrect;
	dstrect.x = x; dstrect.y = y;
	dstrect.w = dstrect.h = 0;
	SDL_BlitSurface(i->surf, 0, screen, &dstrect);
}

void Device::render()
{
	SDL_Flip(screen);
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format,0,0,0));
}

bool Device::run()
{
	int ticks = SDL_GetTicks();
	if (ticks - lastticks < TBF)
		SDL_Delay(TBF-(ticks-lastticks));
	SDL_WM_SetCaption(("Platformutes [FPS="+tostring(1000/(SDL_GetTicks()-lastticks))+"]").c_str(),"Platformutes");
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


Device &getDevice()
{
	static Device device;
	return device;
}

