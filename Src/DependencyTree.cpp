#include "DependencyTree.hpp"

void DependencyTree::addToQueue(IDependencyNode *node)
{
    queue.enqueue(node);
}

void DependencyTree::resolve()
{
    while (!queue.isEmpty())
    {
        IDependencyNode *node = queue.dequeue();
        node->evaluate();
    }
}

/*********************************************
 * IDependencyNode
 **********************************************/
IDependencyNode::IDependencyNode(DependencyTree *parentTree)
{
    parent = parentTree;
}

void IDependencyNode::addDependency(IDependencyNode *dependee)
{
    dependees.add(dependee);
}

uint8_t IDependencyNode::getPriority()
{
    return priority;
}

void IDependencyNode::appendDependencies()
{
    for (uint8_t i = 0; i < dependees.getSize(); i++)
    {
        parent->addToQueue(dependees.get(i));
    }
}