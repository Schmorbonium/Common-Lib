// Note these Templates need to be in Header completely so This is Good
#ifndef __IsaacTypes_
#define __IsaacTypes_




#include "zHal.h"

#define HYBRID_NODE_SIZE 16

template <typename T>
class Queue;

template <typename T>
class HybridNode
{
    friend class Queue<T>;
    T data[HYBRID_NODE_SIZE];
    HybridNode *next;
    HybridNode() : next(nullptr) {}
};

template <typename T>
class Queue
{
public:
    Queue() : head(nullptr), tail(nullptr), size(0)
    {
        InitNodes();
    }
    ~Queue()
    {
        clear();
    }
    void enqueue(T value)
    {
        tailIndex++;
        if (tailIndex == HYBRID_NODE_SIZE)
        {
            this->AppendNewTailNode();
        }
        tail->data[tailIndex] = value;
        size++;
    }
    T dequeue()
    {
        if (size == 0)
        {
            return head->data[0];
        }

        T value = head->data[headIndex];
        headIndex++;
        size--;
        if (headIndex == HYBRID_NODE_SIZE)
        {
            this->FreeHeadNode();
        }
        return value;
    }
    bool isEmpty() const
    {
        return size == 0;
    }
    uint16_t getSize() const
    {
        return size;
    }
    void clear()
    {
        while (this->head != nullptr)
        {
            FreeHeadNode();
        }
    }

private:
    void FreeHeadNode()
    {
        HybridNode<T> *temp = head;
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;
        }
        delete temp;
        headIndex = 0;
    }

    void AppendNewTailNode()
    {
        HybridNode<T> *newNode = new HybridNode<T>();
        tail->next = newNode;
        tail = newNode;
        tailIndex = 0;
    }

    void InitNodes()
    {
        HybridNode<T> *newNode = new HybridNode<T>();
        head = newNode;
        tail = newNode;
        tailIndex = 0;
        headIndex = 0;
        size = 0;
    }

    HybridNode<T> *head;
    HybridNode<T> *tail;
    uint16_t size;
    uint8_t tailIndex;
    uint8_t headIndex;
};



//  ID Manager
class IDManager {
public:
    IDManager();
    uint16_t allocateID();
    void freeID(uint16_t id);
private:
    int nextID;
    Queue<uint16_t> freedIDs;
};
#endif