#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include "Statement.h"
#include "Node.h"
#include "Visitor.h"

class Visitor;

//oggetto immutabile
class Block : public Node {
public:
	//costruttore con parametri e distruttore
	Block(std::vector<Statement*> b, bool k) : statement_list{ b }, keyword{ k } {};
	~Block() = default;
	//copia e assegnamento
	Block(const Block& other) = default;
	Block& operator=(const Block& other) = delete;

	//metodi di accesso
	const std::vector<Statement*>& get_StatementList() const {
		return statement_list;
	}

	bool get_Keyword() const {
		return keyword;
	}

	void accept(Visitor* v) override;

private:
	std::vector<Statement*> statement_list;
	bool keyword;
};


//oggetto immutabile
class Program : public Node {
public:
	//costruttore con parametri e distruttoe
	Program(Block* p) : statements{ p } {};
	~Program() = default;
	//copia e assegnamento
	Program(const Program& other) = default;
	Program& operator=(const Program& other) = delete;

	Block* get_Statements() const {
		return statements;
	}

	void accept(Visitor* v) override;

private:
	Block* statements; 
};


#endif
