#include "./platform/platform.hpp"
#include "./camera/camera.hpp"
#include "./pid/pid.hpp"

#include <iostream>
#include <cmath>

#include <unistd.h>

#define REFRESH_PERIOD  5      // In milliseconds
#define ESC_SC          27     // ESC scan code

#define NORMAL          0
#define SQUARE          1

#define X_BALANCE       440
#define Y_BALANCE       300

#define SQUARE_SIDE     140
#define SQUARE_PERIOD   250


using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[])
{

int mode = NORMAL;

if ( argc == 2 )
{

        if ( !strcmp( argv[1], "square" ) )
        {
                mode = SQUARE;
        }

}

Platform platform( "/dev/tty.usbmodem1421", 9600 );
if ( platform.fail() )
{
        cerr << platform.getErrorStr() << endl;
        exit( EXIT_FAILURE );
}

Camera camera;
if ( camera.fail() )
{
        cerr << camera.getErrorStr() << endl;
        exit( EXIT_FAILURE );
}

PID pidServoX( "./pid/pidServoX_config" );
if ( pidServoX.fail() )
{
        cerr << pidServoX.getErrorStr() << endl;
        exit( EXIT_FAILURE );
}
PID pidServoY( "./pid/pidServoX_config" );
if ( pidServoY.fail() )
{
        cerr << pidServoY.getErrorStr() << endl;
        exit( EXIT_FAILURE );
}

switch( mode )
{

case NORMAL:
        {

        while ( true )
        {
                camera.refresh();
                ///
                cout << "-----------" << endl;
                cout << "X: " << camera.getX() << endl;
                cout << "Y: " << camera.getY() << endl;
                ///

                pidServoX.compute( X_BALANCE - camera.getX() );
                pidServoY.compute( Y_BALANCE - camera.getY() );

                cout << "errorX: " << X_BALANCE - camera.getX() << endl;
                cout << "errorY: " << Y_BALANCE - camera.getY() << endl;

                cout << "signalX: " << pidServoX.getOutput() << endl;
                cout << "signalY: " << pidServoY.getOutput() << endl;

                platform.setAngles( -pidServoX.getOutput(), -pidServoY.getOutput() );

                cout << "-----------" << endl;


                if ( cv::waitKey(REFRESH_PERIOD) == ESC_SC ) // Waiting for 'ESC' key press for 30ms. If 'ESC' key is pressed, break loop
                {

                        platform.setAngles( 0, 0 );

                        cout << "'ESC' key was pressed." << endl;
                        break;
                }
        }

        break;

        }

case SQUARE:
        {

        int xSquare = X_BALANCE - SQUARE_SIDE;
        int ySquare = Y_BALANCE - SQUARE_SIDE;

        int count = 0;

        while ( true )
        {
                camera.refresh();
                ///
                cout << "-----------" << endl;
                cout << "X: " << camera.getX() << endl;
                cout << "Y: " << camera.getY() << endl;
                ///

                count++;
                if ( count == SQUARE_PERIOD / REFRESH_PERIOD * 1)
                {
                        xSquare = X_BALANCE + SQUARE_SIDE;
                        ySquare = Y_BALANCE - SQUARE_SIDE;
                }

                if ( count == SQUARE_PERIOD / REFRESH_PERIOD * 2 )
                {
                        xSquare = X_BALANCE + SQUARE_SIDE;
                        ySquare = Y_BALANCE + SQUARE_SIDE;
                }

                if ( count == SQUARE_PERIOD / REFRESH_PERIOD * 3)
                {
                        xSquare = X_BALANCE - SQUARE_SIDE;
                        ySquare = Y_BALANCE + SQUARE_SIDE;
                }

                if ( count == SQUARE_PERIOD / REFRESH_PERIOD * 4)
                {
                        xSquare = X_BALANCE - SQUARE_SIDE;
                        ySquare = Y_BALANCE - SQUARE_SIDE;

                        count = 0;
                }

                pidServoX.compute( xSquare - camera.getX() );
                pidServoY.compute( ySquare - camera.getY() );

                cout << "squareX: " << xSquare << endl;
                cout << "squareY: " << ySquare << endl;

                cout << "errorX: " << xSquare - camera.getX() << endl;
                cout << "errorY: " << ySquare - camera.getY() << endl;

                cout << "signalX: " << pidServoX.getOutput() << endl;
                cout << "signalY: " << pidServoY.getOutput() << endl;

                platform.setAngles( -pidServoX.getOutput(), -pidServoY.getOutput() );

                cout << "-----------" << endl;


                if ( cv::waitKey(REFRESH_PERIOD) == ESC_SC ) // Waiting for 'ESC' key press for 30ms. If 'ESC' key is pressed, break loop
                {

                        platform.setAngles( 0, 0 );

                        cout << "'ESC' key was pressed." << endl;
                        break;
                }
        }

        break;

        }

}


exit( EXIT_SUCCESS );

}
