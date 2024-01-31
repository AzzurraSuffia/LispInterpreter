#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include <string>
#include "Node.h"
#include "NumExpr.h"

class Visitor;

//Classe base astratta BoolExpr
class BoolExpr : public Node {
	//il distruttore virtual della classe base mi consente di deallocare correttamente 
	//con puntatori BoolExpr gli oggetti derivati 
public:
	virtual ~BoolExpr() {};
	
};

//Classe derivata per espressioni booleane contenenti una costante booleana
class  BoolConst : public BoolExpr {
public:
	//Costruttore con parametri
	BoolConst(bool v) : value{ v } { }
	//Distruttore
	~BoolConst() = default;
	//Costruttore di copia
	BoolConst(const BoolConst& other) = default;
	//Eliminiamo la possibilità di usare l'assegnamento (oggetto immutabile)
	BoolConst& operator=(const BoolConst& other) = delete;

	//metodi di accesso
	bool get_Value() const {
		return value;
	}

	void accept(Visitor* v) override;

private:
	bool value;
};

//Classe derivata per espressioni booleane contenenti un operatore booleano
class BoolOp : public BoolExpr {
public:

	enum OpCode { AND, OR, NOT };

	//Costruttore con parametri
	BoolOp(OpCode o, BoolExpr* lop, BoolExpr* rop) : op{ o }, left{ lop }, right{ rop } { }
	//Distruttore
	//Il distruttore di default implica che venga deallocato solo l'operatore
	//La deallocazione degli operandi è responsabilità di qualcun altro
	~BoolOp() = default;
	//Costruttore di copia
	BoolOp(const BoolOp& other) = default;
	//Eliminiamo la possibilità di usare l'assegnamento (oggetto immutabile)
	BoolOp& operator=(const BoolOp& other) = delete;

	//metodi di accesso
	OpCode get_Op() const {
		return op;
	}

	BoolExpr* get_Left() const {
		return left;
	}

	BoolExpr* get_Right() const {
		return right;
	}

	//visitor e metodi di utilità
	void accept(Visitor* v) override;
	

	static std::string opCodeTostring(OpCode op) {
		switch (op) {
		case AND: return "AND";
		case OR: return "OR";
		case NOT: return "NOT";
		}
	}

private:
	OpCode op;
	BoolExpr* right;
	BoolExpr* left;

};

//Classe derivata per espressioni booleane contenenti un operatore relazione
class RelOp : public BoolExpr {
public:

	enum OpCode { LT, GT, EQ };

	//Costruttore con parametri
	RelOp(OpCode o, NumExpr* lop, NumExpr* rop) : op{ o }, left{ lop }, right{ rop } { }
	//Distruttore
	//Il distruttore di default implica che venga deallocato solo l'operatore
	//La deallocazione degli operandi è responsabilità di qualcun altro
	~RelOp() = default;
	//Costruttore di copia
	RelOp(const RelOp& other) = default;
	//Eliminiamo la possibilità di usare l'assegnamento (oggetto immutabile)
	RelOp& operator=(const RelOp& other) = delete;

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

	//visitor e metodi di utilità
	void accept(Visitor* v) override;
	

	static std::string opCodeTostring(OpCode op) {
		switch (op) {
		case GT: return "GT";
		case LT: return "LT";
		case EQ: return "EQ";
		}
	}

private:
	OpCode op;
	NumExpr* right;
	NumExpr* left;
};

#endif