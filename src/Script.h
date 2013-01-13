#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "tools.h"
#include <string>

class Sprite;
void runScript(std::string script, Sprite*sprite=0, vec2 pos=vec2(0,0));

#endif//__SCRIPT_H__
