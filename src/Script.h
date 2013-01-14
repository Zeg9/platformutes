#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <map>
#include <string>
#include "tools.h"

std::map<std::string, std::string> &getScriptVars();

class Sprite;
void runScript(std::string script, Sprite*sprite=0, vec2 pos=vec2(0,0));


#endif//__SCRIPT_H__
