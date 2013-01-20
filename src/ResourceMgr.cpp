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
#include <stdexcept>
#include "Video.h"
#include "Sound.h"
#include "Tileset.h"
#include "ResourceMgr.h"

#define DATA_PATH "../data/"

#define TRY_PATH(P) if (exists(P)) { std::cout << P << std::endl; return P; }

#include <iostream>

ResourceMgr::ResourceMgr() {}

ResourceMgr::~ResourceMgr()
{
	for(std::map<std::string,Image*>::iterator i = images.begin();
		i != images.end(); i++)
			delete i->second;
	for(std::map<std::string,Sound*>::iterator i = sounds.begin();
		i != sounds.end(); i++)
			delete i->second;
	for(std::map<std::string,Tileset*>::iterator i = tilesets.begin();
		i != tilesets.end(); i++) 
			delete i->second;
}

std::string ResourceMgr::getPath(std::string needle)
{
	TRY_PATH("./data/"+needle)
	TRY_PATH("../data/"+needle)
	#ifdef __unix__
	TRY_PATH("/usr/share/platformutes/"+needle)
	TRY_PATH("/usr/local/share/platformutes/"+needle)
	#endif
	throw std::runtime_error("Couldn't find resource: "+needle);
}

Image *ResourceMgr::getImage(std::string name)
{
	if (images.find(name) == images.end())
		images[name] = new Image(getPath(name+".png"));
	return images[name];
}

Sound *ResourceMgr::getSound(std::string name)
{
	if (sounds.find(name) == sounds.end())
		sounds[name] = new Sound(getPath(name+".ogg"));
	return sounds[name];
}

Tileset *ResourceMgr::getTileset(std::string name)
{
	if (tilesets.find(name) == tilesets.end())
		tilesets[name] = new Tileset(getPath("tilesets/"+name+"/tileset"));
	return tilesets[name];
}


ResourceMgr &getResourceMgr()
{
	static ResourceMgr resourceMgr;
	return resourceMgr;
}
