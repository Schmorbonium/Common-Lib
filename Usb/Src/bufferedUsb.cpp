
#include "zHal.h"
#include "iBufferedChannel.hpp"
#include "bufferedUsb.hpp"
#include <fcntl.h>   // File Control Definitions
#include <termios.h> // POSIX Terminal Control Definitions
#include <unistd.h>  // UNIX Standard Definitions
#include <errno.h>   // ERROR Number Definitions
#include <stdio.h>
#include <string.h>
// #include "iQueue.hpp"
// #include "ringCharBuffer.hpp"

BufferedUsb::BufferedUsb(const char *interfaceName)
{

    this->fd = open(interfaceName, O_RDWR);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if (tcgetattr(this->fd, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        _exit(-1);
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;         // Clear all bits that set the data size
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 0; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(this->fd, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        _exit(-1);
    }

    // // Write to serial port
    // unsigned char msg[] = {'H', 'e', 'l', 'l', 'o', '\r'};
    // write(serial_port, msg, sizeof(msg));

    // // Allocate memory for read buffer, set size according to your needs
    // char read_buf[256];

    // // // Normally you wouldn't do this memset() call, but since we will just receive
    // // // ASCII data for this example, we'll set everything to 0 so we can
    // // // call printf() easily.
    // // memset(&read_buf, '\0', sizeof(read_buf));

    // // Read bytes. The behaviour of read() (e.g. does it block?,
    // // how long does it block for?) depends on the configuration
    // // settings above, specifically VMIN and VTIME
    // int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    // if (num_bytes < 0)
    // {
    //     printf("Error reading: %s", strerror(errno));
    //     return 1;
    // }

    // fd = open(interfaceName, O_RDWR);
    // if (fd == -1)
    // { // Error Checking
    //     perror("Error - Unable to open UART. Ensure it is not in use by another application\n");
    //     _exit(-1);
    // }
    // // Configure Port Settings
    // struct termios SerialPortSettings;
    // tcgetattr(fd, &SerialPortSettings); // Get the current attributes of the Serial port

    // cfsetispeed(&SerialPortSettings, B1152000); // Set Read Speed
    // cfsetospeed(&SerialPortSettings, B1152000); // Set Write Speed

    // SerialPortSettings.c_cflag &= ~PARENB; // No Parity
    // SerialPortSettings.c_cflag &= ~CSTOPB; // Stop bits = 1
    // SerialPortSettings.c_cflag &= ~CSIZE;  // Clears the Mask
    // SerialPortSettings.c_cflag |= CS8;     // Set the data bits = 8

    // SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control
    // SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines

    // // SerialPortSettings.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    // SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non Cannonical mode

    // SerialPortSettings.c_oflag &= ~(OPOST | ONLCR); // No Output Processing

    // SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Disable XON/XOFF flow control
    // SerialPortSettings.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    // SerialPortSettings.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    // SerialPortSettings.c_cc[VMIN] = 0;

    // // Set the attributes to the termios structure
    // if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
    // {
    //     printf("\n  ERROR ! in Setting attributes\n");
    //     _exit(-1);
    // }
    // else
    // {
    // }
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

void BufferedUsb::startSending()
{
}
void BufferedUsb::stopSending()
{
}

void BufferedUsb::asyncHandler()
{
    uint16_t data;
    while (this->TxQue.getSize() > 0)
    {
        data = this->TxQue.pop();
        uint16_t error = write(this->fd, &data, 1);
    }
    int readLen = read(this->fd, &data, 1);
    while (readLen > 0)
    {
        RxQue.append(data);
        readLen = read(this->fd, &data, 1);
        if (readLen == -1)
        {
            perror("Error reading from UART");
            // Handle error appropriately
        }
    }
}
