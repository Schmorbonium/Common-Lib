#include "riscV.hpp"
#include "ibc.h"

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
 * Isaac
 *****************************************/
MemoryOp::MemoryOp(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool MemoryOp::evaluate() {
    return false;
}

/*****************************************
 * RegisterWrite
 * Daniel
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
        case 0: //output
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
 * Daniel
 *****************************************/
NewPC::NewPC(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool NewPC::evaluate() {
    return false;
}

/*****************************************
 * ALUOut
 * Daniel
 *****************************************/
ALUOut::ALUOut(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool ALUOut::evaluate() {
    uint32_t leftOperand;
    uint32_t rightOperand;

    if (processor->decodedInstruction.aluSrcAIsPC) {
        leftOperand = processor->pc.programCounter;
    } else {
        leftOperand = processor->registerOut.regA_value;
    }

    if (processor->decodedInstruction.aluSrcBIsImm) {
        rightOperand = processor->decodedInstruction.immediate_val;
    } else {
        rightOperand = processor->registerOut.regB_value;
    }
    switch (processor->decodedInstruction.aluOp) {
    case IBC_ALUOP_ADD:
        output = leftOperand + rightOperand;
        break;
    case IBC_ALUOP_SLL:
        output = leftOperand << rightOperand;
        break;
    case IBC_ALUOP_SLT:
        output = (int32_t)leftOperand < (int32_t)rightOperand ? 1 : 0;
        break;
    case IBC_ALUOP_SLTU:
        output = leftOperand < rightOperand ? 1 : 0;
        break;
    case IBC_ALUOP_XOR:
        output = leftOperand ^ rightOperand;
        break;
    case IBC_ALUOP_SRL:
        output = leftOperand >> rightOperand;
        break;
    case IBC_ALUOP_OR:
        output = leftOperand | rightOperand;
        break;
    case IBC_ALUOP_AND:
        output = leftOperand & rightOperand;
        break;
    case IBC_ALUOP_SUB:
        output = leftOperand - rightOperand;
        break;
    case IBC_ALUOP_SRA:
        output = (int32_t)leftOperand >> (int32_t)rightOperand;
        break;
    default:
        //TODO: best behavior for unknown ALUop code
        break;
    }

    return true;
}

/*****************************************
 * RegisterOutputs
 * Daniel
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
 * Alex
 *****************************************/
DecodedInstruction::DecodedInstruction(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool DecodedInstruction::evaluate() {
    return false;
}

/*****************************************
 * Instruction
 * Isaac
 *****************************************/
Instruction::Instruction(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool Instruction::evaluate() {
    return false;
}

/*****************************************
 * PC
 * Alex
 *****************************************/
PC::PC(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}

bool PC::evaluate() {
    return false;
}