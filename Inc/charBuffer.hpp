#ifndef __CHAR_BUFFER__
#define __CHAR_BUFFER__



#include "zHal.h"

class CharBuffer {
private:
    static const uint16_t NODE_SIZE = 16;
    struct Node {
        uint8_t data[NODE_SIZE];
        Node* next;
    };
    uint16_t size;
    Node* head;
    uint16_t headIndex;
    Node* tail;
    uint16_t tailIndex;

public:
    CharBuffer();
    ~CharBuffer();

    // Append a new Character to the end of the buffer 
    void append(uint8_t c);

    // Append a new Character to the end of the buffer 
    void append_uint16(uint16_t c);

    // Append a new Character to the end of the buffer 
    void append_uint32(uint32_t c);

    // Remove and return the first character in the buffer
    uint8_t pop();

    // Remove and return the first 2 characters in the buffer
    uint16_t pop_uint16();

    // Remove and return the first 4 characters in the buffer
    uint32_t pop_uint32();

    // Peaks at the value at a given index
    uint8_t peak(uint16_t) const;

    uint16_t peak_uint16() const;
    uint32_t peak_uint32() const;

    // Get the current size of the buffer
    uint16_t getSize() const;

    // Check if the buffer is empty
    bool isEmpty() const;

    // Clear the buffer and release memory
    void clear();

    // Print the contents of the buffer
    void print() const;
};


#endif // End __CHAR_BUFFER__