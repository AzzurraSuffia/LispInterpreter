#ifndef TOKEN_H
#define TOKEN_H

#include <string>


struct Token {
	//Di seguito gli identificativi dei token
	static constexpr int BLOCK = 0;
	static constexpr int INPUT = 1;
	static constexpr int PRINT = 2;
	static constexpr int SET = 3;
	static constexpr int WHILE = 4;
	static constexpr int IF = 5;
	static constexpr int GT = 6;
	static constexpr int LT = 7;
	static constexpr int EQ = 8;
	static constexpr int AND = 9;
	static constexpr int OR = 10;
	static constexpr int NOT = 11;
	static constexpr int TRUE = 12;
	static constexpr int FALSE = 13;
	static constexpr int ADD = 14;
	static constexpr int SUB = 15;
	static constexpr int MUL = 16;
	static constexpr int DIV = 17;
	static constexpr int LP = 18;
	static constexpr int RP = 19;
	static constexpr int NUM = 20;
	static constexpr int ID = 21;

	static constexpr const char* id2word[]{
		"BLOCK", "INPUT", "PRINT", "SET", "WHILE", "IF", "GT", "LT", "EQ", "AND", "OR", "NOT", "TRUE", "FALSE", "ADD", "SUB", "MUL", "DIV", "(", ")", "NUM", "ID"
	};

	//costruttori con argomenti
	Token(int t, const char* w) : tag{ t }, word{ w } { }
	Token(int t, std::string w) : tag{ t }, word{ w } { }

	int tag;
	std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif

