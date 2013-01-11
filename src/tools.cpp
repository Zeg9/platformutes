#include <sstream>
#include <vector>
#include "tools.h"

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

std::vector<std::string> split(std::string in, char sep)
{
	std::vector<std::string> r;
	std::string current("");
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (in[i] == sep)
		{
			r.push_back(current);
			current = "";
		}
		else
			current += in[i];
	}
	r.push_back(current);
	return r;
}

std::string strip(std::string in, std::string what)
{
	std::string out("");
	for (int i = 0; i < in.size(); i++)
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
	for (int i = 0; i < in.size(); i++)
		out += tolower(in[i]);
	return out;
}

