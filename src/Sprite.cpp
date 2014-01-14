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
#include "tools.h"
#include "ResourceMgr.h"
#include "Video.h"
#include "Level.h"
#include "Environment.h"
#include "Tile.h"
#include "Script.h"
#include "Sprite.h"

#include <iostream>

#define W getSize().x
#define H getSize().y

Sprite::Sprite(std::string _img, int x, int y) :
	img(_img), imgptr(0), framecount(0), frame(0), state("stand_r"), p(x, y), v(0,0), physics(true), jumping(false)
{}

Sprite::~Sprite()
{
	std::cout << "Removing " << img << "..." << std::endl;
}

void Sprite::enablePhysics(bool b) { physics = b; }

void Sprite::setImage(std::string _img)
{
	img = _img;
	setState(state);
	updateImage();
}
void Sprite::updateImage()
{
	if (framecount != 0)
	{
		frame = frame+1;
		if (frame >= framecount) frame = 1;
	}
	std::vector<std::string> tokens = split(img,'.');
	std::string filename("");
	if (tokens.size() == 2 && tokens[0]=="common")
		filename += "common/sprites/"+tokens[1];
	else
		filename += "tilesets/"+ENV.lvl.getTilesetName()+"/sprites/"+img;
	filename += '/'+state;
	if (frame != 0)
		filename += '_'+tostring((int)frame+1);
	imgptr = getResourceMgr().getImage(filename);
}
Image *Sprite::getImage()
{
	updateImage();
	return imgptr;
}
std::string Sprite::getImageName() { return img; }
void Sprite::setState(std::string _state) {
	state = _state;
	
	try {
		// see if there is no animation
		framecount = 0;
		frame = 0;
		updateImage();
		std::cout << "Loaded static sprite for " << img << std::endl;
	} catch (...) {
		// try to load an animated sprite
		framecount = 0;
		frame = 1;
		bool done = false;
		while (!done)
		{
			try {
				framecount += 1;
				updateImage();
			} catch(...) { done = true; }
		}
		framecount -= 1;
		frame = 1;
		std::cout << "Loaded anim for " << img << ", framecount=" << framecount << std::endl;
	}
}
std::string Sprite::getState() { return state; }
void Sprite::setPos(int x, int y) { p = vec2(x,y); }
vec2 Sprite::getPos() { return p; }
void Sprite::setVel(int x, int y) { v = vec2(x,y); }
vec2 Sprite::getVel() { return v; }
vec2 Sprite::getSize() { return getImage()->getSize(); }
bool Sprite::hasContact(vec2 po, vec2 si)
{
	return (po.x+si.x >= p.x
	     && po.x <= p.x + getSize().x-1
	     && po.y+si.y >= p.y
	     && po.y <= p.y + getSize().y-1);
}

void Sprite::jump()
{
	if (ENV.lvl.get(p.x/TILE_WIDTH,p.y/TILE_WIDTH+2)->canJump()
	 || ENV.lvl.get((p.x-1)/TILE_WIDTH+1,p.y/TILE_WIDTH+2)->canJump())
		setVel(getVel().x,-10);
}

void Sprite::setJumping(bool j) { jumping = j; }
bool Sprite::isJumping() { return jumping; }

void Sprite::hit()
{
	// When the player hits the sprite
}

void Sprite::die()
{
	// This should be implemented in derivated classes, like player
}

void Sprite::render()
{
	getDevice().drawImage(getImage(),getDrawPos(p));
}

