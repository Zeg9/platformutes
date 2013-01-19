#include "Tile.h"
#include "Video.h"
#include "ResourceMgr.h"
#include "Environment.h"

Tile::Tile() : img(""), sprite("none"), light(0), hurt(0), solid(true), shading(false) {}

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

