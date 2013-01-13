#include <string>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "tools.h"
#include "Image.h"

Image::Image(std::string filename)
{
	surf = IMG_Load(filename.c_str());
	if (!surf)
		throw std::runtime_error("Couldn't load image: "+filename);
}
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

