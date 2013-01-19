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

#ifndef __BADGUY_H__
#define __BADGUY_H__

#include "Sprite.h"

class Badguy : public ScriptedSprite
{
	public:
		Badguy(std::string _img, int x, int y,
			std::map<std::string, std::string> &_scripts);
		virtual void step();
	private:
		bool isGoingLeft();
		bool isGoingRight();
		bool shouldGoLeft();
		bool shouldGoRight();
};

#endif//__BADGUYS_H__
