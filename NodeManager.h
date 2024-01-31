#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <vector>
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Node.h"
#include "Program.h"

//gestore dei nodi per evitare possibili "perdite di memoria"
class NodeManager {
public:

	NodeManager() = default;
	//distruttore
	~NodeManager() {
		clearMemory();
	}

	//impedisco copia e assegnamento
	NodeManager(const NodeManager& other) = delete;
	NodeManager& operator=(const NodeManager& other) = delete;

	//factory method per la creazione degli oggetti
	Node* makeNumber(int64_t value);
	Node* makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r);
	Node* makeVariable(std::string id);
	Node* makeBoolConst(bool value);
	Node* makeBoolOp(BoolOp::OpCode op, BoolExpr* l, BoolExpr* r);
	Node* makeRelOp(RelOp::OpCode op, NumExpr* l, NumExpr* r);
	Node* makeSetStmt(Variable* id, NumExpr* v);
	Node* makePrintStmt(NumExpr* o);
	Node* makeInputStmt(Variable* id);
	Node* makeIfStmt(BoolExpr* c, Block* t, Block* f);
	Node* makeWhileStmt(BoolExpr* c, Block* l);
	Node* makeBlock(std::vector<Statement*> s, bool k);
	Node* makeProgram(Block* p);

	void clearMemory() {

		for (auto i = allocated.begin(); i != allocated.end(); i++) {
			delete(*i);
		}
		allocated.resize(0);
	}

private:
	std::vector<Node*> allocated;
};


#endif
