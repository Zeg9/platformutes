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

#include <vector>
#include <string>
#include "tools.h"
#include "Video.h"
#include "Font.h"

#include <iostream>

Font::Font(std::string def) : font(0)
{
	std::vector<std::string> toks = split(def,'&',2);
	std::string filename = toks[0];
	int size(16);
	if (toks.size()==2) size=toint(toks[1]);
	font = TTF_OpenFont(filename.c_str(),size);
}

Font::~Font()
{
	TTF_CloseFont(font);
}

Image *Font::render(std::string text, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color color; color.r = r; color.g = g; color.b = b;
	return new Image(TTF_RenderText_Blended(font, text.c_str(), color));
}

