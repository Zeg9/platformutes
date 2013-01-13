#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <list>
#include <stack>
#include "Level.h"
#include "Sprite.h"
#include "Player.h"
#include "tools.h"

#define ENV getEnvironment()
#define PLAYER ENV.player
#define PPOS PLAYER->getPos()
#define PVEL PLAYER->getVel()
#define PSIZE PLAYER->getSize()

class Environment
{
	friend Environment &getEnvironment();
	public:
		Level lvl;
		void reset();
		void addSprite(Sprite *s);
		void removeSprite(Sprite *s);
		Player *player;
		void render();
		void step();
		void quit();
	private:
		std::list<Sprite*> sprites;
		std::stack<Sprite*> s2r; // sprites to remove
		Environment();
		~Environment();
};

Environment &getEnvironment();

#endif//__ENVIRONMENT_H__
