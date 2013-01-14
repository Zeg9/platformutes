#include <map>
#include <string>
#include <stdexcept>
#include "tools.h"
#include "Sprite.h"
#include "Environment.h"
#include "Script.h"

#include <iostream>

std::map<std::string, std::string> &getScriptVars()
{
	static std::map<std::string, std::string> scriptVars;
	return scriptVars;
}

std::string parseVar(std::string in)
{
	std::string scriptprefix = "$script.";
	if (startswith(in, scriptprefix))
		return getScriptVars()[in.substr(scriptprefix.size())];
	if (in == "$player.image")
		return PLAYER->getImageName();
	return in;
}

int expectedArgs(std::string f)
{
	if (f == "player.image")    return 1;
	if (f == "player.die")      return 0;
	if (f == "sprite.image")    return 1;
	if (f == "sprite.state")    return 1;
	if (f == "sprite.remove")   return 0;
	if (f == "level.load")      return 1;
	if (f == "level.load_next") return 0;
	if (f == "level.set_tile")  return 3;
	if (f == "set_var")         return 2;
	if (f == "ifeq")            return 2;
	if (f == "endif")           return 0;
	if (f == "print")           return -2;
	return -1;
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
		for (unsigned int i = 0; i < args.size(); i++)
			args[i] = parseVar(args[i]);
		
		int e = expectedArgs(function);
		if (e == -1)
			throw std::runtime_error("Unknown function: "+function);
		if (e != -2 && args.size() != (unsigned int)e)
			throw std::runtime_error("Excepted "+tostring(e)
				+" arguments to "+function);
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
		else if (function == "sprite.image")
			sprite->setImage(args[0]);
		else if (function == "sprite.state")
			sprite->setState(args[0]);
		else if (function == "sprite.remove")
			ENV.removeSprite(sprite);
		else if (function == "level.load")
		{
			PLAYER->setPos(32,64);
			PLAYER->setImage("common.character");
			ENV.lvl.load(args[0]);
		}
		else if (function == "level.load_next")
		{
			PLAYER->setPos(32,64);
			PLAYER->setImage("common.character");
			ENV.lvl.load_next();
		}
		else if (function == "level.set_tile")
		{
			ENV.lvl.set(
				toint(args[0])+pos.x,
				toint(args[1])+pos.y,
				toint(args[2]));
		}
		else if (function == "set_var")
			getScriptVars()[args[0]] = args[1];
		else if (function == "ifeq")
		{
			isinif = true;
			if (args[0] == args[1])
				ifok = true;
			else
				ifok = false;
		}
		else if (function == "endif") {}
		else if (function == "print") // debug purposes only
		{
			for (unsigned int i = 0; i < args.size(); i++)
				std::cout << args[i] << ' ';
			std::cout << std::endl;
		}
	}
}

