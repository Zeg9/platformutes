#include <string>
#include "Video.h"
#include "Sound.h"
#include "Tileset.h"
#include "ResourceMgr.h"

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

Image *ResourceMgr::getImage(std::string name)
{
	if (images.find(name) == images.end())
		images[name] = new Image(DATA_PATH+name+".png");
	return images[name];
}

Sound *ResourceMgr::getSound(std::string name)
{
	if (sounds.find(name) == sounds.end())
		sounds[name] = new Sound(DATA_PATH+name+".ogg");
	return sounds[name];
}

Tileset *ResourceMgr::getTileset(std::string name)
{
	if (tilesets.find(name) == tilesets.end())
		tilesets[name] = new Tileset(DATA_PATH"tilesets/"+name+"/tileset");
	return tilesets[name];
}


ResourceMgr &getResourceMgr()
{
	static ResourceMgr resourceMgr;
	return resourceMgr;
}
