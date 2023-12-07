#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <unordered_map>
#include <cstdint>

class Memory {
public:
    Memory();
    void loadBinFile(const std::string& filePath);
    uint32_t readMemory(uint32_t address) const;
    void writeMemory(uint32_t address, uint32_t value);

private:
    std::unordered_map<uint32_t, uint32_t> memory;
};

#endif // MEMORY_H