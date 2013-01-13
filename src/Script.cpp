#include <string>
#include <map>
#include <stdexcept>
#include "tools.h"
#include "Sprite.h"
#include "Environment.h"
#include "Script.h"

#include <iostream>

std::string parseVar(std::string in)
{
	if (in == "$player.image")
		return PLAYER->getImageName();
	return in;
}

void checkArgs(std::string function, int argc)
{
	std::map<std::string,int> expectedArgs;
	expectedArgs["player.image"] = 1;
	expectedArgs["player.die"] = 0;
	expectedArgs["sprite.image"] = 1;
	expectedArgs["sprite.state"] = 1;
	expectedArgs["sprite.remove"] = 0;
	expectedArgs["level.load"] = 1;
	expectedArgs["level.load_next"] = 0;
	expectedArgs["level.set_tile"] = 3;
	expectedArgs["ifeq"] = 2;
	expectedArgs["endif"] = 0;
	if (argc != expectedArgs[function])
		throw std::runtime_error("Excepted "+tostring(expectedArgs[function])
			+" arguments to "+function);
}

void runScript(std::string script, Sprite*sprite, vec2 pos)
{
	std::vector<std::string> tokens = split(script,')');
	bool isinif(false), ifok(false);
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> t = split(tokens[i],'(',2);
		if (t.size()<1) throw std::runtime_error("Syntax error: "+tokens[i]);
		std::string function = t[0];
		std::vector<std::string> args;
		if (t.size()>1) args = split(t[1],',');
		checkArgs(function,args.size());
		if (startswith(function, "sprite.") && !sprite)
			continue;
		if (isinif && !ifok)
		{
			if (function == "endif")
				isinif = false;
		}
		else if (function == "player.image")
			PLAYER->setImage(args[0]);
		else if (function == "player.die")
			PLAYER->die();
		else if (function == "level.load")
		{
			PLAYER->setPos(32,64);
			PLAYER->setImage("common.character");
			ENV.reset();
			ENV.lvl.load(args[0]);
		}
		else if (function == "level.load_next")
		{
			PLAYER->setPos(32,64);
			PLAYER->setImage("common.character");
			ENV.reset();
			ENV.lvl.load_next();
		}
		else if (function == "level.set_tile")
		{
			ENV.lvl.set(
				toint(args[0])+pos.x,
				toint(args[1])+pos.y,
				toint(args[2]));
		}
		else if (function == "ifeq")
		{
			isinif = true;
			if (parseVar(args[0]) == parseVar(args[1]))
				ifok = true;
			else
				ifok = false;
		}
		else if (function == "endif") {}
		else if (function == "sprite.image")
			sprite->setImage(args[0]);
		else if (function == "sprite.state")
			sprite->setState(args[0]);
		else if (function == "sprite.remove")
			ENV.removeSprite(sprite);
		else throw std::runtime_error("Unknown function: "+function);
	}
}

