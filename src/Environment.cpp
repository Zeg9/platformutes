#include "Sprite.h"
#include "Player.h"
#include "Environment.h"

Environment::Environment() :
	allowSprites(true),
	player(new Player())
{}
Environment::~Environment()
{
	for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
		delete (*i);
	delete player;
}

void Environment::reset()
{
	if (allowSprites) for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
			s2r.push((*i));
}

void Environment::addSprite(Sprite *s) {
	if (allowSprites)
		sprites.push_back(s);
}
void Environment::removeSprite(Sprite *s)
{
	s2r.push(s);
}

void Environment::render()
{
	lvl.render();
	if (allowSprites) for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
		(*i)->render();
	player->render();
}

void Environment::step()
{
	if (allowSprites) for (std::list<Sprite*>::iterator i = sprites.begin();
		i != sprites.end(); i++)
			(*i)->step();
	player->step();
	if (allowSprites) while (!s2r.empty())
	{
		sprites.remove(s2r.top());
		delete s2r.top();
		s2r.pop();
	}
}

Environment &getEnvironment()
{
	static Environment environment;
	return environment;
}

