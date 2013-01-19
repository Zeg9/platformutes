#ifndef __TILESET_H__
#define __TILESET_H__

#include <map>
#include <vector>
#include <string>
#include "Tile.h"

class Tileset
{
	public:
		Tileset(std::string filename);
		Tile &get(int num);
		std::vector<int> getValidTiles();
	private:
		std::map<int, Tile> tiles;
};

#endif//__TILESET_H__
