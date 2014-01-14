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

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <map>
#include <string>
#include "tools.h"
#include "Level.h"

#define SPRITE_FOR_CONTACT_TILES\
	for (int x = getPos().x; x-getPos().x <= getSize().x; x += TILE_WIDTH)\
	for (int y = getPos().y; y-getPos().y <= getSize().y; y += TILE_HEIGHT)\
	if (hasContact(vec2(x,y).toTile().toSprite()))

class Image;

class Sprite
{
	public:
		Sprite(std::string _img, int x, int y);
		virtual ~Sprite();
		void enablePhysics(bool b);
		void setImage(std::string _img);
		void updateImage();
		Image *getImage();
		std::string getImageName();
		void setState(std::string _state);
		std::string getState();
		void setPos(int x, int y);
		vec2 getPos();
		void setVel(int x, int y);
		vec2 getVel();
		vec2 getSize();
		bool hasContact(vec2 po, vec2 si=vec2(TILE_WIDTH,TILE_HEIGHT));
		void jump();
		void setJumping(bool j);
		bool isJumping();
		virtual void hit();
		virtual void die();
		virtual void render();
		virtual void step();
	protected:
		std::string img;
		Image *imgptr;
		std::string state;
		vec2 p; // position
		vec2 v; // velocity
		bool physics;
		bool jumping;
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
