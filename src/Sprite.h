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
		void enablePhysics(bool b);
		void setImage(std::string _img);
		Image *getImage();
		std::string getImageName();
		void setState(std::string _state);
		std::string getState();
		void setPos(int x, int y);
		vec2 getPos();
		void setVel(int x, int y);
		vec2 getVel();
		vec2 getSize();
		void jump();
		virtual void hit();
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
		virtual void hit();
		virtual void step();
	private:
		std::map<std::string, std::string> scripts;
		bool hasContact;
		unsigned int lastSecond;
};

#endif//__SPRITE_H__
