#include <sstream>

#include "Token.h"

//possibilità di stampare i Token (e quindi vector di Token)
std::ostream& operator<<(std::ostream& os, const Token& t) {
	std::stringstream tmp;
	tmp << t.tag << ":" << t.word;
	os << tmp.str();
	return os;
}