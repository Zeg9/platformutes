#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Image.h"
#include "Level.h"
#include "Tileset.h"
#include "Sprite.h"
#include "ResourceMgr.h"
#include "Environment.h"
#include "tools.h"

#define BW BLOCK_WIDTH
#define BH BLOCK_HEIGHT

// WARNING:
// Some (most) of the code here will be moved soon
// It was for testing but it looks like it works great
// so don't complain about "everything is in one file"
// because this will be fixed soon :D

int main(int argc, char ** argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [<level>]" << std::endl;
		return 1;
	}
	std::string lvlpath("../data/levels/pyramid.pmlvl");
	if (argc == 2)
		lvlpath = argv[1];
	Device &d = getDevice();
	Level &lvl = getEnvironment().lvl;
	Sprite *playerSprite = new Sprite("character/stand_r",32,64,32,64);
	getEnvironment().sprites.push_back(playerSprite);
	lvl.load(lvlpath);
	pos &ppos = getEnvironment().ppos;
	SDL_Event e;
	while (d.run())
	{
		ppos = playerSprite->getPos();
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-ppos.x%BW, dy = y-ppos.y%BH;
				if (ppos.x < d.getWidth()/2 || ppos.x > lvl.getWidth()*BW-d.getWidth()/2)
					dx = x;
				if (ppos.y < d.getHeight()/2 || ppos.y+BH > lvl.getHeight()*BW-d.getHeight()/2)
					dy = y;
				d.drawImage(lvl.getBackground(),dx,dy);
			}
		}
		// draw tiles
		// TODO move this ?
		for (int sx = 0; sx < d.getWidth()/BW+1; sx++)
		{
			for (int sy = 0; sy < d.getHeight()/BH+1; sy++)
			{
				int x = sx - d.getWidth()/2/BW+ppos.x/BW;
				int y = sy - d.getHeight()/2/BW+ppos.y/BH;
				int dx = sx*BW-ppos.x%BW;
				int dy = sy*BH-ppos.y%BH;
				if (ppos.x < d.getWidth()/2)
				{
					x = sx;
					dx = sx*BW;
				}
				else if (ppos.x > lvl.getWidth()*BW-d.getWidth()/2)
				{
					x = lvl.getWidth()-d.getWidth()/BW+sx;
					dx = sx*BW;
				}
				if (ppos.y < d.getHeight()/2)
				{
					y = sy;
					dy = sy*BH;
				}
				else if (ppos.y+BH > lvl.getHeight()*BH-d.getHeight()/2)
				{
					y = lvl.getHeight()-d.getHeight()/BH+sy;
					dy = sy*BH;
				}
				if (lvl.get(x,y).isAir()) continue;
				d.drawImage(lvl.get(x,y).getImage(), dx, dy);
				if (!lvl.get(x,y).hasShading()) continue;
				bool t, b, l, r, tr, br, bl, tl;
				int w = lvl.getWidth(), h = lvl.getHeight();
				t = !lvl.get(x,y-1).hasShading();
				b = !lvl.get(x,y+1).hasShading();
				l = !lvl.get(x-1,y).hasShading();
				r = !lvl.get(x+1,y).hasShading();
				tr = !lvl.get(x+1,y-1).hasShading();
				br = !lvl.get(x+1,y+1).hasShading();
				bl = !lvl.get(x-1,y+1).hasShading();
				tl = !lvl.get(x-1,y-1).hasShading();
				if (y > 0 && t)
					d.drawImage(getResourceMgr().getImage("shading/t"),dx,dy);
				if (y < h-1 && b)
					d.drawImage(getResourceMgr().getImage("shading/b"),dx,dy);
				if (x > 0 && l)
					d.drawImage(getResourceMgr().getImage("shading/l"),dx,dy);
				if (x < w-1 && r)
					d.drawImage(getResourceMgr().getImage("shading/r"),dx,dy);
				if (tr && !(t || r))
					d.drawImage(getResourceMgr().getImage("shading/tr"),dx,dy);
				if (br && !(b || r))
					d.drawImage(getResourceMgr().getImage("shading/br"),dx,dy);
				if (bl && !(b || l))
					d.drawImage(getResourceMgr().getImage("shading/bl"),dx,dy);
				if (tl && !(t || l))
					d.drawImage(getResourceMgr().getImage("shading/tl"),dx,dy);
			}
		}
		// draw sprites and character
		// TODO handle this elsewhere
		if (
			ppos.y > lvl.getHeight()*BH ||
			lvl.get(ppos.x/BW, (ppos.y-1+BH*2)/BH).getHurt() ||
			lvl.get(ppos.x/BW+1, (ppos.y-1+BH*2)/BH).getHurt()
			)
		{ ppos.x = 32; ppos.y = 64; }
		playerSprite->setPos(ppos.x, ppos.y);
		getEnvironment().step();
		getEnvironment().renderSprites();
		// </TODO>
		// handle events
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							playerSprite->setVel(-2,playerSprite->getVel().y);
							playerSprite->setImage("character/stand_l");
							break;
						case SDLK_RIGHT:
							playerSprite->setVel(2,playerSprite->getVel().y);
							playerSprite->setImage("character/stand_r");
							break;
						case SDLK_UP:
							if (lvl.get(ppos.x/BW,ppos.y/BW+2).isSolid()
							 || lvl.get((ppos.x-1)/BW+1,ppos.y/BW+2).isSolid())
								playerSprite->setVel(playerSprite->getVel().x,-10);
							break;
						case SDLK_p:
							std::cout << ppos.x << ',' << ppos.y << std::endl;
							break;
						case SDLK_BACKSPACE:
							ppos.x = 32; ppos.y = 64;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							if (playerSprite->getVel().x < 0)
								playerSprite->setVel(0,playerSprite->getVel().y);
							break;
						case SDLK_RIGHT:
							if (playerSprite->getVel().x > 0)
								playerSprite->setVel(0,playerSprite->getVel().y);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
		// we're done, let's render
		d.render();
	}
	// Bye !
	return 0;
}

