#ifndef NUMEXPR_H
#define NUMEXPR_H

#include <string>
#include "Node.h"
#include <vector>
#include "Visitor.h"
#include <unordered_map>

class Visitor;

//Clase base astratta NumExpr
class NumExpr : public Node {
public:
	virtual ~NumExpr() {};


};

//Classe derivata per espressioni numeriche contenenti un numero
class Number : public NumExpr {
public:
	//costruttore con parametri
	Number(int64_t v) : value{ v } { }
	//Distruttore
	~Number() = default;
	//Costruttore di copia
	Number(const Number& other) = default;
	//Eliminiamo la possibilità di usare l'assegnamento (oggetto immutabile)
	Number& operator=(const Number& other) = delete;

	//metodi di accesso
	int64_t get_ValueN() const {
		return value;
	}
	
	void accept(Visitor* v) override;
	

private:
	int64_t value;
};

//Classe derivata per espressioni numeriche contenenti un'operatore
class Operator : public NumExpr {
public:

	enum OpCode { PLUS, MINUS, TIMES, DIV };

	//costruttore con parametri
	Operator(OpCode o, NumExpr* lop, NumExpr* rop) : op{ o }, left{ lop }, right{ rop } { }
	//Distruttore
	//Il distruttore di default implica che la deallocazione degli operandi
	//non è sua responsabilità, ma di qualcun altro
	~Operator() = default;
	//Costruttore di copia
	Operator(const Operator& other) = default;
	//Eliminiamo la possibilità di usare l'assegnamento
	Operator& operator=(const Operator& other) = delete;

	//metodi di accesso
	OpCode get_Op() const {
		return op;
	}

	NumExpr* get_Left() const {
		return left;
	}

	NumExpr* get_Right() const {
		return right;
	}

	void accept(Visitor* v) override;
	
	static std::string opCodeTostring(OpCode op) {
		switch (op) {
		case PLUS:  return "ADD";
		case MINUS: return "SUB";
		case TIMES: return "MUL";
		case DIV:   return "DIV";
		}
	}

private:
	OpCode op;
	NumExpr* left;
	NumExpr* right;
};

//Classe derivata per espressioni numeriche contenente una variabile
class Variable : public NumExpr {
public:
	//costruttore con parametri
	Variable(std::string n) : id{ n } { }
	//Distruttore
	~Variable() = default;
	//Costruttore di copia
	Variable(const Variable& other) = default;
	//Assegnamento
	Variable& operator=(const Variable& other) = delete;


	std::string get_Id() const {
		return id;
	}

	static std::unordered_map<std::string, int64_t > list_Variables;
	
	void accept(Visitor* v) override;
	

private:
	std::string  id;
};


#endif
