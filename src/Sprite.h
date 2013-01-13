#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <map>
#include <string>
#include "tools.h"

class Image;

class Sprite
{
	public:
		Sprite(std::string _img, int x, int y);
		virtual ~Sprite();
		void setImage(std::string _img);
		Image *getImage();
		std::string getImageName();
		void setState(std::string _state);
		void setPos(int x, int y);
		vec2 getPos();
		void setVel(int x, int y);
		vec2 getVel();
		vec2 getSize();
		virtual void die();
		virtual void render();
		virtual void step();
	protected:
		std::string img;
		std::string state;
		vec2 p; // position
		vec2 v; // velocity
		bool physics;
};


class ScriptedSprite : public Sprite
{
	public:
		ScriptedSprite(std::string _img, int x, int y,
			std::map<std::string, std::string> &_scripts);
		virtual void step();
	private:
		std::map<std::string, std::string> scripts;
		bool hasContact;
};

#endif//__SPRITE_H__
