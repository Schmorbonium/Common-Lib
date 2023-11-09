
#include "charBuffer.hpp"




class UartCommand
{
    public:
    UartCommand();
    virtual void serialize(CharBuffer* buff);
};