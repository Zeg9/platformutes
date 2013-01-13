#include "Sprite.h"
#include "Player.h"

Player::Player() : Sprite("common.character", 32, 64) {}
void Player::die()
{
	setPos(32,64);
	setImage("common.character"); // de-mummify
}

