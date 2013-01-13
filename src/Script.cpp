#include <string>
#include <map>
#include <stdexcept>
#include "tools.h"
#include "Sprite.h"
#include "Environment.h"
#include "Script.h"

#include <iostream>

void checkArgs(std::string function, int argc)
{
	std::map<std::string,int> expectedArgs;
	expectedArgs["player.image"] = 1;
	expectedArgs["player.die"] = 0;
	expectedArgs["sprite.image"] = 1;
	expectedArgs["sprite.remove"] = 0;
	if (argc != expectedArgs[function])
		throw std::runtime_error("Excepted "+tostring(expectedArgs[function])
			+" arguments to "+function);
}

void runScript(std::string script, Sprite *caller)
{
	std::vector<std::string> tokens = split(script,')');
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> t = split(tokens[i],'(',2);
		if (t.size()<1) throw std::runtime_error("Syntax error: "+tokens[i]);
		std::string function = t[0];
		std::vector<std::string> args;
		if (t.size()>1) args = split(t[1],',');
		checkArgs(function,args.size());
		if (function == "player.image")
			PLAYER->setImage(args[0]);
		else if (function == "player.die")
			PLAYER->die();
		else if (function == "sprite.image")
			caller->setImage(args[0]);
		else if (function == "sprite.remove")
			ENV.removeSprite(caller);
		else throw std::runtime_error("Unknown function: "+function);
	}
}
