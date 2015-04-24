#include <string>

#ifndef PLATFORM_HPP
#define PLATFORM_HPP




class Platform
{

public:
        Platform();
        Platform( const char* devicePath, int baudRate );

        ~Platform();

        void setAngles( int angleX, int angleY );

        bool fail();
        std::string getErrorStr();

private:
        int initConnection( const char* devicePath, int baudRate );     // Takes path to the device file and returns fd of it

        void setFail();
        void setErrorStr( std::string errorString );

        int platformFD_;                // Platform's file descriptor

        int angleServoX_;               // Current ServoX's angle (-100 : 100)
        int angleServoY_;               // Current ServoY's angle (-100 : 100)

        bool fail_;                     // True on fail
        std::string errorString_;       // If fail_ is true, here is a valid reason of the error
};

#endif // PLATFORM_HPP