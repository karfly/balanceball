#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using std::string;

class camera
{

public:
        camera();
        ~camera();

        void refresh();

        int getX(); 
        int getY();

        bool fail();


        string getErrorStr();

        // Constants for white ball 
        static const int lowH_       = 0;        // Hue (0 - 179)
        static const int highH_      = 32;       //

        static const int lowS_       = 0;        // Saturation (0 - 255)
        static const int highS_      = 71;       //
        
        static const int lowV_       = 255;      // Value (0 - 255)
        static const int highV_      = 255;      //

        static const int ballArea_    = 300000;   // Approximate ball area (compare it with objArea_)

private:
        void setFail();
        void setErrorStr(string );

        int x_;                         // If coordinates' values equal to (-1), it means 
        int y_;                         // that there is no object on the frame

        cv::VideoCapture camera_;       // Web camera

        cv::Mat imgOriginal_;           // New frame from camera
        cv::Mat imgHSV_;                // Original, converted to HSV
        cv::Mat imgThresholded_;        // Thresholded HSV image 

        cv::Moments objMoments_;        // Moments of thresholded picture
        double  objMoment01_;           // Responsible for Y
        double  objMoment10_;           // Responsible for X
        double  objArea_;               // Number of pixels of the object

        bool fail_;                     // True on fail
        string errorString_;            // If fail_ is true, here is a valid reason of the error
};



#endif // CAMERA_HPP