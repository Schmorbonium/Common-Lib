#include "riscV.hpp"

riscV::riscV() : process(),
pc(&process, this),
instruction(&process, this),
decodedInstruction(&process, this),
registerOut(&process, this),
alu(&process, this),
newPC(&process, this),
memOp(&process, this),
regWrite(&process, this) {
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

riscV::~riscV() {
}

/*****************************************
 * MemoryOp Node
 *****************************************/
MemoryOp::MemoryOp(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool MemoryOp::evaluate() {
    return false;
}

/*****************************************
 * RegisterWrite
 *****************************************/
RegisterWrite::RegisterWrite(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
    for (int i = 0; i < 16; i++) {
        regs[i] = 0;
    }
}

bool RegisterWrite::evaluate() {
    if (processor->decodedInstruction.regWriteEnable) {
        switch (processor->decodedInstruction.regDestMux) {
        case 0: //aluQ
            regs[processor->decodedInstruction.regDest_select] = processor->alu.output;
            break;
        case 1: // pc+4
            regs[processor->decodedInstruction.regDest_select] = processor->pc.programCounter;
            break;
        case 2: // memData
            regs[processor->decodedInstruction.regDest_select] = processor->memOp.dataOut;
            break;
        case 3: // imm
            regs[processor->decodedInstruction.regDest_select] = processor->decodedInstruction.immediate_val;
            break;
        }
        return true;
    }
    return false;
}

/*****************************************
 * NewPC
 *****************************************/
NewPC::NewPC(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool NewPC::evaluate() {
    return false;
}

/*****************************************
 * ALUOut
 *****************************************/
ALUOut::ALUOut(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool ALUOut::evaluate() {
    return false;
}

/*****************************************
 * RegisterOutputs
 *****************************************/
RegisterOutputs::RegisterOutputs(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool RegisterOutputs::evaluate() {
    regA_value = processor->regWrite.regs[processor->decodedInstruction.regA_select];
    regB_value = processor->regWrite.regs[processor->decodedInstruction.regB_select];
    return true;
}

/*****************************************
 * DecodedInstruction
 *****************************************/
DecodedInstruction::DecodedInstruction(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool DecodedInstruction::evaluate() {
    return false;
}

/*****************************************
 * Instruction
 *****************************************/
Instruction::Instruction(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool Instruction::evaluate() {
    return false;
}

/*****************************************
 * PC
 *****************************************/
PC::PC(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool PC::evaluate() {
    return false;
}