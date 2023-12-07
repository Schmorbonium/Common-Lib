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
        node->appendDependencies();
        // if(node->evaluate()){
        // }
    }
}

/*********************************************
 * IDependencyNode
 **********************************************/
IDependencyNode::IDependencyNode(DependencyTree *parentTree)
{
    this->priority = 0;
    parent = parentTree;
}

void IDependencyNode::addDependent(IDependencyNode *dependee)
{
    dependees.add(dependee);
}

void IDependencyNode::addDependency(IDependencyNode *dependee)
{
    dependee->addDependent(this);
    if (dependee->priority >= this->priority)
    {
        this->priority = dependee->priority + 1;
    }
}

uint8_t IDependencyNode::getPriority()
{
    return priority;
}

bool IDependencyNode::evaluate()
{
    return false;
}

void IDependencyNode::appendDependencies()
{
    for (uint8_t i = 0; i < dependees.getSize(); i++)
    {
        parent->addToQueue(dependees.get(i));
    }
}