// Note these Templates need to be in Header completely so This is Good
#ifndef __IsaacTypes_
#define __IsaacTypes_




#include "zHal.h"

#define HYBRID_NODE_SIZE 16

template <typename T>
struct HybridNode
{
    T data[HYBRID_NODE_SIZE];
    HybridNode *next;
    HybridNode() : next(nullptr) {}
};
// Hybrid Queue (Gotta reduce that allocation time)
template <typename T>
class Queue
{

protected:

public:
    Queue() : head(nullptr), tail(nullptr), size(0)
    {
        InitNodes();
    }
    ~Queue()
    {
        clear();
    }

    // Delete copy constructor and copy assignment operator
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // Delete move constructor and move assignment operator
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

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


template <typename T>
class ArrayList {
public:
    ArrayList() : size(0) {}

    bool add(T* item) {
        if (size < MAX_SIZE) {
            array[size++] = item;
            return true;
        }
        return false;
    }

    T* get(size_t index) const {
        if (index < size) {
            return array[index];
        }
        return nullptr;
    }

    bool remove(size_t index) {
        if (index < size) {
            for (size_t i = index; i < size - 1; ++i) {
                array[i] = array[i + 1];
            }
            size--;
            return true;
        }
        return false;
    }

    uint16_t getSize() const {
        return size;
    }

    bool isFull() const {
        return size == MAX_SIZE;
    }

    bool isEmpty() const {
        return size == 0;
    }

private:
    static const size_t MAX_SIZE = 4;
    T* array[MAX_SIZE];
    uint16_t size;
};
// Map Built on linked list
// template<typename Key, typename Value>
// class LinkedListMap {
// private:
//     struct LinkListNode {
//         Key key;
//         Value value;
//         LinkListNode* next;

//         LinkListNode(const Key& k, const Value& v, LinkListNode* n) : key(k), value(v), next(n) {}
//     };

//     LinkListNode* head;

// public:
//     LinkedListMap() : head(nullptr) {}

//     ~LinkedListMap() {
//         Node* current = head;
//         while (current != nullptr) {
//             Node* temp = current;
//             current = current->next;
//             delete temp;
//         }
//     }

//     void insert(const Key& key, const Value& value) {
//         // If the Key is in the set update it
//         for (Node* current = head; current != nullptr; current = current->next) {
//             if (current->key == key) {
//                 current->value = value;
//                 return;
//             }
//         }
//         // Otherwise we are adding it to the front of the list
//         head = new Node(key, value, head);
//     }

//     Value* find(const Key& key) {
//         for (Node* current = head; current != nullptr; current = current->next) {
//             if (current->key == key) {
//                 return &current->value;
//             }
//         }
//         return nullptr;
//     }
// };


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