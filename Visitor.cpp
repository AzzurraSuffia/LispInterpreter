#include "Visitor.h"
#include <iostream>
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Statement.h"
#include "Program.h"
#include  <unordered_map>
#include "Exceptions.h"
#include <sstream>

std::unordered_map<std::string, int64_t> Variable::list_Variables;

//funzione per il controllo dell'esistenza della variabile
void checkVariable(Variable* var) {
    auto it = Variable::list_Variables.find(var->get_Id());
    if (it == Variable::list_Variables.end()) {
        //La variabile non esiste
        std::stringstream tmp{};
        tmp << "udefined variable " << var->get_Id();
        throw EvaluationError(tmp.str());
    }
}

//EVALUATIONVISITOR

void EvaluationVisitor::visitOperator(Operator* opNode) {
    // Prelevo i puntatori agli operandi e gli faccio accettare 
    // questo oggetto come visitatore (propago la visita)
    NumExpr* left = opNode->get_Left();
    if (dynamic_cast<Variable*>(left)) {
        Variable* var = dynamic_cast<Variable*>(left);
        checkVariable(var);
    }
    left->accept(this);
    int64_t lval = accumulator.back(); accumulator.pop_back();
    NumExpr* right = opNode->get_Right();
    if (dynamic_cast<Variable*>(right)) {
        Variable* var = dynamic_cast<Variable*>(right);
        checkVariable(var);
    }
    right->accept(this);
    // Prelevo i valori dall'accumulatore
    int64_t rval = accumulator.back(); accumulator.pop_back();
    //Inserisco nella cima dell'accumulatore il risultato dell'operazione
    switch (opNode->get_Op()) {
    case Operator::PLUS:
        accumulator.push_back(lval + rval); return;
    case Operator::MINUS:
        accumulator.push_back(lval - rval); return;
    case Operator::TIMES:
        accumulator.push_back(lval * rval); return;
    case Operator::DIV: {
        //Controllo sul divisore
        if (rval == 0) {
            throw EvaluationError("division by zero");
        }
        accumulator.push_back((lval/rval)); return; }
    default:
        return;
    }
}


void EvaluationVisitor::visitNumber(Number* numNode) {
    // Il metodo push_back inserisce il valore come ultimo elemento del vector
    accumulator.push_back(numNode->get_ValueN());
}


void EvaluationVisitor::visitVariable(Variable* varNode) {
    //Prelevo il valore della variabile e lo inserisco nella cima dell'accumulatore
    std::string id = varNode->get_Id();
    int64_t value = varNode->list_Variables[id];
    accumulator.push_back(value);
}

void EvaluationVisitor::visitRelOp(RelOp* opNode) {
    // Prelevo i puntatori agli operandi e gli faccio accettare 
    // questo oggetto come visitatore (propago la visita)
    NumExpr* left = opNode->get_Left();
    if (dynamic_cast<Variable*>(left)) {
        Variable* var = dynamic_cast<Variable*>(left);
        checkVariable(var);
    }
    left->accept(this);
    NumExpr* right = opNode->get_Right();
    if (dynamic_cast<Variable*>(right)) {
        Variable* var = dynamic_cast<Variable*>(right);
        checkVariable(var);
    }
    right->accept(this);
    int64_t rval = accumulator.back(); accumulator.pop_back();
    int64_t lval = accumulator.back(); accumulator.pop_back();
    //Uso un accumulatore separato per le operazioni booleane
    switch (opNode->get_Op()) {
    case RelOp::GT:
        bool_accumulator.push_back(lval > rval); return;
    case RelOp::LT:
        bool_accumulator.push_back(lval < rval); return;
    case RelOp::EQ:
        bool_accumulator.push_back(lval == rval); return;
    default:
        return;
    }
}

