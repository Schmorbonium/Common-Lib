#ifndef __CHAR_BUFFER__
#define __CHAR_BUFFER__

#include "zHal.h"
#include "iQueue.hpp"


class CharBuffer : public IQueue
{
private:
    static const uint16_t NODE_SIZE = 64;
    struct Node
    {
        uint8_t data[NODE_SIZE];
        Node *next;
    };
    uint16_t size;
    Node *head;
    uint16_t headIndex;
    Node *tail;
    uint16_t tailIndex;

    uint8_t checkSumAppended = 0;
    uint8_t checkSumPopped = 0;

public:
    CharBuffer();
    ~CharBuffer();

    // Delete copy constructor and copy assignment operator
    CharBuffer(const CharBuffer &) = delete;
    CharBuffer &operator=(const CharBuffer &) = delete;

    // Delete move constructor and move assignment operator
    CharBuffer(CharBuffer &&) = delete;
    CharBuffer &operator=(CharBuffer &&) = delete;

    // Append a new Character to the end of the buffer
    void append(uint8_t c);

    // Remove and return the first character in the buffer
    uint8_t pop();

    // Peaks at the value at a given index
    uint8_t peak(uint16_t) const;

    // Get the current size of the buffer
    uint16_t getSize() const;

    // Check if the buffer is empty
    bool isEmpty() const;

    // Clear the buffer and release memory
    void clear();

    // Print the contents of the buffer
    void print() const;

    uint8_t getPoppedCheckSum();
    uint8_t setPoppedCheckSum(uint8_t value);

    uint8_t getQueuedCheckSum();
    uint8_t setQueuedCheckSum(uint8_t value);
};

#endif // End __CHAR_BUFFER__