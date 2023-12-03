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
 * Alex
 *****************************************/
NewPC::NewPC(DependencyTree* parent, riscV* processor) : IDependencyNode(parent) {
    this->processor = processor;
}


bool NewPC::evaluate()
{
    pcPlus4 = processor->pc.programCounter + 4;
    programCounter = pcPlus4;
    if (processor->decodedInstruction.opcode == BRANCH)
    {
        switch (processor->decodedInstruction.funct3)
        {
        case 0x0: // BEQ
            if (processor->alu.out == 0) // branch true
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        case 0x1: // BNE
            if (processor->alu.out != 0)
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        case 0x4: // BLT
            if ((int32_t)processor->alu.out < 0)
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        case 0x5: // BGE
            if ((int32_t)processor->alu.out > 0)
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        case 0x6: // BLTU
            if ((int32_t)processor->alu.out < 0)
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        case 0x7:
            if ((int32_t)processor->alu.out > 0)
                programCounter = processor->pc.programCounter + processor->decodedInstruction.immediate_val;
            break;
        default:
            break;
        }
    }
    else if (processor->decodedInstruction.jump)
        programCounter = (processor->pc.programCounter + processor->decodedInstruction.immediate_val) & 0xFFFFFFFE;
    else
        ;
    return true;

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

bool DecodedInstruction::evaluate()
{
    uint32_t instruction = processor->instruction.instruction;
    // Read the opcode (Bits 7:0).
    opcode = instruction & 0x7F;
    regDest_select = (instruction >> 7) & 0xF;
    regA_select = (instruction >> 15) & 0xF;
    regB_select = (instruction >> 20) & 0xF;
    funct3 = (instruction >> 12) & 0x7;
    uint8_t funct7 = (instruction >> 25) & 0x7F;
    regDestMux = 0;
    regWriteEnable = false;
    memReadEnable = false;
    memWriteEnable = false;
    aluOp = 0;
    aluSrcAIsPC = false;
    aluSrcBIsImm = false;
    jump = false;

    immediate_val = 0;
    // Light the LEDs based on the current Opcode
    switch (opcode)
    {
    case LOAD:
        immediate_val = (instruction >> 20) & 0xFFF;
        regWriteEnable = true;
        aluOp = IBC_ALUOP_ADD;
        memReadEnable = true;
        break;
    case OPIMM:
        regWriteEnable = true;
        // determine what ALUOP is on the instruction
        aluSrcBIsImm = true;
        switch (funct3)
        {
        case 0x0: // ADD or SUB
            if (funct7 > 0)
            {
                aluOp = IBC_ALUOP_SUB;
            }
            else
            {
                aluOp = IBC_ALUOP_ADD;
            }
            break;
        case 0x1: // SLL
            aluOp = IBC_ALUOP_SLL;
            break;
        case 0x2: // SLT
            aluOp = IBC_ALUOP_SLT;
            break;
        case 0x3: // SLTU
            aluOp = IBC_ALUOP_SLTU;
            break;
        case 0x4: // XOR
            aluOp = IBC_ALUOP_XOR;
            break;
        case 0x5: // SRL or SRA
            if (funct7 > 0)
                aluOp = IBC_ALUOP_SRA;
            else
                aluOp = IBC_ALUOP_SRL;
            break;
        case 0x6: // OR
            aluOp = IBC_ALUOP_OR;
            break;
        case 0x7: // AND
            aluOp = IBC_ALUOP_AND;
            break;
        default:
            break;
        }
        // No mem op on OPIMM
        break;
    case AUIPC:
        immediate_val = instruction & 0xFFFFF000;
        regWriteEnable = true;
        aluSrcAIsPC = true;
        aluSrcBIsImm = true;
        break;
    case STORE:
        immediate_val = (instruction >> 20 & 0xFE0) + (instruction >> 7 & 0x1F);
        memWriteEnable = true;
        aluSrcBIsImm = true;
        break;
    case OP:
        regWriteEnable = true;
        switch (funct3)
        {
        case 0x0: // ADD or SUB
            if (funct7 > 0)
                aluOp = IBC_ALUOP_SUB;
            else
                aluOp = IBC_ALUOP_ADD;
            break;
        case 0x1: // SLL
            aluOp = IBC_ALUOP_SLL;
            break;
        case 0x2: // SLT
            aluOp = IBC_ALUOP_SLT;
            break;
        case 0x3: // SLTU
            aluOp = IBC_ALUOP_SLTU;
            break;
        case 0x4: // XOR
            aluOp = IBC_ALUOP_XOR;
            break;
        case 0x5: // SRL or SRA
            if (funct7 > 0)
                aluOp = IBC_ALUOP_SRA;
            else
                aluOp = IBC_ALUOP_SRL;
            break;
        case 0x6: // OR
            aluOp = IBC_ALUOP_OR;
            break;
        case 0x7: // AND
            aluOp = IBC_ALUOP_AND;
            break;
        default:
            break;
        }
        break;
    case LUI:
        immediate_val = instruction & 0xFFFFF000;
        regWriteEnable = true;
        aluSrcBIsImm = true;
        break;
    case BRANCH:
        aluOp = IBC_ALUOP_SUB;
        immediate_val = 0x00000000 | (((instruction & 0x80000000) >> 19) | ((instruction & 0x80) << 4) | ((instruction & 0x7E000000) >> 20) | ((instruction & 0xF00) >> 7));
        break;
    case JALR:
        immediate_val = 0x00000000 | ((instruction & 0xFFF00000) >> 20);
        aluOp = IBC_ALUOP_ADD;
        aluSrcBIsImm = true;
        jump = true;
        break;
    case JAL:
        jump = true;
        immediate_val = 0x00000000 | (((instruction & 0x800000000) >> 11) | (instruction & 0xFF000) | ((instruction & 0x100000) >> 9) | ((instruction & 0x7FE00000) >> 20));
        break;
    case SYSTEM:
        break;
    default:
        break;
    }
    return true;
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

bool PC::evaluate()
{
    this->programCounter = processor->newPC.programCounter;
    return true;

}