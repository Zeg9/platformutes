#include "Sprite.h"
#include "Environment.h"

#include <iostream>

Environment::Environment() :
	player(new Sprite("character",32,64,32,64))
{}
Environment::~Environment()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
		delete sprites[i];
	delete player;
	std::cout << "== Bye !" << std::endl;
}

void Environment::render()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
		sprites[i]->render();
	player->render();
}

void Environment::step()
{
	for (unsigned int i = 0; i < sprites.size(); i++)
		sprites[i]->step();
	player->step();
}

Environment &getEnvironment()
{
	static Environment environment;
	return environment;
}

