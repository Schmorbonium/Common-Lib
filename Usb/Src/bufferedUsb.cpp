
#include "zHal.h"
#include "iBufferedChannel.hpp"
#include "bufferedUsb.hpp"
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>   // ERROR Number Definitions
#include <stdio.h>
// #include "iQueue.hpp"
// #include "ringCharBuffer.hpp"

BufferedUsb::BufferedUsb(const char *interfaceName)
{
    fd = open(interfaceName, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
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

BufferedUsb::~BufferedUsb()
{
    // close(this->fd);
}

void BufferedUsb::startListening()
{
}
void BufferedUsb::stopListening()
{
}

void BufferedUsb::startSending(){
    
}
void BufferedUsb::stopSending(){
    
}

void BufferedUsb::asyncHandler()
{
    uint8_t data;
    if (this->TxQue.getSize() > 0)
    {
        data = this->TxQue.pop();
        uint16_t error = write(this->fd, &data, 1);
    }
    while (read(this->fd, &data, 1) > 0)
    {
        RxQue.append(data);
    }
}
