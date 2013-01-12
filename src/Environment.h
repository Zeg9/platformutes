#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <vector>
#include "Level.h"
#include "tools.h"

#define ENV getEnvironment()
#define PLAYER ENV.player
#define PPOS PLAYER->getPos()
#define PVEL PLAYER->getVel()
#define PSIZE PLAYER->getSize()

class Sprite;

class Environment
{
	friend Environment &getEnvironment();
	public:
		Level lvl;
		std::vector<Sprite*> sprites;
		Sprite *player;
		void render();
		void step();
		void quit();
	private:
		Environment();
		~Environment();
};

Environment &getEnvironment();

#endif//__ENVIRONMENT_H__
