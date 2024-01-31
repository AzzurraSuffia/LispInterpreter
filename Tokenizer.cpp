#include <cctype>
#include <string>
#include <sstream>
#include "Tokenizer.h"
#include "Exceptions.h"

#include <iostream>

//Gli unici errori che devo valutare qui sono se i token sono corretti
//dal punto di vista lessicale. 
//La valutazione della sintassi viene fatta in un secondo momento.

void Tokenizer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {

	//Leggo il file carattere per carattere
	char ch;
	ch = inputFile.get();
	int lineNumber = 1;

	while (!inputFile.eof()) {

		//Gestione degli spazi vuoti
		if (std::isspace(static_cast<unsigned char>(ch))) {
			//Tengo traccia della riga in cui mi trovo
			if (ch == '\n') {
				lineNumber++;
			}
			//Salto lo spazio bianco e vado avanti
			ch = inputFile.get();
			continue;
		}

		//Creo oggetti Token passando come parametri il codice e la parola associati
		if (ch == '(') {
			inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
		}
		else if (ch == ')') {
			inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
		}

		else if (std::isalpha(static_cast<unsigned char>(ch))) {
			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isalpha(static_cast<unsigned char>(ch))) tmp << ch;
			} while (std::isalpha(static_cast<unsigned char>(ch)));

			//Creazione di Token contenenti parole chiave
			if (tmp.str() == "BLOCK") {
				inputTokens.push_back(Token{ Token::BLOCK, tmp.str() }); 
			}
			else if (tmp.str() == "INPUT") {
				inputTokens.push_back(Token{ Token::INPUT, tmp.str() }); 
			}
			else if (tmp.str() == "PRINT") {
				inputTokens.push_back(Token{ Token::PRINT, tmp.str() }); 
			}
			else if (tmp.str() == "SET") {
				inputTokens.push_back(Token{ Token::SET,   tmp.str() }); 
			}
			else if (tmp.str() == "WHILE") {
				inputTokens.push_back(Token{ Token::WHILE, tmp.str() }); 
			}
			else if (tmp.str() == "IF") {
				inputTokens.push_back(Token{ Token::IF,    tmp.str() }); 
			}
			else if (tmp.str() == "GT") {
				inputTokens.push_back(Token{ Token::GT,    tmp.str() }); 
			}
			else if (tmp.str() == "LT") {
				inputTokens.push_back(Token{ Token::LT,    tmp.str() }); 
			}
			else if (tmp.str() == "EQ") {
				inputTokens.push_back(Token{ Token::EQ,    tmp.str() }); 
			}
			else if (tmp.str() == "AND") {
				inputTokens.push_back(Token{ Token::AND,   tmp.str() }); 
			}
			else if (tmp.str() == "OR") {
				inputTokens.push_back(Token{ Token::OR,    tmp.str() }); 
			}
			else if (tmp.str() == "NOT") {
				inputTokens.push_back(Token{ Token::NOT,   tmp.str() }); 
			}
			else if (tmp.str() == "TRUE") {
				inputTokens.push_back(Token{ Token::TRUE,  tmp.str() }); 
			}
			else if (tmp.str() == "FALSE") {
				inputTokens.push_back(Token{ Token::FALSE, tmp.str() }); 
			}
			else if (tmp.str() == "ADD") {
				inputTokens.push_back(Token{ Token::ADD,   tmp.str() }); 
			}
			else if (tmp.str() == "SUB") {
				inputTokens.push_back(Token{ Token::SUB,   tmp.str() }); 
			}
			else if (tmp.str() == "MUL") {
				inputTokens.push_back(Token{ Token::MUL,   tmp.str() }); 
			}
			else if (tmp.str() == "DIV") {
				inputTokens.push_back(Token{ Token::DIV,   tmp.str() }); 
			}
			else {
				inputTokens.push_back(Token{ Token::ID,    tmp.str() }); 
			}
			continue;
		}
		//Gestione dei numeri negativi
		else if (ch == '-') {
			std::stringstream tmp{};
			tmp << ch;
			ch = inputFile.get();
			if (std::isdigit(static_cast<unsigned char>(ch))) {
				tmp << ch;
				do {
					ch = inputFile.get();
					if (std::isdigit(static_cast<unsigned char>(ch))) tmp << ch;
				} while (std::isdigit(static_cast<unsigned char>(ch)));

				inputTokens.push_back(Token{ Token::NUM, tmp.str() });
				continue;
			}
			else {
				tmp << " '-' is not followed by a number in input at line " << lineNumber;
				throw LexicalError(tmp.str());
			}
		}

		//Gestione dei numeri postivi
		else if (std::isdigit(static_cast<unsigned char>(ch))) {

			std::stringstream tmp{};
			tmp << ch;
			do {
				ch = inputFile.get();
				if (std::isdigit(static_cast<unsigned char>(ch))) tmp << ch;
			} while (std::isdigit(static_cast<unsigned char>(ch)));

			inputTokens.push_back(Token{ Token::NUM, tmp.str() });
			continue;
		} else {
			std::stringstream tmp{};
			tmp << "stray character " << ch << " in input at line " << lineNumber;
			throw LexicalError(tmp.str());
		}

		//Prendo il prossimo carattere da valutare
		ch = inputFile.get();

	}

	//Se il file è vuoto, va restituito errore
	if (inputTokens.empty()) {
		throw ParseError("Empty Input");
	}
}
