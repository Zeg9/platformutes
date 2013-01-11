#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <vector>

std::string tostring(int i);
int toint(std::string s);
bool tobool(std::string s);

std::vector<std::string> split(std::string in, char sep);
std::string strip(std::string in, std::string what);
std::string stripspaces(std::string in);

std::string lowercase(std::string in);

#endif//__TOOLS_H__
