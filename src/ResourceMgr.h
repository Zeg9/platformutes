#ifndef __RESOURCEMGR_H__
#define __RESOURCEMGR_H__

#include <map>
#include <string>

#define DATA_PATH "../data/"

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
