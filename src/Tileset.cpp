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
#include "tools.h"
#include "Tile.h"
#include "Tileset.h"

#include <iostream>

Tileset::Tileset(std::string filename)
{
	for (unsigned int i = 0; i < TILESET_LENGTH; i++)
		tiles[i] = 0;
	std::cout << "== Loading tileset: " << filename << "..." << std::endl;
	std::ifstream ifs(filename.c_str());
	std::string contents;
	while (ifs.good())
		contents += ifs.get();
	std::vector<std::string> tokens = split(contents,'[');
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> ls = split(tokens[i],']',2);
		if (ls.size() != 2) continue;
		if (stripspaces(ls[0])=="tileset")
		{
			std::vector<std::string> toks = split(ls[1],';');
			for (unsigned int i = 0; i < toks.size(); i++)
			{
				std::vector<std::string> kv = split(stripspaces(toks[i]),':',2);
				if (kv.size() != 2) continue;
				std::string key = kv[0], value = kv[1];
				if (startswith(key,"on_")) scripts[key] = value;
			}
		} else {
			int num = toint(stripspaces(ls[0]));
			if (!num) continue;
			tiles[num] = new Tile(ls[1]);
		}
	}
}
Tileset::~Tileset()
{
	for (unsigned int i = 0; i < TILESET_LENGTH; i++)
		if (tiles[i])
			delete tiles[i];
}

Tile *Tileset::get(int num)
{
	if (num > 0 && num < TILESET_LENGTH && tiles[num])
		return tiles[num];
	return getAirTile();
}

std::vector<int> Tileset::getValidTiles()
{
	std::vector<int> r;
	for (unsigned int i = 0; i < TILESET_LENGTH; i++)
		if (tiles[i])
			r.push_back(i);
	return r;
}

