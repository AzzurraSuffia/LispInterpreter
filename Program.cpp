#include "Program.h"
#include "Visitor.h"

void Block::accept(Visitor* v) {
	v->visitBlockStmt(this);
}

void Program::accept(Visitor* v) {
	v->visitProgram(this);
}

