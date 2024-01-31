#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "Exceptions.h"
#include "Token.h"
#include "Tokenizer.h"
#include "NumExpr.h"
#include "NodeManager.h"
#include "Parser.h"
#include "Visitor.h"

int main(int argc, const char* argv[]) {

	if (argc < 2) {
		std::cerr << "File not specified!" << std::endl;
		std::cerr << "Use: " << argv[0] << " <nome_file>" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream inputFile;
	try {
		inputFile.open(argv[1]);
		if (!inputFile.is_open()) {
			throw std::runtime_error("File not found or could not be opened.");
		}

	}
	catch (std::exception& exc) {
		std::cerr << "Unable to open " << argv[1] << std::endl;
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}



	//FASE DI TOKENIZZAZIONE
	//Dichiarazione di oggetto Tokenizer, chiamato tokenize
	Tokenizer tokenize;
	//Dichiarazione di un vector di Token, chiamato inputTokens
	std::vector<Token> inputTokens;

	try {
		//Assegnamento del risultato di tokenize(inputFile) a inputTokens
		inputTokens = std::move(tokenize(inputFile));
		//Chiusura del file
		inputFile.close();
	}
	catch (LexicalError& le) {
		std::cerr << "(ERROR in tokenizer: ";
		std::cerr << le.what() << ")" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception& exc) {
		std::cerr << "Unable to read from " << argv[1] << std::endl;
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}

	//FASE DI PARSING
	//Creo il manager dei nodi
	NodeManager manager;

	//Creo il function object per il parsing
	Parse parse{ manager };


	try {
		Program* expr = (parse(inputTokens));
		//std::cout << "Your program prints: " << std::endl;
		EvaluationVisitor* v = new EvaluationVisitor();
		expr->accept(v);

		//Codice da usare per stampare il programma
		/*
		PrintVisitor* p = new PrintVisitor();
		std::cout << "Your program is: " << std::endl;
		expr->accept(p);
		std::cout << std::endl;
		*/
	}
	catch (ParseError& pe) {
		std::cerr << "(ERROR in parser: ";
		std::cerr << pe.what() << ")" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::invalid_argument& arg) {
		std::cerr << "Input Error" << std::endl;
		std::cerr << arg.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (EvaluationError& arg) {
		std::cerr << "(ERROR in evaluator: ";
		std::cerr << arg.what() << ")" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception& exc) {
		std::cerr << "Generic Error" << std::endl;
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}