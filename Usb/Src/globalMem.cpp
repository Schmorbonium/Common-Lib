#include "globalMem.hpp"
#include <fstream>
#include <iostream>

Memory::Memory() {}

void Memory::loadBinFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    uint32_t address = 0;
    uint32_t instruction;
    while (file.read(reinterpret_cast<char*>(&instruction), sizeof(instruction))) {
        memory[address] = instruction;
        address += 4;  // Move to the next word
    }
}

uint32_t Memory::readMemory(uint32_t address) const {
    uint32_t wordAddress = address & ~0x3;  // Align the address to the start of the word
    auto it = memory.find(wordAddress);
    if (it != memory.end()) {
        return it->second;
    }
    return 0; // Return 0 if the word is not in memory
}

void Memory::writeMemory(uint32_t address, uint32_t value) {
    uint32_t wordAddress = address & ~0x3;  // Align the address to the start of the word
    memory[wordAddress] = value;
}
