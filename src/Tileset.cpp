#include <string>
#include <fstream>
#include "tools.h"
#include "Tile.h"
#include "Tileset.h"

#include <iostream>

Tileset::Tileset(std::string filename)
{
	std::cout << "== Loading tileset: " << filename << "..." << std::endl;
	std::ifstream ifs(filename.c_str());
	std::string l;
	std::string contents;
	while (ifs.good())
	{
		getline(ifs, l);
		contents += l;
	}
	std::vector<std::string> tokens = split(contents,'[');
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> ls = split(stripspaces(tokens[i]),']',2);
		if (ls.size() != 2) continue;
		int num = toint(ls[0]);
		if (!num) continue;
		ls = split(ls[1],';');
		for (unsigned int i = 0; i < ls.size(); i++)
		{
			std::vector<std::string> kv = split(ls[i],':');
			if (kv.size() != 2) continue;
			std::string key = kv[0], value = kv[1];
			if (key == "img") tiles[num].img = value;
			if (key == "sprite") tiles[num].sprite = value;
			if (key == "light") tiles[num].light = toint(value);
			if (key == "hurt") tiles[num].hurt = toint(value);
			if (key == "solid") tiles[num].solid = tobool(value);
			if (key == "shading") tiles[num].shading = tobool(value);
			if (startswith(key,"on_")) tiles[num].scripts[key] = value;
		}
		//std::cout << "Loaded tile " << num << " with image " << tiles[num].img << std::endl;
	}
	//std::cout << "== Tileset loaded !" << std::endl;
}

Tile &Tileset::get(int num)
{
	if (num >0 && tiles.find(num)!=tiles.end()) return tiles[num];
	return getAirTile();
}

std::vector<int> Tileset::getValidTiles()
{
	std::vector<int> r;
	for (std::map<int, Tile>::iterator i = tiles.begin(); i != tiles.end(); i++)
		r.push_back(i->first);
	return r;
}

