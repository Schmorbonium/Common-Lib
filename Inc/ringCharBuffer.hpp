#ifndef __RING_CHAR_BUFFER__
#define __RING_CHAR_BUFFER__

#include "zHal.h"
#include "iBasicQue.hpp"

class RingCharBuffer : public I_Basic_Que
{
private:
    static const uint16_t NODE_SIZE = 64;
    struct Node
    {
        Node *next;
        Node *last;
        uint8_t active;
        uint8_t data[NODE_SIZE];
    };

    uint16_t size;
    uint16_t nodeCount;
    uint16_t activeNodeCount;

    Node *head;
    uint16_t headIndex;
    Node *tail;
    uint16_t tailIndex;

    uint8_t checkSumAppended = 0;
    uint8_t checkSumPopped = 0;

    void initNodes();
    void insertNode();
    void removeNode();
    void advanceTail();
    void advanceHead();

public:
    RingCharBuffer();
    ~RingCharBuffer();

    // Delete copy constructor and copy assignment operator
    RingCharBuffer(const RingCharBuffer &) = delete;
    RingCharBuffer &operator=(const RingCharBuffer &) = delete;

    // Delete move constructor and move assignment operator
    RingCharBuffer(RingCharBuffer &&) = delete;
    RingCharBuffer &operator=(RingCharBuffer &&) = delete;

    // Append a new Character to the end of the buffer
    void append(uint8_t c);
    // Remove and return the first character in the buffer
    uint8_t pop();
    // Peaks at the value at a given index
    uint8_t peak(uint16_t index) const;
    // Get the current size of the buffer
    uint16_t getSize() const;
    // Check if the buffer is empty
    bool isEmpty() const;
    // Clear the buffer and release memory
    void clear();
    uint8_t getPoppedCheckSum();
    uint8_t setPoppedCheckSum(uint8_t value);
    uint8_t getQueuedCheckSum();
    uint8_t setQueuedCheckSum(uint8_t value);
};

#endif // End __CHAR_BUFFER__