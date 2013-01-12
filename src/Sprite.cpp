
#include <string>
#include "tools.h"
#include "ResourceMgr.h"
#include "Video.h"
#include "Level.h"
#include "Environment.h"
#include "Tile.h"
#include "Sprite.h"

#include <iostream>

Sprite::Sprite(std::string _img, int x, int y, int w, int h) :
	img(_img), state("stand_r"), p(x, y), v(0,0), s(w, h), physics(true)
{}

Sprite::~Sprite()
{
	std::cout << "Removing sprite (" << img << ")..." << std::endl;
}

void Sprite::setImage(std::string _img) { img = _img; }
void Sprite::setState(std::string _state) { state = _state; }
void Sprite::setPos(int x, int y) { p = vec2(x,y); }
vec2 Sprite::getPos() { return p; }
void Sprite::setVel(int x, int y) { v = vec2(x,y); }
vec2 Sprite::getVel() { return v; }
void Sprite::setSize(int x, int y) { s = vec2(x,y); }
vec2 Sprite::getSize() { return s; }

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
	d.drawImage(getResourceMgr().getImage("sprites/"+img+'/'+state),dx,dy);
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
		lvl.get(p.x/BLOCK_WIDTH,(ny-1+BLOCK_HEIGHT*2)/BLOCK_HEIGHT).isSolid() ||
		lvl.get((p.x-1)/BLOCK_WIDTH+1,(ny-1+BLOCK_HEIGHT*2)/BLOCK_HEIGHT).isSolid()
		))
		p.y = ny;
	else if (v.y > 0)
	{
		p.y = round(p.y,BLOCK_HEIGHT);
		v.y = 0;
	}
	if (v.x < 0 && !(
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

