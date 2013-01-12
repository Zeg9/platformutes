#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stack>
#include <SDL/SDL.h>

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_BPP 32
#define VIDEO_SDL_FLAGS 0

#define MAX_FPS 60
#define TBF 1000/MAX_FPS // Time between frames


class Image;

class Device
{
	friend Device &getDevice();
	public:
		int getWidth();
		int getHeight();
		void drawImage(Image *i, int x=0, int y=0);
		void render();
		bool run();
		bool hasEvent();
		SDL_Event nextEvent();
	private:
		Device();
		~Device();
		SDL_Surface *screen;
		std::stack<SDL_Event> eventStack;
		int lastticks; // last frame SDL_GetTicks()
		bool done; // When set to true, the application exits
};
Device &getDevice();

#endif//__VIDEO_H__
