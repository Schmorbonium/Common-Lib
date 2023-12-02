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
    this->append(c & 0xFF);
    this->append((c >> 8) & 0xFF);
}

void CharBuffer::append_uint32(uint32_t c)
{
    this->append(c & 0xFF);
    this->append((c >> 8) & 0xFF);
    this->append((c >> 16) & 0xFF);
    this->append((c >> 24) & 0xFF);
}

uint16_t CharBuffer::pop_uint16()
{
    uint16_t d = this->pop();
    d |= this->pop() << 8;
    return d;
}

uint32_t CharBuffer::pop_uint32()
{
    uint32_t d = this->pop();
    d |= this->pop() << 8;
    d |= this->pop() << 16;
    d |= this->pop() << 24;
    return d;
}

uint16_t CharBuffer::peak_uint16() const
{
    uint16_t d = this->peak(0);
    d |= this->peak(1) << 8;
    return d;
}

uint16_t CharBuffer::peak_uint16(uint16_t startingIndex) const
{
    uint16_t d = this->peak(startingIndex);
    d |= this->peak(startingIndex + 1) << 8;
    return d;
}

uint32_t CharBuffer::peak_uint32() const
{
    uint32_t d = this->peak(0);
    d |= this->peak(1) << 8;
    d |= this->peak(2) << 16;
    d |= this->peak(3) << 24;
    return d;
}
