#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <string>
#include <SDL/SDL.h>

class Image
{
	friend class Device;
	public:
		Image(std::string filename);
		~Image();
		int getWidth();
		int getHeight();
	private:
		SDL_Surface *surf;
};

#endif//__IMAGE_H__
