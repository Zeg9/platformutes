#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>
#include "tools.h"

class Image;

class Sprite
{
	public:
		Sprite(std::string _img, int x, int y, int w, int h);
		virtual ~Sprite();
		void setImage(std::string _img);
		void setPos(int x, int y);
		vec2 getPos();
		void setVel(int x, int y);
		vec2 getVel();
		virtual void render();
		virtual void step();
	private:
		std::string img;
		vec2 p; // position
		vec2 v; // velocity
		vec2 s; // size
		bool physics;
};

#endif//__SPRITE_H__