void Sprite::step()
{
	Level &lvl = getEnvironment().lvl;
	vec2 slowness (1,1);
	SPRITE_FOR_CONTACT_TILES
	{
		vec2 p = vec2(x,y).toTile();
		vec2 c = lvl.get(p)->getSlowness();
		if (c.x > slowness.x)
			slowness.x = c.x;
		if (c.y > slowness.y)
			slowness.y = c.y;
	}
	if (!physics)
	{
		p.x += v.x; p.y += v.y;
		return;
	}
	if (jumping) jump();
	int nx = p.x+(v.x/slowness.x),
	    ny = p.y+(v.y/slowness.y);
	if (v.y < 0 && !(
		lvl.get(p.x/TILE_WIDTH,ny/TILE_HEIGHT)->isSolid() ||
		lvl.get((p.x-1)/TILE_WIDTH+1,ny/TILE_HEIGHT)->isSolid()
		))
		p.y = ny;
	else if (v.y < 0)
	{
		p.y = round(p.y,TILE_HEIGHT);
		v.y = 0;
	}
	if (v.y > 0 && !(
		lvl.get(p.x/TILE_WIDTH,(ny-1+H)/TILE_HEIGHT)->isSolid() ||
		lvl.get((p.x-1)/TILE_WIDTH+1,(ny-1+H)/TILE_HEIGHT)->isSolid()
		))
		p.y = ny;
	else if (v.y > 0)
	{
		p.y = round(p.y,TILE_HEIGHT);
		v.y = 0;
	}
	if (v.x < 0 && !(
		nx < 0 ||
		lvl.get(nx/TILE_WIDTH, p.y/TILE_HEIGHT)->isSolid() ||
		lvl.get(nx/TILE_WIDTH, (p.y-1+TILE_HEIGHT)/TILE_HEIGHT)->isSolid() ||
		lvl.get(nx/TILE_WIDTH, (p.y-1+TILE_HEIGHT*2)/TILE_HEIGHT)->isSolid()
		))
		p.x = nx;
	else if (v.x < 0)
		p.x = round(p.x,TILE_WIDTH);
	if (v.x > 0 && !(
		nx+getSize().x > lvl.getWidth()*TILE_WIDTH ||
		lvl.get((nx-1)/TILE_WIDTH+1, p.y/TILE_HEIGHT)->isSolid() ||
		lvl.get((nx-1)/TILE_WIDTH+1, (p.y-1+TILE_HEIGHT)/TILE_HEIGHT)->isSolid() ||
		lvl.get((nx-1)/TILE_WIDTH+1, (p.y-1+TILE_HEIGHT*2)/TILE_HEIGHT)->isSolid()
		))
		p.x = nx;
	else if (v.x > 0)
		p.x = round(p.x,TILE_WIDTH);
	if (v.y < 5)
		v.y += 1;
}

ScriptedSprite::ScriptedSprite(std::string _img, int x, int y,
	std::map<std::string, std::string> &_scripts):
		Sprite(_img,x,y), scripts(_scripts), hasContact(false)
{
	runScript(scripts["on_spawn"], this, vec2(p.x/TILE_WIDTH,p.y/TILE_HEIGHT));
}

void ScriptedSprite::hit()
{
	runScript(scripts["on_hit"], this, vec2(p.x/TILE_WIDTH,p.y/TILE_HEIGHT));
}

void ScriptedSprite::step()
{
	if (SDL_GetTicks()/1000 != lastSecond)
	{
		lastSecond = SDL_GetTicks()/1000;
		runScript(scripts["on_second"], this, vec2(p.x/TILE_WIDTH, p.y/TILE_HEIGHT));
	}
	runScript(scripts["on_step"], this, vec2(p.x/TILE_WIDTH,p.y/TILE_HEIGHT));
	if (p.x+getSize().x >= PPOS.x
	 && p.x <= PPOS.x + PSIZE.x-1
	 && p.y+getSize().y >= PPOS.y
	 && p.y <= PPOS.y + PSIZE.y-1)
	{
		if (!hasContact)
		{
			runScript(scripts["on_contact"], this, vec2(p.x/TILE_WIDTH,p.y/TILE_HEIGHT));
			hasContact = true;
		}
	}
	else if (hasContact)
		hasContact = false;
	int nx = p.x+v.x, ny = p.y+v.y;
	Sprite::step();
	if (p.x != nx || p.y != ny)
		runScript(scripts["on_collision"], this, vec2(p.x/TILE_WIDTH,p.y/TILE_HEIGHT));
}

