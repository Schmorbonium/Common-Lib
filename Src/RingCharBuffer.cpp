#include "ringCharBuffer.hpp"

void RingCharBuffer::initNodes()
{
    Node *a = new Node();
    Node *b = new Node();
    Node *c = new Node();

    a->active = 0;
    b->active = 0;
    c->active = 0;

    a->next = b;
    b->next = c;
    c->next = a;

    a->last = c;
    b->last = a;
    c->last = b;

    head = a;
    tail = a;
    head->active = 1;
    headIndex = 0;
    tailIndex = 0;

    size = 0;
    nodeCount = 3;
    activeNodeCount = 1;
}

void RingCharBuffer::insertNode()
{
    Node *temp = head->next;
    Node *newNode = new Node();
    newNode->last = head;
    newNode->next = temp;
    head->next = newNode;
    temp->last = newNode;
    nodeCount = nodeCount + 1;
}

void RingCharBuffer::removeNode()
{
    Node *temp = tail->last;
    tail->last = temp->last;
    temp->last->next = tail;
    delete temp;
    nodeCount = nodeCount + 1;
}

void RingCharBuffer::advanceTail()
{
    tail->active = 0;
    tail = tail->next;
    activeNodeCount = activeNodeCount - 1;
    tailIndex = 0;
}
void RingCharBuffer::advanceHead()
{
    if (head->next->active)
    {
        // Allocate a new node if we need one
        insertNode();
    }

    activeNodeCount = activeNodeCount + 1;
    head = head->next;
    head->active = 1;
    headIndex = 0;
}

RingCharBuffer::RingCharBuffer()
{
    initNodes();
}
RingCharBuffer::~RingCharBuffer()
{
    Node *temp = head->next;
    head->next = nullptr;
    while (temp->next != nullptr)
    {
        temp = temp->next;
        delete temp->last;
    }
    delete temp;
}


// Append a new Character to the end of the buffer
void RingCharBuffer::append(uint8_t c)
{
    if (this->headIndex >= NODE_SIZE)
    {
        advanceHead();
    }
    head->data[headIndex] = c;
    headIndex++;
    size++;
    checkSumAppended += c;
}

// Remove and return the first character in the buffer
uint8_t RingCharBuffer::pop()
{
    if (this->tailIndex >= NODE_SIZE)
    {
        advanceTail();
    }
    uint8_t val = tail->data[tailIndex];
    tailIndex++;
    size--;
    checkSumPopped += val;
    return val;
}

// Peaks at the value at a given index
uint8_t RingCharBuffer::peak(uint16_t index) const
{
    if (index >= size)
    {
        return 0;
    }
    uint16_t relativeIndex = index + tailIndex;
    uint16_t NodeDistance = relativeIndex / NODE_SIZE;
    uint16_t ActualIndex = relativeIndex % NODE_SIZE;
    Node *temp = tail;
    for (uint16_t i = 0; i < NodeDistance; i++)
    {
        temp = temp->next;
    }

    uint8_t val = temp->data[ActualIndex];

    return val;
}

// Get the current size of the buffer
uint16_t RingCharBuffer::getSize() const
{
    return size;
}

// Check if the buffer is empty
bool RingCharBuffer::isEmpty() const
{
    return (size == 0);
}

// Clear the buffer and release memory
void RingCharBuffer::clear()
{
    while (tail != head)
    {
        advanceTail();
    }
    tailIndex = 0;
}

uint8_t RingCharBuffer::getPoppedCheckSum()
{
    return this->checkSumPopped;
}
uint8_t RingCharBuffer::setPoppedCheckSum(uint8_t value)
{
    uint16_t t = this->checkSumPopped;
    this->checkSumPopped = value;
    return t;
}

uint8_t RingCharBuffer::getQueuedCheckSum()
{
    return this->checkSumAppended;
}
uint8_t RingCharBuffer::setQueuedCheckSum(uint8_t value)
{
    uint8_t t = this->checkSumAppended;
    this->checkSumAppended = value;
    return t;
}
