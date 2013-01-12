#ifndef __TILE_H__
#define __TILE_H__

#include <string>

class Image;

class Tile
{
	friend class Tileset;
	public:
		Tile();
		Image *getImage();
		std::string getSpawn();
		int getLighting();
		int getHurt();
		bool isSolid();
		bool hasShading();
		virtual bool isAir();
	protected:
		std::string img, spawn;
		int light, hurt;
		bool solid, shading;
};
class AirTile : public Tile
{
	friend AirTile &getAirTile();
	public:
		virtual bool isAir();
	private:
		AirTile();
};
AirTile &getAirTile();

#endif//__TILE_H__
