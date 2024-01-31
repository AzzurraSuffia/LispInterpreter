#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

class Tokenizer {
public:

	std::vector<Token> operator()(std::ifstream& inputFile) {
		std::vector<Token> inputTokens;
		tokenizeInputFile(inputFile, inputTokens);
		return inputTokens;
	}

private:
	//Man mano che il file viene letto il vettore di Token viene riempito
	void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);

};

#endif
