#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <vector>
#include "Level.h"
#include "tools.h"

class Sprite;

class Environment
{
	friend Environment &getEnvironment();
	public:
		Level lvl;
		pos ppos;
		std::vector<Sprite*> sprites;
		void renderSprites();
		void step();
		void quit();
	private:
		Environment();
		~Environment();
};

Environment &getEnvironment();

#endif//__ENVIRONMENT_H__
