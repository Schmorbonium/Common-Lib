#include "DependencyTree.hpp"

class riscV;
class PC : public IDependencyNode
{
private:
    uint32_t programCounter;
    riscV *processor;
public:
    bool evaluate();
    PC(DependencyTree *parent, riscV *processor);
    ~PC();
};

class Instruction : public IDependencyNode
{
private:
    uint32_t instruction;
    riscV *processor;
public:
    bool evaluate();
    Instruction(DependencyTree *parent, riscV *processor);
    ~Instruction();
};

class DecodedInstruction : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t immediate_val;
    uint8_t regA_select;
    uint8_t regB_select;
    uint8_t regDestMux;
    uint8_t regDest_select;
    uint8_t regWriteEnable;

    bool memReadEnable;
    bool memWriteEnable;
    
    uint8_t regDest;
    uint8_t aluOp;
    bool aluSrcAIsPC;
    bool aluSrcBIsImm;
public:
    bool evaluate();
    DecodedInstruction(DependencyTree *parent, riscV *processor);
    ~DecodedInstruction();
};

class RegisterOutputs : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t regA_value;
    uint32_t regB_value;
public:
    bool evaluate();
    RegisterOutputs(DependencyTree *parent, riscV *processor);
    ~RegisterOutputs();
};

class ALUOut : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t programCounter;
public:
    bool evaluate();
    ALUOut(DependencyTree *parent, riscV *processor);
    ~ALUOut();
};

class NewPC : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t programCounter;
public:
    bool evaluate();
    NewPC(DependencyTree *parent, riscV *processor);
    ~NewPC();
};

class RegisterWrite : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t programCounter;
public:
    bool evaluate();
    RegisterWrite(DependencyTree *parent, riscV *processor);
    ~RegisterWrite();
};

class MemoryOp : public IDependencyNode
{
private:
    riscV *processor;
    uint32_t programCounter;
public:
    bool evaluate();
    MemoryOp(DependencyTree *parent, riscV *processor);
    ~MemoryOp();
};

class riscV
{
private:
    DependencyTree process;
    PC pc;
    Instruction instruction;
    DecodedInstruction decodedInstruction;
    RegisterOutputs registerOut;
    ALUOut alu;
    NewPC newPC;
    MemoryOp memOp;
    RegisterWrite regWrite;

public:
    riscV();
    ~riscV();
};
