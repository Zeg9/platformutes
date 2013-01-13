#include "Tile.h"
#include "Image.h"
#include "ResourceMgr.h"
#include "Environment.h"

Tile::Tile() : img(""), sprite("none"), light(0), hurt(0), solid(true), shading(false) {}

Image *Tile::getImage()
{
	return getResourceMgr().getImage(
		"tilesets/"+ENV.lvl.getTilesetName()+"/tiles/"+img);
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

