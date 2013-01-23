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

#include "Video.h"
#include "Font.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "tools.h"
#include "ResourceMgr.h"

#include "game.h" // for testing levels

#include "editor.h"

#define EDITOR_FADE_RESET editor_fade = 0; editor_fadetime = SDL_GetTicks()+1000;
#define EDITOR_TILE_PREV if (editor_currenttile > 0) editor_currenttile --; else editor_currenttile = tiles.size()-1;
#define EDITOR_TILE_NEXT if (editor_currenttile < tiles.size()-1) editor_currenttile ++; else editor_currenttile = 0;

void startEditor()
{
	Device &d = getDevice();
	d.showCursor(true);
	unsigned int editor_currenttile(0), editor_fade(TILE_WIDTH), editor_fadetime(0);
	bool showRaw = false;
	PLAYER->enablePhysics(false);
	ENV.allowSprites = false;
	Level &lvl = ENV.lvl;
	lvl.load(FIRST_LEVEL);
	PLAYER->die();
	SDL_Event e;
	bool end(false);
	while (d.run() && !end)
	{
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-PPOS.x%TILE_WIDTH, dy = y-PPOS.y%TILE_HEIGHT;
				if (PPOS.x < d.getWidth()/2 || PPOS.x > lvl.getWidth()*TILE_WIDTH-d.getWidth()/2)
					dx = x;
				if (PPOS.y < d.getHeight()/2 || PPOS.y+TILE_HEIGHT > lvl.getHeight()*TILE_WIDTH-d.getHeight()/2)
					dy = y;
				d.drawImage(lvl.getBackground(),dx,dy);
			}
		}
		// update position
		ENV.step();
		vec2 min (
			d.getWidth()/2/TILE_WIDTH*TILE_WIDTH,
			d.getHeight()/2/TILE_HEIGHT*TILE_HEIGHT);
		vec2 max (
			lvl.getWidth()*TILE_WIDTH-d.getWidth()/2/TILE_WIDTH*TILE_WIDTH,
			lvl.getHeight()*TILE_HEIGHT-d.getHeight()/2/TILE_HEIGHT*TILE_HEIGHT);
		if (PPOS.x < min.x) PLAYER->setPos(min.x, PPOS.y);
		if (PPOS.x > max.x) PLAYER->setPos(max.x, PPOS.y);
		if (PPOS.y < min.y) PLAYER->setPos(PPOS.x, min.y);
		if (PPOS.y > max.y) PLAYER->setPos(PPOS.x, max.y);
		// render
		ENV.render();
		// level editor
		d.drawImage(getResourceMgr().getImage("common/editor/spawn"),getDrawPos(vec2(32,64)));
		std::vector<int> tiles = lvl.getTileset()->getValidTiles();
		if (editor_fade < TILE_WIDTH)
		{
			for (int i = -5; i <= 5; i++)
			{
				d.drawImage(getResourceMgr().getImage("common/editor/slot"),
					d.getWidth()-TILE_WIDTH+editor_fade,
					d.getHeight()/2-TILE_HEIGHT/2+i*TILE_HEIGHT);
				int ci = (int)editor_currenttile + i;
				if (ci < 0) ci = tiles.size()+ci;
				if (ci >= (int)tiles.size()) ci = ci%tiles.size();
				Tile &t = lvl.getTileset()->get(tiles[ci]);
				if (!t.isAir())
				{
					d.drawImage(t.getImage(),
						d.getWidth()-TILE_WIDTH+editor_fade,
						d.getHeight()/2-TILE_HEIGHT/2+i*TILE_HEIGHT,
						0,0, TILE_WIDTH, TILE_HEIGHT);
				}
			}
			d.drawImage(getResourceMgr().getImage("common/editor/select"),
				d.getWidth()-TILE_WIDTH+editor_fade,
				d.getHeight()/2-TILE_HEIGHT/2);
			if (SDL_GetTicks() > editor_fadetime)
				editor_fade+=2;
		}
		int x, y;
		SDL_GetMouseState(&x, &y);
		Tile &tile = lvl.getTileset()->get(tiles[editor_currenttile]);
		d.drawImage(tile.getImage(), x-TILE_WIDTH/2, y-TILE_HEIGHT/2);
		if (tile.hasShading())
		{
			d.drawImage(getResourceMgr().getImage("common/shading/t"), x-TILE_WIDTH/2, y-TILE_HEIGHT/2);
			d.drawImage(getResourceMgr().getImage("common/shading/b"), x-TILE_WIDTH/2, y-TILE_HEIGHT/2);
			d.drawImage(getResourceMgr().getImage("common/shading/l"), x-TILE_WIDTH/2, y-TILE_HEIGHT/2);
			d.drawImage(getResourceMgr().getImage("common/shading/r"), x-TILE_WIDTH/2, y-TILE_HEIGHT/2);
		}
		getResourceMgr().getFont("common/FreeMono&16")->render(
			tostring(tiles[editor_currenttile]),255,255,255,
			x-TILE_WIDTH/2, y-TILE_HEIGHT/2, ALIGN_LEFT, ALIGN_BOTTOM);
		if (showRaw)
		{
			getResourceMgr().getFont("common/FreeMono&16")->render(
				lvl.getTileset()->get(tiles[editor_currenttile]).getRawData(),255,255,255);
		}
		// we're done, let's render
		d.render();
		// handle events
		Uint8 buttons = SDL_GetMouseState(&x,&y);
		vec2 p = getRealPos(vec2(x,y));
		if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
			lvl.set(p.x/TILE_WIDTH, p.y/TILE_HEIGHT,
				lvl.getTileset()->getValidTiles()[editor_currenttile]);
		if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
			lvl.set(p.x/TILE_WIDTH, p.y/TILE_HEIGHT, 0);
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					vec2 p = getRealPos(vec2(e.button.x, e.button.y));
					switch (e.button.button)
					{
						case SDL_BUTTON_LEFT:
							lvl.set(p.x/TILE_WIDTH, p.y/TILE_HEIGHT,
								lvl.getTileset()->getValidTiles()[editor_currenttile]);
							break;
						case SDL_BUTTON_RIGHT:
							lvl.set(p.x/TILE_WIDTH, p.y/TILE_HEIGHT, 0);
							break;
						case SDL_BUTTON_WHEELUP:
							EDITOR_TILE_PREV
							EDITOR_FADE_RESET
							break;
						case SDL_BUTTON_WHEELDOWN:
							EDITOR_TILE_NEXT
							EDITOR_FADE_RESET
							break;
						case SDL_BUTTON_MIDDLE:
							for (unsigned int i = 0; i < tiles.size(); i++)
							{
								if (tiles[i]==lvl.getId(p.x/TILE_WIDTH, p.y/TILE_HEIGHT))
								{
									if (i == editor_currenttile) break;
									editor_currenttile = i;
									EDITOR_FADE_RESET
									break;
								}
							}
							break;
						default:
							break;
					}
				} break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							end = true;
							break;
						case SDLK_LEFT:
							PLAYER->setVel(-5,PLAYER->getVel().y);
							break;
						case SDLK_RIGHT:
							PLAYER->setVel(5,PLAYER->getVel().y);
							break;
						case SDLK_UP:
							PLAYER->setVel(PLAYER->getVel().x,-5);
							break;
						case SDLK_DOWN:
							PLAYER->setVel(PLAYER->getVel().x,5);
							break;
						case SDLK_PAGEDOWN:
							lvl.load_next();
							break;
						case SDLK_MINUS:
						case SDLK_KP_MINUS:
							EDITOR_TILE_PREV
							EDITOR_FADE_RESET
							break;
						case SDLK_PLUS:
						case SDLK_KP_PLUS:
							EDITOR_TILE_NEXT
							EDITOR_FADE_RESET
							break;
						case SDLK_i:
							showRaw = !showRaw;
							break;
						case SDLK_s:
							lvl.save();
							break;
						case SDLK_r:
							lvl.reload();
							break;
						case SDLK_p:
							ENV.allowSprites = true;
							PLAYER->die();
							startGame(false);
							d.showCursor(true);
							PLAYER->enablePhysics(false);
							PLAYER->die();
							ENV.allowSprites = false;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							if (PLAYER->getVel().x < 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						case SDLK_RIGHT:
							if (PLAYER->getVel().x > 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						case SDLK_UP:
							if (PLAYER->getVel().y < 0)
								PLAYER->setVel(PLAYER->getVel().x, 0);
							break;
						case SDLK_DOWN:
							if (PLAYER->getVel().y > 0)
								PLAYER->setVel(PLAYER->getVel().x, 0);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}
}

