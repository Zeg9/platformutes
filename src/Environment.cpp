#include "Sprite.h"
#include "Environment.h"

#include <iostream>

Environment::Environment() : ppos(32,64) {}
Environment::~Environment()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		delete sprites[i];
	}
	std::cout << "== Bye !" << std::endl;
}

void Environment::renderSprites()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
		sprites[i]->render();
}

void Environment::step()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
		sprites[i]->step();
}

Environment &getEnvironment()
{
	static Environment environment;
	return environment;
}

