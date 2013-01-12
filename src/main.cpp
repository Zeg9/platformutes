#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Image.h"
#include "Level.h"
#include "Tileset.h"
#include "ResourceMgr.h"
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
		// you have no argument !
		std::cerr << "Usage: " << argv[0] << " [<level>]" << std::endl;
		return 1;
	}
	std::string lvlpath("../data/levels/test.pmlvl");
	if (argc == 2)
		lvlpath = argv[1];
	Device &d = getDevice();
	// TODO I DON'T WANNA SEE THIS HERE !
	Image *c = getResourceMgr().getImage("sprites/character/stand");
	int cx(32), cy(64), ncx(cx), ncy(cy), cxv(0), cyv(1); // character position and velocity; TODO be implemented elsewhere
	Level lvl(lvlpath);
	// </TODO>
	SDL_Event e;
	while (d.run())
	{
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-cx%BW, dy = y-cy%BH;
				if (cx < d.getWidth()/2 || cx > lvl.getWidth()*BW-d.getWidth()/2)
					dx = x;
				if (cy < d.getHeight()/2 || cy+BH > lvl.getHeight()*BW-d.getHeight()/2)
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
				int x = sx - d.getWidth()/2/BW+cx/BW;
				int y = sy - d.getHeight()/2/BW+cy/BH;
				int dx = sx*BW-cx%BW;
				int dy = sy*BH-cy%BH;
				if (cx < d.getWidth()/2)
				{
					x = sx;
					dx = sx*BW;
				}
				else if (cx > lvl.getWidth()*BW-d.getWidth()/2)
				{
					x = lvl.getWidth()-d.getWidth()/BW+sx;
					dx = sx*BW;
				}
				if (cy < d.getHeight()/2)
				{
					y = sy;
					dy = sy*BH;
				}
				else if (cy+BH > lvl.getHeight()*BH-d.getHeight()/2)
				{
					y = lvl.getHeight()-d.getHeight()/BH+sy;
					dy = sy*BH;
				}
				if (lvl.get(x,y).isAir()) continue;
				d.drawImage(lvl.get(x,y).getImage(), dx, dy);
				if (!lvl.get(x,y).isSolid()) continue;
				bool t, b, l, r, tr, br, bl, tl;
				int w = lvl.getWidth(), h = lvl.getHeight();
				t = !lvl.get(x,y-1).isSolid();
				b = !lvl.get(x,y+1).isSolid();
				l = !lvl.get(x-1,y).isSolid();
				r = !lvl.get(x+1,y).isSolid();
				tr = !lvl.get(x+1,y-1).isSolid();
				br = !lvl.get(x+1,y+1).isSolid();
				bl = !lvl.get(x-1,y+1).isSolid();
				tl = !lvl.get(x-1,y-1).isSolid();
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
		ncx = cx+cxv; ncy = cy+cyv;
		if (cyv < 0 && !(
			lvl.get(cx/BW,ncy/BH).isSolid() ||
			lvl.get(cx/BW+1,ncy/BH).isSolid()
			))
			cy = ncy;
		else if (cyv < 0)
		{
			cy = round(cy,BH);
			cyv = 0;
		}
		if (cyv > 0 && !(
			lvl.get(cx/BW,(ncy-1+BH*2)/BH).isSolid() ||
			lvl.get((cx-1)/BW+1,(ncy-1+BH*2)/BH).isSolid()
			))
			cy = ncy;
		else if (cyv > 0)
		{
			cy = round(cy,BH);
			cyv = 0;
		}
		if (cxv < 0 && !(
			lvl.get(ncx/BW, cy/BH).isSolid() ||
			lvl.get(ncx/BW, (cy-1+BH)/BH).isSolid() ||
			lvl.get(ncx/BW, (cy-1+BH*2)/BH).isSolid()
			))
			cx = ncx;
		else if (cxv < 0)
			cx = round(cx,BW);
		if (cxv > 0 && !(
			lvl.get((ncx-1)/BW+1, cy/BH).isSolid() ||
			lvl.get((ncx-1)/BW+1, (cy-1+BH)/BH).isSolid() ||
			lvl.get((ncx-1)/BW+1, (cy-1+BH*2)/BH).isSolid()
			))
			cx = ncx;
		else if (cxv > 0)
			cx = round(cx,BW);
		if (cyv < 5)
			cyv += 1;
		if (
			lvl.get(cx/BW, (cy-1+BH*2)/BH).getHurt() ||
			lvl.get(cx/BW+1, (cy-1+BH*2)/BH).getHurt()
			)
		{ cx = 32; cy = 64; }
		int dx = d.getWidth()/2/BW*BW;
		int dy = d.getHeight()/2/BH*BH;
		if (cx < d.getWidth()/2)
			dx = cx;
		else if (cx > lvl.getWidth()*BW-d.getWidth()/2)
			dx = cx - lvl.getWidth()*BW+d.getWidth();
		if (cy < d.getHeight()/2)
			dy = cy;
		else if (cy+BH > lvl.getHeight()*BH-d.getHeight()/2)
			dy = cy - lvl.getHeight()*BH+d.getHeight();
		d.drawImage(c, dx, dy);
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
							cxv = -2;
							break;
						case SDLK_RIGHT:
							cxv = 2;
							break;
						case SDLK_UP:
							if (lvl.get(cx/BW,cy/BW+2).isSolid()
							 || lvl.get(cx/BW+1,cy/BW+2).isSolid())
								cyv = -10;
							break;
						case SDLK_p:
							std::cout << cx << ',' << cy << std::endl;
							break;
						case SDLK_BACKSPACE:
							cx = 32; cy = 64;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							if (cxv < 0) cxv = 0;
							break;
						case SDLK_RIGHT:
							if (cxv > 0) cxv = 0;
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