void EvaluationVisitor::visitBoolOp(BoolOp* opNode) {
    // Prelevo i puntatori agli operandi e gli faccio accettare 
    // questo oggetto come visitatore (propago la visita)
    BoolExpr* left = opNode->get_Left();
    left->accept(this);
    BoolExpr* right = opNode->get_Right();
    right->accept(this);
    bool rval = bool_accumulator.back(); bool_accumulator.pop_back();
    bool lval = bool_accumulator.back(); bool_accumulator.pop_back();
    switch (opNode->get_Op()) {
    case BoolOp::AND:
        if (!lval) {
            //l'AND è cortocircuitato 
            bool_accumulator.push_back(false); return;
        }
        bool_accumulator.push_back(lval && rval); return;
    case BoolOp::OR:
        if (lval) {
            //l'OR è cortocircuitato
            bool_accumulator.push_back(true); return;
        }
       bool_accumulator.push_back(lval || rval); return;
    case BoolOp::NOT:
       bool_accumulator.push_back(!lval); return;
    default:
        return;
    }
}

void EvaluationVisitor::visitBoolConst(BoolConst* boolNode) {
    // Il metodo push_back inserisce il valore come ultimo elemento del vector
    bool_accumulator.push_back(boolNode->get_Value());
}

void EvaluationVisitor::visitSetStmt(SetStmt* setNode) {
    //prelevo il valore
    NumExpr* value = setNode->get_ValueS();
    value->accept(this);
    int64_t num = accumulator.back(); accumulator.pop_back();
    //modifico il valore della variabile o la creo se non esiste
    Variable* variable_id = setNode->get_Variable();
    auto it = Variable::list_Variables.find(variable_id->get_Id());
    std::string id = variable_id->get_Id();
    if (it != Variable::list_Variables.end()) {
        //La variabile esiste già: aggiorno il valore della variabile
        variable_id->list_Variables[id] = num;
    }
    else {
       //La variabile non esiste e viene creata
        Variable::list_Variables.insert({id, num});
    }  
}

void EvaluationVisitor::visitPrintStmt(PrintStmt* printNode) {
    NumExpr* output = printNode->get_Output();
    if (dynamic_cast<Variable*>(output)) {
        Variable* var = dynamic_cast<Variable*>(output);
        auto it = Variable::list_Variables.find(var->get_Id());
        if (it != Variable::list_Variables.end()) {
            //La variabile esiste già e ne stampo il valore
            std::string id = var->get_Id();
            std::cout << var->list_Variables[id] << std::endl;
        }
        else {
            //La variabile non esiste
            throw ParseError("using an invalid variable");
        }
    }
    else {
        output->accept(this);
        int64_t num = accumulator.back(); accumulator.pop_back();
        std::cout << num << std::endl;
    }
}
   

void EvaluationVisitor::visitInputStmt(InputStmt* inputNode) {
    Variable* variable_id = inputNode->get_Variable();
    std::string input_str;
    std::cout << "Enter the vaue of variable " << variable_id->get_Id() << ": ";
    //Controllo del valore inserito da console
    std::getline(std::cin, input_str);
    for (char c : input_str) {
        if (!isdigit(c)) {
            throw std::invalid_argument("invalid value in input");
        }
    }
    auto it = Variable::list_Variables.find(variable_id->get_Id());
    std::string id = variable_id->get_Id();
    if (it != Variable::list_Variables.end()) {
        //La variabile esiste già: aggiorno il valore della variabile
        variable_id->list_Variables[id] = std::stoll(input_str);
    }
    else {
        //La variabile non esiste e viene creata
        Variable::list_Variables.insert({ id, std::stoll(input_str) });
    }
}

void EvaluationVisitor::visitWhileStmt(WhileStmt* whileNode) {
    BoolExpr* condition = whileNode->get_Condition();
    condition->accept(this);
    bool result = bool_accumulator.back(); bool_accumulator.pop_back();
    while (result) {
        Block* list = whileNode->get_List();
        list->accept(this);
        condition->accept(this);
        result = bool_accumulator.back(); bool_accumulator.pop_back();
       
    } 
}

