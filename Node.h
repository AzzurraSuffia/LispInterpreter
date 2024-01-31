#ifndef NODE_H
#define NODE_H

class Visitor;

//classe nodo per la corretta dealoccazione degli oggetti
class Node {
public:
	virtual ~Node() {};
	virtual void accept(Visitor* v) = 0;
};

#endif

