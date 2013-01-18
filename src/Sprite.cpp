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
	img(_img), state("stand_r"), p(x, y), v(0,0), physics(true)
{}

Sprite::~Sprite()
{
}

void Sprite::enablePhysics(bool b) { physics = b; }

void Sprite::setImage(std::string _img) { img = _img; }
Image *Sprite::getImage()
{
	std::vector<std::string> tokens = split(img,'.');
	if (tokens.size() == 2 && tokens[0]=="common")
		return getResourceMgr().getImage(
			"common/sprites/"+tokens[1]+'/'+state);
	return getResourceMgr().getImage(
		"tilesets/"+getEnvironment().lvl.getTilesetName()
		+"/sprites/"+img+'/'+state);
}
std::string Sprite::getImageName() { return img; }
void Sprite::setState(std::string _state) { state = _state; }
std::string Sprite::getState() { return state; }
void Sprite::setPos(int x, int y) { p = vec2(x,y); }
vec2 Sprite::getPos() { return p; }
void Sprite::setVel(int x, int y) { v = vec2(x,y); }
vec2 Sprite::getVel() { return v; }
vec2 Sprite::getSize() { return getImage()->getSize(); }

void Sprite::jump()
{
	if (ENV.lvl.get(p.x/BLOCK_WIDTH,p.y/BLOCK_WIDTH+2).isSolid()
	 || ENV.lvl.get((p.x-1)/BLOCK_WIDTH+1,p.y/BLOCK_WIDTH+2).isSolid())
		setVel(getVel().x,-10);
}

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
	if (!physics) return;
	Level &lvl = getEnvironment().lvl;
	int nx = p.x+v.x, ny = p.y+v.y;
	if (v.y < 0 && !(
		lvl.get(p.x/BLOCK_WIDTH,ny/BLOCK_HEIGHT).isSolid() ||
		lvl.get((p.x-1)/BLOCK_WIDTH+1,ny/BLOCK_HEIGHT).isSolid()
		))
		p.y = ny;
	else if (v.y < 0)
	{
		p.y = round(p.y,BLOCK_HEIGHT);
		v.y = 0;
	}
	if (v.y > 0 && !(
		lvl.get(p.x/BLOCK_WIDTH,(ny-1+H)/BLOCK_HEIGHT).isSolid() ||
		lvl.get((p.x-1)/BLOCK_WIDTH+1,(ny-1+H)/BLOCK_HEIGHT).isSolid()
		))
		p.y = ny;
	else if (v.y > 0)
	{
		p.y = round(p.y,BLOCK_HEIGHT);
		v.y = 0;
	}
	if (v.x < 0 && !(
		nx < 0 ||
		lvl.get(nx/BLOCK_WIDTH, p.y/BLOCK_HEIGHT).isSolid() ||
		lvl.get(nx/BLOCK_WIDTH, (p.y-1+BLOCK_HEIGHT)/BLOCK_HEIGHT).isSolid() ||
		lvl.get(nx/BLOCK_WIDTH, (p.y-1+BLOCK_HEIGHT*2)/BLOCK_HEIGHT).isSolid()
		))
		p.x = nx;
	else if (v.x < 0)
		p.x = round(p.x,BLOCK_WIDTH);
	if (v.x > 0 && !(
		nx+getSize().x > lvl.getWidth()*BLOCK_WIDTH ||
		lvl.get((nx-1)/BLOCK_WIDTH+1, p.y/BLOCK_HEIGHT).isSolid() ||
		lvl.get((nx-1)/BLOCK_WIDTH+1, (p.y-1+BLOCK_HEIGHT)/BLOCK_HEIGHT).isSolid() ||
		lvl.get((nx-1)/BLOCK_WIDTH+1, (p.y-1+BLOCK_HEIGHT*2)/BLOCK_HEIGHT).isSolid()
		))
		p.x = nx;
	else if (v.x > 0)
		p.x = round(p.x,BLOCK_WIDTH);
	if (v.y < 5)
		v.y += 1;
}

ScriptedSprite::ScriptedSprite(std::string _img, int x, int y,
	std::map<std::string, std::string> &_scripts):
		Sprite(_img,x,y), scripts(_scripts), hasContact(false)
{
	runScript(scripts["on_spawn"], this, vec2(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT));
}

void ScriptedSprite::hit()
{
	runScript(scripts["on_hit"], this, vec2(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT));
}

void ScriptedSprite::step()
{
	if (SDL_GetTicks()/1000 != lastSecond)
	{
		lastSecond = SDL_GetTicks()/1000;
		runScript(scripts["on_second"], this, vec2(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT));
	}
	runScript(scripts["on_step"], this, vec2(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT));
	if (p.x+getSize().x >= PPOS.x
	 && p.x <= PPOS.x + PSIZE.x-1
	 && p.y+getSize().y >= PPOS.y
	 && p.y <= PPOS.y + PSIZE.y-1)
	{
		if (!hasContact)
		{
			runScript(scripts["on_contact"], this, vec2(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT));
			hasContact = true;
		}
	}
	else if (hasContact)
		hasContact = false;
	int nx = p.x+v.x, ny = p.y+v.y;
	Sprite::step();
	if (p.x != nx || p.y != ny)
		runScript(scripts["on_collision"], this, vec2(p.x/BLOCK_WIDTH,p.y/BLOCK_HEIGHT));
}

