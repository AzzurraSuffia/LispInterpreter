#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <iostream>

class Operator; 
class Number; 
class Variable;
class RelOp; 
class BoolOp; 
class BoolConst;
class SetStmt;
class PrintStmt;
class InputStmt;
class IfStmt;
class WhileStmt;
class Block;
class Program;

// Visitor astratto per la visita degli oggetti della gerarchia
class Visitor {
public:
    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
    virtual void visitVariable(Variable* varNode) = 0;
    virtual void visitRelOp(RelOp* opNode) = 0;
    virtual void visitBoolOp(BoolOp* opNode) = 0;
    virtual void visitBoolConst(BoolConst* boolNode) = 0;
    virtual void visitSetStmt(SetStmt* setNode) = 0;
    virtual void visitPrintStmt(PrintStmt* printNode) = 0;
    virtual void visitInputStmt(InputStmt* inputNode) = 0;
    virtual void visitIfStmt(IfStmt* ifNode) = 0;
    virtual void visitWhileStmt(WhileStmt* whileNode) = 0;
    virtual void visitBlockStmt(Block* blockNode) = 0;
    virtual void visitProgram(Program* programNode) = 0;
  
};


// Visitor concreto per la valutazione del programma
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor() : accumulator{ }, bool_accumulator { } {}
    ~EvaluationVisitor() = default;
    EvaluationVisitor(const EvaluationVisitor& other) = default;
    EvaluationVisitor& operator=(const EvaluationVisitor& other) = delete;

    void visitOperator(Operator* opNode)      override;
    void visitNumber(Number* numNode)         override;
    void visitVariable(Variable* varNode)     override;
    void visitRelOp(RelOp* opNode)            override;
    void visitBoolOp(BoolOp* opNode)          override;
    void visitBoolConst(BoolConst* boolNode)  override;
    void visitSetStmt(SetStmt* setNode)        override;
    void visitPrintStmt(PrintStmt* printNode) override;
    void visitInputStmt(InputStmt* inputNode) override;
    void visitIfStmt(IfStmt* ifNode)          override;
    void visitWhileStmt(WhileStmt* whileNode) override;
    void visitBlockStmt(Block* blockNode)     override;
    void visitProgram(Program* programNode)   override;

private:
    std::vector<int64_t> accumulator;
    std::vector<bool> bool_accumulator;
};


// Visitor concreto per la stampa delle espressioni
//(non presente nel main, ma implementato per un possibile uso)
class PrintVisitor : public Visitor {
public:
    PrintVisitor() {}
    ~PrintVisitor() = default;
    PrintVisitor(const PrintVisitor& other) = default;
    PrintVisitor& operator=(const PrintVisitor& other) = delete;

    void visitOperator(Operator*)   override;
    void visitVariable(Variable*)   override;
    void visitNumber(Number*)       override;
    void visitBoolOp(BoolOp*)       override;
    void visitBoolConst(BoolConst*) override;
    void visitRelOp(RelOp*)         override;
    void visitBlockStmt(Block*)     override;
    void visitPrintStmt(PrintStmt*) override;
    void visitSetStmt(SetStmt*)     override;
    void visitInputStmt(InputStmt*) override;
    void visitWhileStmt(WhileStmt*) override;
    void visitIfStmt(IfStmt*)       override;
    void visitProgram(Program*)     override;
};

#endif
