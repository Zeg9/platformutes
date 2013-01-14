#include <sstream>
#include <vector>
#include "tools.h"

#include <iostream>

std::string tostring(int i)
{
	std::ostringstream oss;
	oss << i;
	return oss.str();
}

int toint(std::string s)
{
	std::istringstream iss(s);
	int i;
	iss >> i;
	return i;
}

bool tobool(std::string s)
{
	if (toint(s)) return true;
	if (lowercase(s)=="true") return true;
	return false;
}

int round(int i, int f)
{
	if (i%f > f/2)
		return (i/f+1)*f;
	return (i/f)*f;
}

std::vector<std::string> split(std::string in, char sep, unsigned int max)
{
	std::vector<std::string> r;
	std::string current("");
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (in[i] == sep && (max==0 || r.size()+1 < max))
		{
			if (current != "")
				r.push_back(current);
			current = "";
		}
		else
			current += in[i];
	}
	if (current != "")
		r.push_back(current);
	return r;
}

std::string strip(std::string in, std::string what)
{
	std::string out("");
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (what.find(in[i]) == what.npos)
			out += in[i];
	}
	return out;
}

std::string stripspaces(std::string in)
{
	return strip(in, " \t\n\r");
}

std::string lowercase(std::string in)
{
	std::string out;
	for (unsigned int i = 0; i < in.size(); i++)
		out += tolower(in[i]);
	return out;
}

bool startswith(std::string s, std::string what)
{
	if (s.size() < what.size()) return false;
	if (s.substr(0,what.size()) == what) return true;
	return false;
}

vec2::vec2(int _x, int _y) :
	x(_x), y(_y)
	{}

bool operator==(vec2 const&a, vec2 const&b)
{
	return a.x == b.x && a.y == b.y;
}

