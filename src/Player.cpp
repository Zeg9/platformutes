#include "Sprite.h"
#include "Player.h"

Player::Player() : Sprite("character", 32, 64) {}
void Player::die()
{
	setPos(32,64);
	setImage("character"); // de-mummify
}

