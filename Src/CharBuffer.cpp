#include "charBuffer.hpp"
#include "InterruptController.h"

CharBuffer::CharBuffer() : size(0), head(nullptr), headIndex(0), tail(nullptr), tailIndex(0) {}

CharBuffer::~CharBuffer()
{
    clear();
}

// Add a character to the end of the buffer
void CharBuffer::append(uint8_t c)
{
    InterruptController_enter();
    checkSumAppended += c;
    tailIndex++;
    if (tail == nullptr || tailIndex == NODE_SIZE)
    {
        Node *newNode = new Node();
        if (tail == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        tailIndex = 0;
    }
    tail->data[tailIndex] = c;
    size++;
    InterruptController_leave();
}

// Remove and return the first character in the buffer
uint8_t CharBuffer::pop()
{
    InterruptController_enter();
    if (size == 0)
    {
        return 0;
        // throw std::out_of_range("Buffer is empty");
    }
    

    char value = head->data[headIndex];
    headIndex++;
    size--;
    if (headIndex == NODE_SIZE)
    {
        Node *temp = head;
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;
        }
        delete temp;
        headIndex = 0;
    }
    checkSumPopped += value;
    InterruptController_leave();
    return value;
}

// Peaks at a value at given index. Warning! This i O(index)
uint8_t CharBuffer::peak(uint16_t index) const
{
    if (index + 1 > size)
    {
        return 0;
    }
    uint16_t relIndex = index + headIndex;
    Node *curNode = head;
    while (relIndex >= NODE_SIZE)
    {
        curNode = curNode->next;
        relIndex -= NODE_SIZE;
    }
    return curNode->data[relIndex];
}
// Get the current size of the buffer
uint16_t CharBuffer::getSize() const
{
    return size;
}

// Check if the buffer is empty
bool CharBuffer::isEmpty() const
{
    return size == 0;
}

// Clear the buffer and release memory
void CharBuffer::clear()
{
    InterruptController_enter();
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
    tailIndex = 0;
    headIndex = 0;
    InterruptController_leave();
}

// Print the contents of the buffer
void CharBuffer::print() const
{
    Node *current = head;
    size_t currentIndex = 0;

    while (current != nullptr)
    {
        while (currentIndex < tailIndex)
        {
        }
        currentIndex = 0;
        current = current->next;
    }
}

void CharBuffer::append_uint16(uint16_t c)
{
    uint8_t lsb = c & 0xFF; 
    uint8_t msb = (c >> 8) & 0xFF; 
    this->append(lsb);
    this->append(msb);
}

uint16_t CharBuffer::pop_uint16()
{
    uint16_t lsb = this->pop();
    uint16_t msb = this->pop();
    return (lsb | (msb<<8));
}

uint16_t CharBuffer::peak_uint16(uint16_t startingIndex) const
{
    uint16_t lsb = this->peak(startingIndex);
    uint16_t msb = this->peak(startingIndex+1);
    return (lsb | (msb<<8));
}

uint16_t CharBuffer::peak_uint16() const
{
    return peak_uint16(0);
}

void CharBuffer::append_uint32(uint32_t c)
{
    uint16_t lsW = c & 0xFFFF; 
    uint16_t msW = (c >> 16) & 0xFFFF; 
    this->append_uint16(lsW);
    this->append_uint16(msW);
}

uint32_t CharBuffer::pop_uint32()
{
    uint32_t lsW = pop_uint16(); 
    uint32_t msW = pop_uint16(); 
    return (lsW | msW << 16);
}

uint32_t CharBuffer::peak_uint32() const
{
    uint16_t lsW = this->peak_uint16(0);
    uint16_t msW = this->peak_uint16(2);
    return (lsW | (msW<<16));
}

uint8_t CharBuffer::getPoppedCheckSum()
{
    return this->checkSumPopped;
}

uint8_t CharBuffer::setPoppedCheckSum(uint8_t newVal)
{
    uint8_t value = this->checkSumPopped;
    this->checkSumPopped = newVal;
    return value;
}

uint8_t CharBuffer::getQueuedCheckSum()
{
    return this->checkSumAppended;
}

uint8_t CharBuffer::setQueuedCheckSum(uint8_t newVal)
{
    uint8_t value = this->checkSumAppended;
    this->checkSumAppended = newVal;
    return value;
}