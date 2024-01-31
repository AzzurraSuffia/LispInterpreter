#ifndef PARSER_H
#define PARSER_H

#include <vector>


#include "NumExpr.h"
#include "BoolExpr.h"
#include "Statement.h"
#include "NodeManager.h"
#include "Exceptions.h"
#include "Token.h"

#include <iostream>

struct Token;
struct ParseError;

class Parse {

public:
	//costruttore di copia
	Parse(NodeManager& manager) : em{ manager } { }

	Program* operator()(const std::vector<Token>& tokenStream) {
		auto tokenItr = tokenStream.begin();
		streamEnd = tokenStream.end();
		Program* prg = ParseProgram(tokenItr);
		if (tokenItr != (streamEnd - 1)) {
		throw ParseError("Unexcepted Token or Tokens");
		}
		
		return prg;
	}



private:
	std::vector<Token>::const_iterator streamEnd;

	//Manager
	NodeManager& em;

	//metodo con possibilità di ricorsione per il parsing di espressioni aritmetiche
	NumExpr* ParseNumExpr(std::vector<Token>::const_iterator& tokenItr);
	//metodo con possibilità di ricorsione per il parsing di espressioni booleane
	BoolExpr* ParseBoolExpr(std::vector<Token>::const_iterator& tokenItr);
	//metodo con possibilità di ricorsione per il parsing degli statement
	Statement* ParseStatement(std::vector<Token>::const_iterator& tokenItr);
	//metodo con possibilità di ricorsione per il parsing di programmi
	Program* ParseProgram(std::vector<Token>::const_iterator& tokenItr);
	//metodo con possibilità di ricorsione per il parsing di block statement
	Block* ParseBlock(std::vector<Token>::const_iterator& tokenItr);

	//metodo per l'avanzamento sicuro dell'iteratore
	void safe_next(std::vector<Token>::const_iterator& itr) {
		itr++;	
		if (itr == streamEnd) {
			throw ParseError("overflow in token stream");
		}
	}

	

};

#endif
