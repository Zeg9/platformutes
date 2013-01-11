#include "Tile.h"
#include "Image.h"
#include "ResourceMgr.h"

Tile::Tile() : img(""), light(0), hurt(0), solid(true) {}

Image *Tile::getImage() { return getResourceMgr().getImage("tiles/"+img); }
int Tile::getLighting() { return light; }
int Tile::getHurt() { return hurt; }
bool Tile::isSolid() { return solid; }
bool Tile::isAir() { return false; }

AirTile::AirTile() : Tile() { solid = false; }
bool AirTile::isAir() { return true; }

AirTile &getAirTile()
{
	static AirTile airTile;
	return airTile;
}

