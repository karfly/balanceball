#include "./camera/camera.hpp"

#include <iostream>
#include <unistd.h>

#define ESC_SC 27

using std::cout;
using std::cerr;
using std::endl;

int main()
{

Camera camera;
if ( camera.fail() )
{
        cerr << camera.getErrorStr() << endl;
        exit( EXIT_FAILURE );
}

while (true)
{
        camera.refresh();

        if ( cv::waitKey(20) == ESC_SC ) // Waiting for 'ESC' key press for 30ms. If 'ESC' key is pressed, break loop
        {
                cout << "'ESC' key is pressed." << endl;
                break; 
        }

}


exit( EXIT_SUCCESS );

}