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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>

class Config
{
	friend Config& getConfig();
	public:
		bool getBool(std::string key);
		void setBool(std::string key, bool value);
		int getInt(std::string key);
		void setInt(std::string key, int value);
		std::string getString(std::string key);
		void setString(std::string key, std::string value);
	private:
		Config();
		~Config();
		std::map<std::string, std::string> settings;
};

Config& getConfig();

#endif//__CONFIG_H__
