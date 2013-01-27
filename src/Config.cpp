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

#include <fstream>
#include "ResourceMgr.h"
#include "tools.h"
#include "Config.h"

#include <iostream>

Config::Config()
{
	using namespace std;
	try {
		ifstream in (getResourceMgr().getPath("config").c_str());
		string content("");
		while (in.good())
			content += in.get();
		in.close();
		vector<string> toks = split(content, ';');
		for (unsigned int i = 0; i < toks.size(); i++)
		{
			vector<string> kv = split(toks[i],':',2);
			if (kv.size() != 2) continue;
			string key = stripspaces(kv[0]), value = stripspaces(kv[1]);
			settings[key] = value;
		}
	} catch (...) {
		std::cerr << "Warning: Couldn't read config file" << std::endl;
	}
}
Config::~Config()
{
	using namespace std;
	try {
		ofstream out (getResourceMgr().getWritePath("config").c_str());
		for (map<string,string>::iterator i = settings.begin(); i != settings.end(); i++)
		{
			if (i->first[0] != '_') // keys starting with _ won't be saved
				out << i->first << ": " << i->second << ";\n";
		}
	} catch (...) {
		std::cerr << "Warning: Couldn't save config file" << std::endl;
	}
}

bool Config::getBool(std::string key)
{
	if (settings.find(key) == settings.end())
		settings[key] = tostring(false);
	return tobool(settings[key]);
}
void Config::setBool(std::string key, bool value)
{
	settings[key] = tostring(value);
}

int Config::getInt(std::string key)
{
	if (settings.find(key) == settings.end())
		settings[key] = tostring(0);
	return toint(settings[key]);
}
void Config::setInt(std::string key, int value)
{
	settings[key] = tostring(value);
}

std::string Config::getString(std::string key)
{
	if (settings.find(key) == settings.end())
		settings[key] = "";
	return settings[key];
}
void Config::setString(std::string key, std::string value)
{
	settings[key] = value;
}

Config &getConfig()
{
	static Config config;
	return config;
}
