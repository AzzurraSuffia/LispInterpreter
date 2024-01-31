#include "NumExpr.h"


void Number::accept(Visitor* v) {
	v->visitNumber(this);
}

void Operator::accept(Visitor* v) {
	v->visitOperator(this);
}

void Variable::accept(Visitor* v) {
    v->visitVariable(this);
}

static std::unordered_map<std::string, int> list_Variables = {};