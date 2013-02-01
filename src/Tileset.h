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

#ifndef __TILESET_H__
#define __TILESET_H__

#include <map>
#include <vector>
#include <string>
#include "Tile.h"

#define TILESET_LENGTH 128

class Tileset
{
	public:
		Tileset(std::string filename);
		~Tileset();
		Tile *get(int num);
		std::vector<int> getValidTiles();
		std::map<std::string, std::string> scripts;
	private:
		Tile* tiles[128];
};

#endif//__TILESET_H__
