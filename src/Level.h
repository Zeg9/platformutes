#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

// TODO find some way for this to not be hard coded
#define FIRST_LEVEL "pyramid/lvl1"

// don't change this
#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32

class Tile;
class Tileset;
class Image;

class Level
{
	public:
		Level();
		void load(std::string _name);
		void load_next();
		void reload();
		int getId(int x, int y);
		Tile &get(int x, int y);
		void set(int x, int y, int tile);
		Image *getBackground();
		Tileset *getTileset();
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
