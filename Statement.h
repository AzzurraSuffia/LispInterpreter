#ifndef STATEMENT_H
#define STATEMENT_H

#include "NumExpr.h"
#include "BoolExpr.h"
#include "Node.h"

class Visitor;
class Block;

//classe base astratta statement
class Statement : public Node {
public:
	virtual ~Statement() {};
	
};

//oggetto immutabile 
class PrintStmt : public Statement {
public:
	//Costruttore con parametri
	PrintStmt(NumExpr* o) : output{ o } { }
	//Distruttore
	~PrintStmt() = default; 
	//Costruttore di copia
	PrintStmt(const PrintStmt& other) = default;
	//assegnamento
	PrintStmt& operator=(const PrintStmt& other) = delete;

	//metodi di accesso
	NumExpr* get_Output() const {
		return output;
	}

	void accept(Visitor* v) override;

private:
	NumExpr* output;
};

//oggetto immutabile
class SetStmt : public Statement {
public:
	//costruttore
	SetStmt(Variable* v, NumExpr* n) : variable_id{ v }, value{ n } { }
	//Distruttore
	~SetStmt() = default;
	//copia
	SetStmt(const SetStmt& s) = default;
	//assegnamento
	SetStmt& operator=(const SetStmt& s) = delete;

	//metodi di accesso
	Variable* get_Variable() const {
		return variable_id;
	}

	NumExpr* get_ValueS() const {
		return value;
	}

	void accept(Visitor* v) override;

private:
	Variable* variable_id;
	NumExpr* value;
};

//oggetto immutabile
class InputStmt : public Statement {
public:
	//costruttore
	InputStmt(Variable* v) : variable_id{ v } { }
	//distruttore
	~InputStmt() = default;
	//copia
	InputStmt(const InputStmt& i) = default;
	//assegnamento
	InputStmt& operator=(const InputStmt& i) = delete;

	//metodo di accesso
	Variable* get_Variable() const {
		return variable_id;
	}

	void accept(Visitor* v) override;

private:
	Variable* variable_id;
};

//oggetto immutabile
class WhileStmt : public Statement {
public:
	//Costruttore con parametri
	WhileStmt(BoolExpr* c, Block* s) : condition{ c }, loop{ s } { }
	//Distruttore
	~WhileStmt() = default;
	//copia
	WhileStmt(const WhileStmt& other) = default;
	//assegnamento
	WhileStmt& operator=(const WhileStmt& other) = delete;

	//metodi di accesso
	BoolExpr* get_Condition() const {
		return condition;
	}

	Block* get_List() const {
		return loop;
	}

	void accept(Visitor* v) override;

private:
	BoolExpr* condition;
	Block* loop;
};

//oggetto immutabile
class IfStmt : public Statement {
public:
	//Costruttore con parametri
	IfStmt(BoolExpr* c, Block* r, Block* l) :
		condition{ c }, truecond{ r }, falsecond{ l } { }
	//Distruttore
	~IfStmt() = default;
	//Copia
	IfStmt(const IfStmt& other) = default;
	//assegnamento
	IfStmt& operator=(const IfStmt& other) = delete;

	//metodi di accesso
	BoolExpr* get_Condition() const {
		return condition;
	}

	Block* get_Truelist() const {
		return truecond;
	}

	Block* get_Falselist() const {
		return falsecond;
	}

	void accept(Visitor* v) override;

private:
	BoolExpr* condition;
	Block* truecond;
	Block* falsecond;
};


#endif
