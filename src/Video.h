#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stack>
#include <string>
#include <SDL/SDL.h>
#include "tools.h"

// You can change the resolution here
// It is recommended to keep a relatively small one,
// because the game become really laggy with higher ones.
// You should keep multiples of 32 for both width and height.
// Default: 640x480
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

// Bits per pixel
// Minimum: 8
// Default: 32
#define VIDEO_BPP 32

// Optional SDL_FLAGS
#define VIDEO_SDL_FLAGS 0

// You can change the FPS here, but you shouldn't
// If you set higher FPS all the game will be faster,
// sprites (character, badguys, ...) too
#define MAX_FPS 60

// You shouldn't modify this.
#define TBF 1000/MAX_FPS // Time between frames


class Image
{
	friend class Device;
	public:
		Image(std::string filename);
		~Image();
		int getWidth();
		int getHeight();
		vec2 getSize();
	private:
		SDL_Surface *surf;
};

class Device
{
	friend Device &getDevice();
	public:
		int getWidth();
		int getHeight();
		void toggleFullscreen();
		// draw image by position
		void drawImage(Image *i, int x=0, int y=0);
		void drawImage(Image *i, vec2 p);
		// draw image by position and a clipping rect
		void drawImage(Image *i, int x, int y, int cx, int cy, int cw, int ch);
		// call this when the scene is drawn
		void render();
		bool run();
		bool hasEvent();
		SDL_Event nextEvent();
		void quit(); // done = true
		void close(); // This closes the window faster
	private:
		Device();
		~Device();
		SDL_Surface *screen;
		std::stack<SDL_Event> eventStack;
		int lastticks; // last frame SDL_GetTicks()
		bool fullscreen;
		bool done; // When set to true, the application exits
};
Device &getDevice();

#endif//__VIDEO_H__
