#include "isaacsTypes.hpp"

IDManager::IDManager()
    :   nextID(0),
    freedIDs()
{
}


uint16_t IDManager::allocateID()
{
    if (freedIDs.isEmpty())
    {
        return nextID++;
    }
    else
    {
        return freedIDs.dequeue();
    }
}
void IDManager::freeID(uint16_t id)
{
    freedIDs.enqueue(id);
}
