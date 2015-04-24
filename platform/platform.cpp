#include "platform.hpp"

#include <iostream>
#include <string>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>    // POSIX terminal control definitions
#include <sys/ioctl.h>


// Constructors

Platform::Platform() :  platformFD_(0),

                        angleServoX_(0),
                        angleServoY_(0),

                        fail_(false),
                        errorString_("")
{

}

Platform::Platform( const char* devicePath, int baudRate ) :  angleServoX_(0),
                        angleServoY_(0),

                        fail_(false),
                        errorString_("")
{
        platformFD_ = this->initConnection( devicePath, baudRate );
}

// Desctructor
Platform::~Platform()
{
        int ret = 0;

        ret = close( platformFD_ );
        if (ret == -1)
        {
                this->setFail();
                this->setErrorStr( "Failed to close connection with device. " + (std::string)strerror(errno) );
        }

        platformFD_ = -1;

        angleServoX_ = -1;
        angleServoY_ = -1;

        fail_ = true;
        errorString_ = "";
}

// Methods

void
Platform::setAngles( int angleX, int angleY )
{
        angleServoX_ = angleX;
        angleServoY_ = angleY;

        std::string angleX_str = std::to_string( angleX );
        std::string angleY_str = std::to_string( angleY );

        std::string signal = angleX_str + ":" + angleY_str;
        signal[signal.length()] = '$';

        int ret = 0;

        ret = write(platformFD_, signal.c_str(), signal.length() + 1);
        if (ret == -1)
        {
                this->setFail();
                this->setErrorStr( "Failed to write to device. " + (std::string)strerror(errno) );

                return;
        }
}


int
Platform::initConnection( const char* devicePath, int baudRate )
{
        int fd = 0;
        int ret = 0;

        struct termios terminalOptions;         // POSIX structure for configurating terminal devices

        fd = open( devicePath, O_WRONLY | O_NDELAY | O_NOCTTY );
        if (fd == -1)
        {
                this->setFail();
                this->setErrorStr( "Failed to open: " + (std::string)devicePath + ". " + (std::string)strerror(errno) );

                return -1;
        }

        memset( &terminalOptions, 0, sizeof( struct termios ) );        // Cleaning up the structure
        cfmakeraw(&terminalOptions);                                    //

        cfsetspeed(&terminalOptions, baudRate);

        terminalOptions.c_cflag = CLOCAL;       // If CLOCAL is set, the line behaves as if DCD is always asserted.
                                                // It is used when your device is local

        terminalOptions.c_cflag |= CS8;         // Character size mask

        terminalOptions.c_cc[VMIN] = 0;         // 1 second timeout
        terminalOptions.c_cc[VTIME] = 10;       //

        ret = ioctl( fd, TIOCSETA, &terminalOptions );  // Configuring the device
        if (ret == -1)
        {
                this->setFail();
                this->setErrorStr( "Failed to failed to configure device: " + (std::string)devicePath + ". " + (std::string)strerror(errno) );

                return -1;
        }

        return fd;
}

void
Platform::setFail()
{
        fail_ = true;
}

bool
Platform::fail()
{
        return fail_;
}

void
Platform::setErrorStr(std::string errorString)
{
        errorString_ = errorString;
}

std::string
Platform::getErrorStr()
{
        return errorString_;
}
