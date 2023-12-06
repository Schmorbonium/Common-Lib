#ifndef __USB_CHANNEL__
#define __USB_CHANNEL__

#include "zHal.h"
#include "iBufferedChannel.hpp"




class BufferedUsb : public IBufferedChannel
{
protected:
    virtual void startSending();
    virtual void stopSending();
    int fd;

public:
    BufferedUsb(const char *interfaceName);
    ~BufferedUsb();

    // Delete copy constructor and copy assignment operator
    BufferedUsb(const BufferedUsb &) = delete;
    BufferedUsb &operator=(const BufferedUsb &) = delete;

    // Delete move constructor and move assignment operator
    BufferedUsb(BufferedUsb &&) = delete;
    BufferedUsb &operator=(BufferedUsb &&) = delete;

    virtual void startListening();
    virtual void stopListening();
    virtual void asyncHandler();
};
#endif // End __USB_CHANNEL__