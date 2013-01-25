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
#include <fstream>
#include <stdexcept>
#include "tools.h"
#include "ResourceMgr.h"
#include "Tile.h"
#include "Tileset.h"
#include "Environment.h"
#include "Sprite.h"
#include "Badguy.h"
#include "Video.h"
#include "Config.h"
#include "Script.h"
#include "Level.h"

#include <iostream>

void spriteFromTile (int x, int y, Tile&t)
{
	std::string image = t.getImageName();
	std::vector<std::string> tokens = split(image,'&',2);
	if (tokens.size()==2)
		image = tokens[0];
	Sprite * s = 0;
	if(t.getSprite() == "badguy")
		s = new Badguy(
			image, x*TILE_WIDTH, y*TILE_HEIGHT,
			t.getScripts());
	else if (t.getSprite() == "scripted")
		s = new ScriptedSprite(
			image, x*TILE_WIDTH, y*TILE_HEIGHT,
			t.getScripts());
	if (s != 0)
	{
		if (tokens.size()==2)
			s->setState(tokens[1]);
		s->enablePhysics(t.isSolid());
		ENV.addSprite(s);
	}
}

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
	if (getConfig().getBool("_ingame"))
		getConfig().setString("current_level",name);
	std::cout << "== Loading level: " << name << "..." << std::endl;
	width = height = 0;
	tileset = background = next = "";
	std::ifstream ifs((getResourceMgr().getPath("levels/"+name+".pmlvl")).c_str());
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
	/*std::cout << "Tileset: " << tileset << std::endl;
	std::cout << "Background: " << background << std::endl;
	std::cout << "Size: " << width << "x" << height << std::endl;*/
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
	runScript(getTileset()->scripts["on_level_load"]);
	//std::cout << "== Level loaded !" << std::endl;
}
void Level::save()
{
	if (ENV.allowSprites)
		throw std::runtime_error("Can't save levels without disabling sprites !");
	std::ofstream out (getResourceMgr().getWritePath("levels/"+name+".pmlvl").c_str());
	out << "width: " << width << "; ";
	out << "height: " << height << "; ";
	out << "tileset: " << tileset << "; ";
	out << "background: " << background << "; ";
	out << "next: " << next << "; ";
	out << '\n';
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			out << getId(x, y);
			if (x < width-1)
			{
				for (unsigned int i = 0; i < 3-tostring(getId(x,y)).size(); i++)
					out << ' ';
				out << ' ';
			}
		}
		out << '\n';
	}
	out << std::flush;
}

int Level::getId(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		return blocks[x][y];
	return 0;
}
Tile &Level::get(int x, int y)
{
	return getTileset()->get(getId(x,y));
}
void Level::set(int x, int y, int tile)
{
	if (!(x >= 0 && x < getWidth() && y >= 0 && y < getHeight())) return;
	Tile &t = getTileset()->get(tile);
	if (ENV.allowSprites && t.getSprite() != "none")
		spriteFromTile(x,y,t);
	else
		blocks[x][y] = tile;
}

Image *Level::getBackground()
{
	return getResourceMgr().getImage("tilesets/"+tileset+"/background");
}
Tileset *Level::getTileset()
{
	return getResourceMgr().getTileset(tileset);
}
std::string Level::getTilesetName() { return tileset; }
int Level::getWidth() { return width; }
int Level::getHeight() { return height; }

void Level::render()
{
	Device &d = getDevice();
	for (int sx = 0; sx < d.getWidth()/TILE_WIDTH+1; sx++)
	{
		for (int sy = 0; sy < d.getHeight()/TILE_HEIGHT+1; sy++)
		{
			int x = sx - d.getWidth()/2/TILE_WIDTH+PPOS.x/TILE_WIDTH;
			int y = sy - d.getHeight()/2/TILE_WIDTH+PPOS.y/TILE_HEIGHT;
			if (PPOS.x < d.getWidth()/2)
				x = sx;
			else if (PPOS.x > getWidth()*TILE_WIDTH-d.getWidth()/2)
				x = getWidth()-d.getWidth()/TILE_WIDTH+sx;
			if (PPOS.y < d.getHeight()/2)
				y = sy;
			else if (PPOS.y+TILE_HEIGHT > getHeight()*TILE_HEIGHT-d.getHeight()/2)
				y = getHeight()-d.getHeight()/TILE_HEIGHT+sy;
			if (get(x,y).isAir()) continue;
			
			if (ENV.allowSprites && get(x,y).getSprite() != "none")
			{
				spriteFromTile(x,y,get(x,y));
				blocks[x][y] = -1;
				continue;
			}
			vec2 dp = getDrawPos(vec2(x*TILE_WIDTH, y*TILE_HEIGHT));
			d.drawImage(get(x,y).getImage(), dp);
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
				d.drawImage(getResourceMgr().getImage("common/shading/t"),dp);
			if (y < h-1 && b)
				d.drawImage(getResourceMgr().getImage("common/shading/b"),dp);
			if (x > 0 && l)
				d.drawImage(getResourceMgr().getImage("common/shading/l"),dp);
			if (x < w-1 && r)
				d.drawImage(getResourceMgr().getImage("common/shading/r"),dp);
			if (tr && !(t || r))
				d.drawImage(getResourceMgr().getImage("common/shading/tr"),dp);
			if (br && !(b || r))
				d.drawImage(getResourceMgr().getImage("common/shading/br"),dp);
			if (bl && !(b || l))
				d.drawImage(getResourceMgr().getImage("common/shading/bl"),dp);
			if (tl && !(t || l))
				d.drawImage(getResourceMgr().getImage("common/shading/tl"),dp);
		}
	}
}

