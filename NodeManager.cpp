#include "NodeManager.h"


Node* NodeManager::makeNumber(int64_t value) {
	//creo dinamicamente un oggetto passandogli i parametri
	Node* n = new Number(value);
	//tengo traccia della sua esistenza in allocated
	allocated.push_back(n);
	return n;
}

Node* NodeManager::makeVariable(std::string id) {
	Node* v = new Variable(id);
	allocated.push_back(v);
	return v;
}

Node* NodeManager::makeBoolConst(bool value) {
	Node* c = new BoolConst(value);
	allocated.push_back(c);
	return c;
}

Node* NodeManager::makeBoolOp(BoolOp::OpCode op, BoolExpr* l, BoolExpr* r) {
	Node* o = new BoolOp(op, l, r);
	allocated.push_back(o);
	return o;
}

Node* NodeManager::makeRelOp(RelOp::OpCode op, NumExpr* l, NumExpr* r) {
	Node* o = new RelOp(op, l, r);
	allocated.push_back(o);
	return o;
}

Node* NodeManager::makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r) {
	Node* o = new Operator(op, l, r);
	allocated.push_back(o);
	return o;
}

Node* NodeManager::makeSetStmt(Variable* id, NumExpr* v) {
	Node* s = new SetStmt(id, v);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makePrintStmt(NumExpr* o) {
	Node* s = new PrintStmt(o);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makeInputStmt(Variable* id) {
	Node* s = new InputStmt(id);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makeIfStmt(BoolExpr* c, Block* t, Block* f) {
	Node* s = new IfStmt(c, t, f);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makeWhileStmt(BoolExpr* c, Block* l) {
	Node* s = new WhileStmt(c, l);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makeBlock(std::vector<Statement*> b, bool k) {
	Node* s = new Block(b, k);
	allocated.push_back(s);
	return s;
}

Node* NodeManager::makeProgram(Block* p) {
	Node* s = new Program(p);
	allocated.push_back(s);
	return s;
}
