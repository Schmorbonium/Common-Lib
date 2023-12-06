#include "isaacsTypes.hpp"


class IUpdateListener{
    public:
    virtual bool PropagateValue() {return false;}
};

template <typename T>
class PropVal
{
public:
    PropVal(T value) :
    {
        IUpdateListener
    }
    ~PropVal()
    {
    }

    // Delete copy constructor and copy assignment operator
    PropVal(const PropVal &) = delete;
    PropVal &operator=(const PropVal &) = delete;

    // Delete move constructor and move assignment operator
    PropVal(PropVal &&) = delete;
    PropVal &operator=(PropVal &&) = delete;

    void set(T value)
    {
        needUpdate = true;
        this->value = value;
    }
    T get()
    {
        return value;
    }
    void addListener(&IUpdateListener listener){
        this->listeners.add(listener);
    }
    void updateDependant(){
        for (uint16_t i = 0; i < listeners.getSize(); i++)
        {
            listeners.get(i)->PropagateValue();
        }
    }
private:
    T value;
    bool needUpdate;
    uint16_t numberOfListeners;
    ArrayList<IUpdateListener> listeners;
};