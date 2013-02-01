/*
 Platformutes
 Copyright (C) 2012-2013 Zeg9 <dazeg9@gmail.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <map>
#include <string>
#include <stdexcept>
#include "tools.h"
#include "Sprite.h"
#include "Environment.h"
#include "Sound.h"
#include "ResourceMgr.h"
#include "Config.h"
#include "Script.h"

#include <iostream>

std::map<std::string, std::string> &getScriptVars()
{
	static std::map<std::string, std::string> scriptVars;
	return scriptVars;
}

std::string parseVar(std::string in, Sprite* sprite=0)
{
	std::string c=in;
	std::string varp = "$script.";
	if (startswith(in, varp))
		c = getScriptVars()[in.substr(varp.size())];
	else if (startswith(in, "$sprite.") && !sprite)
		c = "";
	else if (in == "$sprite.image")
		c = sprite->getImageName();
	else if (in == "$sprite.state")
		c = sprite->getState();
	else if (in == "$player.image")
		c = PLAYER->getImageName();
	return c;
}

int expectedArgs(std::string f)
{
	if (f == "player.image")    return 1;
	if (f == "player.die")      return 0;
	if (f == "sprite.image")    return 1;
	if (f == "sprite.state")    return 1;
	if (f == "sprite.velocity") return 2;
	if (f == "sprite.remove")   return 0;
	if (f == "level.load")      return 1;
	if (f == "level.load_next") return 0;
	if (f == "level.set_tile")  return 3;
	if (f == "play_sound")      return 1;
	if (f == "set_var")         return 2;
	if (f == "add")             return 2;
	if (f == "remove")          return 2;
	if (f == "ifeq")            return 2;
	if (f == "ifnoteq")         return 2;
	if (f == "ifempty")         return 1;
	if (f == "else")            return 0;
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
			args[i] = parseVar(args[i], sprite);
		
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
			if (function == "else")
				ifok = !ifok;
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
		else if (function == "sprite.velocity")
			sprite->setVel(toint(args[0]), toint(args[1]));
		else if (function == "sprite.remove")
			ENV.removeSprite(sprite);
		else if (function == "level.load")
			ENV.lvl.load(args[0]);
		else if (function == "level.load_next")
			ENV.lvl.load_next();
		else if (function == "level.set_tile")
		{
			ENV.lvl.set(
				toint(args[0])+pos.x,
				toint(args[1])+pos.y,
				toint(args[2]));
		}
		else if (function == "play_sound")
			getSoundManager().playSound(getResourceMgr().getSound(
				"tilesets/"+ENV.lvl.getTilesetName()+"/sounds/"+args[0]),
				pos.x*TILE_WIDTH-PPOS.x, pos.y*TILE_HEIGHT-PPOS.y);
		else if (function == "set_var")
			getScriptVars()[args[0]] = args[1];
		else if (function == "add")
			getScriptVars()[args[0]] = tostring(toint(getScriptVars()[args[0]]) + toint(args[1]));
		else if (function == "remove")
			getScriptVars()[args[0]] = tostring(toint(getScriptVars()[args[0]]) - toint(args[1]));
		else if (function == "ifeq")
		{
			ifok = (!isinif || ifok) && args[0] == args[1];
			isinif = true;
			// TODO implement an "if stack"
		}
		else if (function == "ifnoteq")
		{
			ifok = (!isinif || ifok) && args[0] != args[1];
			isinif = true;
		}
		else if (function == "ifempty")
		{
			ifok = (!isinif || ifok) && args[0] == "";
			isinif = true;
		}
		else if (function == "else")
			ifok = !ifok;
		else if (function == "endif") {}
		else if (function == "print") // debug purposes only
		{
			for (unsigned int i = 0; i < args.size(); i++)
				std::cout << args[i] << ' ';
			std::cout << std::endl;
		}
	}
}

