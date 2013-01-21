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

#include "tools.h"
#include "Tile.h"
#include "Video.h"
#include "ResourceMgr.h"
#include "Environment.h"

Tile::Tile() : img(""), sprite("none"), light(0), hurt(0), solid(true), shading(false) {}

void Tile::parse(std::string in)
{
	raw = in;
	std::vector<std::string> toks = split(raw,';');
	for (unsigned int i = 0; i < toks.size(); i++)
	{
		std::vector<std::string> kv = split(stripspaces(toks[i]),':',2);
		if (kv.size() != 2) continue;
		std::string key = kv[0], value = kv[1];
		if (key == "img") img = value;
		if (key == "sprite") sprite = value;
		if (key == "light") light = toint(value);
		if (key == "hurt") hurt = toint(value);
		if (key == "solid") solid = tobool(value);
		if (key == "shading") shading = tobool(value);
		if (startswith(key,"on_")) scripts[key] = value;
	}
}

std::string Tile::getRawData()
{
	return raw;
}

Image *Tile::getImage()
{
	try {
		return getResourceMgr().getImage(
			"tilesets/"+ENV.lvl.getTilesetName()+"/tiles/"+split(img,'&')[0]);
	} catch(...) {
		try {
			return getResourceMgr().getImage(
				"tilesets/"+ENV.lvl.getTilesetName()+"/sprites/"+img+"/stand_r");
		} catch(...) {
			try {
				std::vector<std::string> is = split(img,'&',2);
				return getResourceMgr().getImage(
					"tilesets/"+ENV.lvl.getTilesetName()+"/sprites/"+is[0]+'/'+is[1]);
			} catch(...) {
				return 0;
			}
		}
	}
	return 0;
}
std::string Tile::getImageName() { return img; }
std::string Tile::getSprite() { return sprite; }
std::map<std::string,std::string> &Tile::getScripts() { return scripts; }
int Tile::getLighting() { return light; }
int Tile::getHurt() { return hurt; }
bool Tile::isSolid() { return solid; }
bool Tile::hasShading() { return shading; }
bool Tile::isAir() { return false; }

AirTile::AirTile() : Tile() { solid = false; }
bool AirTile::isAir() { return true; }

AirTile &getAirTile()
{
	static AirTile airTile;
	return airTile;
}

