#include "riscV.hpp"

riscV::riscV() : process(),
                 pc(&process, this),
                 instruction(&process, this),
                 decodedInstruction(&process, this),
                 registerOut(&process, this),
                 alu(&process, this),
                 newPC(&process, this),
                 memOp(&process, this),
                 regWrite(&process, this)
{
    instruction.addDependency(&pc);
    decodedInstruction.addDependency(&instruction);
    registerOut.addDependency(&decodedInstruction);
    alu.addDependency(&decodedInstruction);
    alu.addDependency(&registerOut);
    newPC.addDependency(&decodedInstruction);
    newPC.addDependency(&alu);
    memOp.addDependency(&decodedInstruction);
    memOp.addDependency(&registerOut);
    memOp.addDependency(&alu);
    regWrite.addDependency(&decodedInstruction);
    regWrite.addDependency(&alu);
    regWrite.addDependency(&newPC);
    regWrite.addDependency(&memOp);
}

riscV::~riscV()
{
}

/*****************************************
 * MemoryOp Node
 *****************************************/
MemoryOp::MemoryOp(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool MemoryOp::evaluate()
{
    return false;
}

/*****************************************
 * RegisterWrite
 *****************************************/
RegisterWrite::RegisterWrite(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool RegisterWrite::evaluate()
{
    return false;
}

/*****************************************
 * NewPC
 *****************************************/
NewPC::NewPC(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool NewPC::evaluate()
{
    return false;
}

/*****************************************
 * ALUOut
 *****************************************/
ALUOut::ALUOut(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool ALUOut::evaluate()
{
    return false;
}

/*****************************************
 * RegisterOutputs
 *****************************************/
RegisterOutputs::RegisterOutputs(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool RegisterOutputs::evaluate()
{
    return false;
}

/*****************************************
 * DecodedInstruction
 *****************************************/
DecodedInstruction::DecodedInstruction(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool DecodedInstruction::evaluate()
{
    return false;
}

/*****************************************
 * Instruction
 *****************************************/
Instruction::Instruction(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool Instruction::evaluate()
{
    return false;
}

/*****************************************
 * PC
 *****************************************/
PC::PC(DependencyTree *parent, riscV *processor) : IDependencyNode(parent)
{
    this->processor = processor;
}

bool PC::evaluate()
{
    this->programCounter = processor->newPC.programCounter;
    return true;
}