#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <vector>

std::string tostring(int i);
int toint(std::string s);
bool tobool(std::string s);

int round(int i, int f);

std::vector<std::string> split(std::string in, char sep, unsigned int max=0);
std::string strip(std::string in, std::string what);
std::string stripspaces(std::string in);
std::string lowercase(std::string in);

bool startswith(std::string s, std::string what);

struct vec2
{
	vec2(int _x, int _y);
	int x, y;
};

#endif//__TOOLS_H__
