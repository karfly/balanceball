#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Camera
{

public:
        Camera();
        ~Camera();

        void refresh();

        int getX();
        int getY();

        bool fail();
        std::string getErrorStr();

        // Constants for white ball
        static const int lowH_       = 0;        // Hue (0 - 179)
        static const int highH_      = 32;       //

        static const int lowS_       = 0;        // Saturation (0 - 255)
        static const int highS_      = 71;       //

        static const int lowV_       = 255;      // Value (0 - 255)
        static const int highV_      = 255;      //

        static const int ballArea_    = 300000;  // Approximate ball area (compare it with objArea_)
        static const int ballRadius_  = 37;      // Approximate ball radis to circle it with square

private:
        void setFail();
        void setErrorStr( std::string errorString );

        int x_;                         // If coordinates' values equal to (-1), it means
        int y_;                         // that there is no object on the frame

        cv::VideoCapture camera_;       // Web camera

        cv::Mat imgOriginal_;           // New frame from camera
        cv::Mat imgHSV_;                // Original, converted to HSV
        cv::Mat imgThresholded_;        // Thresholded HSV image

        cv::Mat imgSquare_;             // For circling the ball in a square

        cv::Moments objMoments_;        // Moments of thresholded picture
        double  objMoment01_;           // Responsible for Y
        double  objMoment10_;           // Responsible for X
        double  objArea_;               // Number of pixels of the object

        bool fail_;                     // True on fail
        std::string errorString_;       // If fail_ is true, here is a valid reason of the error
};

#endif // CAMERA_HPP