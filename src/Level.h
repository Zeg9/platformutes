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

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

// TODO find some way for this to not be hard coded
#define FIRST_LEVEL "pyramid/lvl1"

// don't change this
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class Tile;
class Tileset;
class Image;

class Level
{
	public:
		Level();
		void load(std::string _name);
		void load_next();
		void reload();
		int getId(int x, int y);
		Tile &get(int x, int y);
		void set(int x, int y, int tile);
		Image *getBackground();
		Tileset *getTileset();
		std::string getTilesetName();
		int getWidth();
		int getHeight();
		void render();
	protected:
		std::string tileset;
		std::string background;
		std::string name, next;
		int width, height;
		int ** blocks;
};

#endif//__LEVEL_H__
