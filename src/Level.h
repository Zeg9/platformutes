#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

class Tile;

class Level
{
	public:
		Level(std::string filename);
		Tile &get(int x, int y);
		Image *getBackground();
	protected:
		std::string tileset;
		std::string background;
		int width, height;
		int ** blocks;
};

#endif//__LEVEL_H__
