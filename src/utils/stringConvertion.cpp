#include "stringConvertion.h"
#include <sstream>

bool convert(std::string& s, int& i)
{
	std::istringstream iss(s);
	return (iss>>i) && (iss.eof());
}
bool convert(std::string& s, float& i)
{
	std::istringstream iss(s);
	return (iss>>i) && (iss.eof());
}