void EvaluationVisitor::visitIfStmt(IfStmt* ifNode) {
    BoolExpr* condition = ifNode->get_Condition();
    condition->accept(this);
    bool result = bool_accumulator.back(); bool_accumulator.pop_back();
    if (result) {
        Block* truelist = ifNode->get_Truelist();
        truelist->accept(this);
    }
    else {
        Block* falselist = ifNode->get_Falselist();
        falselist->accept(this);
    }
}

void EvaluationVisitor::visitBlockStmt(Block* blockNode) {
    const std::vector<Statement*>& list = blockNode->get_StatementList();
    for (Statement* statement : list) {
        statement->accept(this);
    }
}

void EvaluationVisitor::visitProgram(Program* programNode) {
    Block* statements = programNode->get_Statements();
    statements->accept(this);
}


//PRINTVISITOR

void PrintVisitor::visitOperator(Operator* opNode) {
    std::cout << "(";
    std::cout << Operator::opCodeTostring(opNode->get_Op());
    std::cout << " ";
    opNode->get_Left()->accept(this);
    std::cout << " ";
    opNode->get_Right()->accept(this);
    std::cout << ")";
}
void PrintVisitor::visitNumber(Number* numNode) {
    std::cout << numNode->get_ValueN();
}
void PrintVisitor::visitRelOp(RelOp* opNode) {
    std::cout << "(";
    std::cout << RelOp::opCodeTostring(opNode->get_Op());
    std::cout << " ";
    opNode->get_Left()->accept(this);
    std::cout << " ";
    opNode->get_Right()->accept(this);
    std::cout << ")";
}
void PrintVisitor::visitBoolOp(BoolOp* opNode) {
    if (opNode->get_Op() == BoolOp::NOT) {
        std::cout << "(NOT ";
        opNode->get_Left()->accept(this);
        std::cout << ")";
    }
    else {
        std::cout << "(";
        std::cout << BoolOp::opCodeTostring(opNode->get_Op());
        std::cout << " ";
        opNode->get_Left()->accept(this);
        std::cout << " ";
        opNode->get_Right()->accept(this);
        std::cout << ")";
    }
}
void PrintVisitor::visitBoolConst(BoolConst* boolNode)  {
    if (boolNode->get_Value() == true) {
        std::cout << "TRUE";
    }
    else if (boolNode->get_Value() == false) {
        std::cout << "FALSE";
    }
}
void PrintVisitor::visitVariable(Variable* varNode)  {
    std::cout << varNode->get_Id();
}

void PrintVisitor::visitSetStmt(SetStmt* setNode)  {
    std::cout << "(SET ";
    setNode->get_Variable()->accept(this);
    std::cout << " ";
    setNode->get_ValueS()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitPrintStmt(PrintStmt* printNode)  {
    std::cout << "(PRINT ";
    printNode->get_Output()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitInputStmt(InputStmt* inputNode)  {
    std::cout << "(INPUT ";
    inputNode->get_Variable()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitWhileStmt(WhileStmt* whileNode)  {
    std::cout << "(WHILE ";
    whileNode->get_Condition()->accept(this);
    std::cout << " ";
    whileNode->get_List()->accept(this);
    std::cout << ")" << std::endl;
}
void PrintVisitor::visitIfStmt(IfStmt* ifNode)  {
    std::cout << "(IF ";
    ifNode->get_Condition()->accept(this);
    std::cout << " ";
    ifNode->get_Truelist()->accept(this);
    std::cout << " ";
    ifNode->get_Falselist()->accept(this);
    std::cout << ")" << std::endl;
}
void PrintVisitor::visitBlockStmt(Block* blockNode)  {
    const std::vector<Statement*>& statements = blockNode->get_StatementList();
    if (blockNode->get_Keyword() == false) {
        statements[0]->accept(this);
    }
    else if (blockNode->get_Keyword() == true) {
        std::cout << "(BLOCK " << std::endl;
        for (Statement* statement : statements) {
            statement->accept(this);
        }
        std::cout << ")" << std::endl;
    }

}
void PrintVisitor::visitProgram(Program* blockNode) {
    blockNode->get_Statements()->accept(this);
}
