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

#ifndef __RESOURCEMGR_H__
#define __RESOURCEMGR_H__

#include <map>
#include <string>

#define DATA_PATH "../data/"
/* TODO better data path handling
Check for:
1) /usr/share/
2) /usr/local/share/
3) ./
*/

class Image;
class Sound;
class Tileset;

class ResourceMgr
{
	friend ResourceMgr &getResourceMgr();
	public:
		Image *getImage(std::string name);
		Sound *getSound(std::string name);
		Tileset *getTileset(std::string name);
	private:
		ResourceMgr();
		~ResourceMgr();
		std::map<std::string, Image*> images;
		std::map<std::string, Sound*> sounds;
		std::map<std::string, Tileset*> tilesets;
};

ResourceMgr &getResourceMgr();

#endif//__RESOURCEMSG_H__
