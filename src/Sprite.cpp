#include <string>
#include "tools.h"
#include "ResourceMgr.h"
#include "Video.h"
#include "Level.h"
#include "Environment.h"
#include "Tile.h"
#include "Image.h"
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
	std::cout << "Removing sprite (" << img << ")..." << std::endl;
}

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
void Sprite::setPos(int x, int y) { p = vec2(x,y); }
vec2 Sprite::getPos() { return p; }
void Sprite::setVel(int x, int y) { v = vec2(x,y); }
vec2 Sprite::getVel() { return v; }
vec2 Sprite::getSize() { return getImage()->getSize(); }

void Sprite::die()
{
	// This should be implemented in derivated classes, like player
}

void Sprite::render()
{
	Level &lvl = getEnvironment().lvl;
	Device &d = getDevice();
	int lw = lvl.getWidth()*BLOCK_WIDTH;
	int lh = lvl.getHeight()*BLOCK_HEIGHT;
	int dx = p.x - PPOS.x + d.getWidth()/2/BLOCK_WIDTH*BLOCK_WIDTH;
	int dy = p.y - PPOS.y + d.getHeight()/2/BLOCK_HEIGHT*BLOCK_HEIGHT;
	if (PPOS.x < d.getWidth()/2)
		dx = p.x;
	else if (PPOS.x > lw-d.getWidth()/2)
		dx = p.x - lw + d.getWidth();
	if (PPOS.y < d.getHeight()/2)
		dy = p.y;
	else if (PPOS.y+BLOCK_HEIGHT > lh-d.getHeight()/2)
		dy = p.y - lh + d.getHeight();
	d.drawImage(getImage(),dx,dy);
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
{}

void ScriptedSprite::step()
{
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
	Sprite::step();
}

