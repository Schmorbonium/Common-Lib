#ifndef __USB_CHANNEL__
#define __USB_CHANNEL__

#include "zHal.h"
#include "iBufferedChannel.hpp"
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>   // ERROR Number Definitions
#include <stdio.h>
// #include "iQueue.hpp"
// #include "ringCharBuffer.hpp"

class BufferedUsb : public IBufferedChannel
{
protected:
    virtual void startSending();
    virtual void stopSending();
    int fd;

public:
    BufferedUsb(char *interfaceName)
    {
        fd = open(interfaceName, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1)
        { // Error Checking
            perror("Error - Unable to open UART. Ensure it is not in use by another application\n");
        }
        // Configure Port Settings
        struct termios SerialPortSettings;
        tcgetattr(fd, &SerialPortSettings); // Get the current attributes of the Serial port

        cfsetispeed(&SerialPortSettings, B115200); // Set Read Speed
        cfsetospeed(&SerialPortSettings, B115200); // Set Write Speed

        SerialPortSettings.c_cflag &= ~PARENB; // No Parity
        SerialPortSettings.c_cflag &= ~CSTOPB; // Stop bits = 1
        SerialPortSettings.c_cflag &= ~CSIZE;  // Clears the Mask
        SerialPortSettings.c_cflag |= CS8;     // Set the data bits = 8

        SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control
        SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines

        SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable XON/XOFF flow control
        SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non Cannonical mode

        SerialPortSettings.c_oflag &= ~OPOST; // No Output Processing

        // Set the attributes to the termios structure
        if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
        {
            printf("\n  ERROR ! in Setting attributes\n");
        }
        else
        {
        }
    }
} ~BufferedUsb();

// Delete copy constructor and copy assignment operator
BufferedUsb(const BufferedUsb &) = delete;
BufferedUsb &operator=(const BufferedUsb &) = delete;

// Delete move constructor and move assignment operator
BufferedUsb(BufferedUsb &&) = delete;
BufferedUsb &operator=(BufferedUsb &&) = delete;

virtual void startListening(){

}
virtual void stopListening(){

}
virtual void asyncHandler(){
    
}



#endif // End __USB_CHANNEL__