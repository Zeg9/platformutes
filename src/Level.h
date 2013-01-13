#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

class Tile;
class Image;

class Level
{
	public:
		Level();
		void load(std::string _name);
		void load_next();
		void reload();
		Tile &get(int x, int y);
		Image *getBackground();
		std::string getTilesetName();
		int getWidth();
		int getHeight();
		void render();
	protected:
		std::string tileset;
		std::string background;
		std::string name, next;
		int width, height;
		int ** blocks;
};

#endif//__LEVEL_H__
