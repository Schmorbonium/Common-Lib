#include "zHal.h"
#include "isaacsTypes.hpp"

class IDependencyNode;

class DependencyTree{
public:
    void addToQueue(IDependencyNode *node);
    void resolve();

private:
    PriorityQueue<IDependencyNode> queue;
};

class IDependencyNode{
    private:
    DependencyTree *parent;
    uint8_t priority;
    ArrayList<IDependencyNode> dependees;
    void addDependent(IDependencyNode *dependee);
public:
    virtual bool evaluate();
    IDependencyNode(DependencyTree *parentTree);
    void addDependency(IDependencyNode *dependee);
    uint8_t getPriority();
    void appendDependencies();
protected:
};



