#include <string>
#include <fstream>
#include <stdexcept>
#include "tools.h"
#include "ResourceMgr.h"
#include "Tile.h"
#include "Tileset.h"
#include "Environment.h"
#include "Sprite.h"
#include "Badguy.h"
#include "Image.h"
#include "Video.h"
#include "Level.h"

#include <iostream>

Level::Level():
	tileset(""), background(""), name(""),
	width(0), height(0),
	blocks(0)
{}
void Level::load(std::string _name)
{
	name = _name;
	reload();
}
void Level::load_next()
{
	if (next != "")
		name = next;
	reload();
}
void Level::reload()
{
	ENV.reset();
	std::cout << "== Loading level: " << name << "..." << std::endl;
	width = height = 0;
	tileset = background = next = "";
	std::ifstream ifs(("../data/levels/"+name+".pmlvl").c_str());
	std::string fl;
	getline(ifs,fl);
	std::vector<std::string> fls = split(stripspaces(fl),';');
	for (unsigned int i = 0; i < fls.size(); i++)
	{
		std::vector<std::string> kv = split(fls[i],':');
		if (kv.size() != 2) continue;
		std::string key = kv[0], value = kv[1];
		if (key == "width") width = toint(value);
		if (key == "height") height = toint(value);
		if (key == "tileset") tileset = value;
		if (key == "background") background = value;
		if (key == "next") next = value;
	}
	if (tileset == "" || width == 0 || height == 0)
		throw std::runtime_error("Please specify a tileset, a width and a height.");
	std::cout << "Tileset: " << tileset << std::endl;
	std::cout << "Background: " << background << std::endl;
	std::cout << "Size: " << width << "x" << height << std::endl;
	blocks = new int*[width];
	for (int x = 0; x < width; x++)
	{
		blocks[x] = new int[height];
		for (int y = 0; y < height; y++)
			blocks[x][y] = 0;
	}
	int x=0, y=0;
	std::string b("");
	char c(0);
	while (ifs.good())
	{
		if (x >= width || y >= height) break;
		c = ifs.get();
		if (c == '\n' || c == '\r')
		{
			if (b != "")
			{
				blocks[x][y] = toint(b);
				x=0;
				y++;
				b = "";
			}
		}
		else if (c == ' ' || c == '\t')
		{
			if (b != "")
			{
				blocks[x][y] = toint(b);
				x++;
				b = "";
			}
		}
		else
			b += c;
	}
	std::cout << "== Level loaded !" << std::endl;
}

Tile &Level::get(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		Tile &t = getResourceMgr().getTileset(tileset)->get(blocks[x][y]);
		if (t.getSprite() != "none")
		{
			std::string image = t.getImageName();
			std::vector<std::string> tokens = split(image,'&',2);
			if (tokens.size()==2)
				image = tokens[0];
			Sprite * s = 0;
			if(t.getSprite() == "badguy")
				s = new Badguy(
					image, x*BLOCK_WIDTH, y*BLOCK_HEIGHT,
					t.getScripts());
			else if (t.getSprite() == "scripted")
				s = new ScriptedSprite(
					image, x*BLOCK_WIDTH, y*BLOCK_HEIGHT,
					t.getScripts());
			if (s != 0)
			{
				if (tokens.size()==2)
					s->setState(tokens[1]);
				s->enablePhysics(t.isSolid());
				ENV.addSprite(s);
			}
			blocks[x][y] = -1;
			return getAirTile();
		}
		return t;
	}
	return getAirTile();
}
void Level::set(int x, int y, int tile)
{
	if (x >= 0 && x < getWidth() && y >= 0 && y < getHeight())
		blocks[x][y] = tile;
}

Image *Level::getBackground()
{
	return getResourceMgr().getImage("tilesets/"+tileset+"/background");
}

std::string Level::getTilesetName() { return tileset; }
int Level::getWidth() { return width; }
int Level::getHeight() { return height; }

void Level::render()
{
	Device &d = getDevice();
	for (int sx = 0; sx < d.getWidth()/BLOCK_WIDTH+1; sx++)
	{
		for (int sy = 0; sy < d.getHeight()/BLOCK_HEIGHT+1; sy++)
		{
			int x = sx - d.getWidth()/2/BLOCK_WIDTH+PPOS.x/BLOCK_WIDTH;
			int y = sy - d.getHeight()/2/BLOCK_WIDTH+PPOS.y/BLOCK_HEIGHT;
			int dx = sx*BLOCK_WIDTH-PPOS.x%BLOCK_WIDTH;
			int dy = sy*BLOCK_HEIGHT-PPOS.y%BLOCK_HEIGHT;
			if (PPOS.x < d.getWidth()/2)
			{
				x = sx;
				dx = sx*BLOCK_WIDTH;
			}
			else if (PPOS.x > getWidth()*BLOCK_WIDTH-d.getWidth()/2)
			{
				x = getWidth()-d.getWidth()/BLOCK_WIDTH+sx;
				dx = sx*BLOCK_WIDTH;
			}
			if (PPOS.y < d.getHeight()/2)
			{
				y = sy;
				dy = sy*BLOCK_HEIGHT;
			}
			else if (PPOS.y+BLOCK_HEIGHT > getHeight()*BLOCK_HEIGHT-d.getHeight()/2)
			{
				y = getHeight()-d.getHeight()/BLOCK_HEIGHT+sy;
				dy = sy*BLOCK_HEIGHT;
			}
			if (get(x,y).isAir()) continue;
			d.drawImage(get(x,y).getImage(), dx, dy);
			if (!get(x,y).hasShading()) continue;
			bool t, b, l, r, tr, br, bl, tl;
			int w = getWidth(), h = getHeight();
			t = !get(x,y-1).hasShading();
			b = !get(x,y+1).hasShading();
			l = !get(x-1,y).hasShading();
			r = !get(x+1,y).hasShading();
			tr = !get(x+1,y-1).hasShading();
			br = !get(x+1,y+1).hasShading();
			bl = !get(x-1,y+1).hasShading();
			tl = !get(x-1,y-1).hasShading();
			if (y > 0 && t)
				d.drawImage(getResourceMgr().getImage("common/shading/t"),dx,dy);
			if (y < h-1 && b)
				d.drawImage(getResourceMgr().getImage("common/shading/b"),dx,dy);
			if (x > 0 && l)
				d.drawImage(getResourceMgr().getImage("common/shading/l"),dx,dy);
			if (x < w-1 && r)
				d.drawImage(getResourceMgr().getImage("common/shading/r"),dx,dy);
			if (tr && !(t || r))
				d.drawImage(getResourceMgr().getImage("common/shading/tr"),dx,dy);
			if (br && !(b || r))
				d.drawImage(getResourceMgr().getImage("common/shading/br"),dx,dy);
			if (bl && !(b || l))
				d.drawImage(getResourceMgr().getImage("common/shading/bl"),dx,dy);
			if (tl && !(t || l))
				d.drawImage(getResourceMgr().getImage("common/shading/tl"),dx,dy);
		}
	}
}

