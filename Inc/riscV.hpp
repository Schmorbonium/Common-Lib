#include "DependencyTree.hpp"

#define LOAD    0x03
#define LOADFP  0x07 // unused in RV32E Instruction Set
#define MISCMEM 0x0f // FENCE instruction - not implementing. I refuse.
#define OPIMM   0x13
#define AUIPC   0x17
#define OPIMM32 0x1f
#define STORE   0x23
#define STOREFP 0x27
#define AMO     0x2f // Unused in RV32E
#define OP      0x33
#define LUI     0x37
#define OP32    0x3b
#define MADD    0x43
#define MSUB    0x47
#define NMSUB   0x4b
#define NMADD   0x4f
#define OPFP    0x53
#define BRANCH  0x63
#define JALR    0x67
#define JAL     0x6F
#define SYSTEM  0x73

class riscV;
class PC : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t programCounter;
    bool evaluate();
    PC(DependencyTree *parent, riscV *processor);
    ~PC();
};

class Instruction : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t instruction;
    bool evaluate();
    Instruction(DependencyTree *parent, riscV *processor);
    ~Instruction();
};

class DecodedInstruction : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t immediate_val;
    uint8_t regA_select;
    uint8_t regB_select;
    uint8_t regDestMux;
    uint8_t regDest_select;
    bool regWriteEnable;
    bool jump;

    bool memReadEnable;
    bool memWriteEnable;
    
    uint8_t aluOp;
    bool aluSrcAIsPC;
    bool aluSrcBIsImm;

    uint8_t opcode;
    uint8_t funct3;
    
    bool evaluate();
    DecodedInstruction(DependencyTree *parent, riscV *processor);
    ~DecodedInstruction();
};

class RegisterOutputs : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t regA_value;
    uint32_t regB_value;
    bool evaluate();
    RegisterOutputs(DependencyTree *parent, riscV *processor);
    ~RegisterOutputs();
};

class ALUOut : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t programCounter;
    bool evaluate();
    ALUOut(DependencyTree *parent, riscV *processor);
    ~ALUOut();
};

class NewPC : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t programCounter;
    uint32_t pcPlus4;
    bool evaluate();
    NewPC(DependencyTree *parent, riscV *processor);
    ~NewPC();
};

class RegisterWrite : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t programCounter;
    bool evaluate();
    RegisterWrite(DependencyTree *parent, riscV *processor);
    ~RegisterWrite();
};

class MemoryOp : public IDependencyNode
{
private:
    riscV *processor;
public:
    uint32_t programCounter;
    bool evaluate();
    MemoryOp(DependencyTree *parent, riscV *processor);
    ~MemoryOp();
};

class riscV
{
private:
public:
    DependencyTree process;
    PC pc;
    Instruction instruction;
    DecodedInstruction decodedInstruction;
    RegisterOutputs registerOut;
    ALUOut alu;
    NewPC newPC;
    MemoryOp memOp;
    RegisterWrite regWrite;

    riscV();
    ~riscV();
};
