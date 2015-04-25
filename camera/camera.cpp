#include "camera.hpp"

// Constructor
Camera::Camera() : x_(0),
                   y_(0),

                   camera_(0),

                   imgOriginal_(),
                   imgHSV_(),
                   imgThresholded_(),

                   objMoments_(),
                   objMoment01_(0.0),
                   objMoment10_(0.0),
                   objArea_(0.0),

                   fail_(false),
                   errorString_("")
{
        if ( !camera_.isOpened() )  // If not success, exit program
        {
                this->setFail();
                this->setErrorStr( "Failed to open camera." );
        }

        cv::Mat imgTmp;
        camera_.read( imgTmp );

        // Creating a black image with the size as the camera output
        imgSquare_ = cv::Mat::zeros( imgTmp.size(), CV_8UC3 );
}

// Destructor
Camera::~Camera()
{
        x_ = -1;
        y_ = -1;

        camera_.~VideoCapture();

        imgOriginal_.~Mat();
        imgHSV_.~Mat();
        imgThresholded_.~Mat();

        objMoments_.~Moments();
        objMoment01_ = -1;
        objMoment10_ = -1;
        objArea_     = -1;

        fail_ = true;
        errorString_ = "";
}

// Methods

void
Camera::refresh()
{
        int ret = 0;

        ret = camera_.read( imgOriginal_ ); // Reading new frame from video stream
        if ( !ret )
        {
                this->setFail();
                this->setErrorStr( "Failed to read frame from video stream." );

                return;
        }

        cv::cvtColor( imgOriginal_, imgHSV_, cv::COLOR_BGR2HSV ); // Converting the captured frame from BGR to HSV

        cv::inRange( imgHSV_,                                     // Thresholding the image
                     cv::Scalar(lowH_,  lowS_,  lowV_ ),
                     cv::Scalar(highH_, highS_, highV_),
                     imgThresholded_ );

        // Morphological opening (removes small objects from the foreground)
        cv::erode ( imgThresholded_, imgThresholded_, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
        cv::dilate( imgThresholded_, imgThresholded_, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );


        // Morphological closing (removes small holes from the foreground)
        cv::dilate( imgThresholded_, imgThresholded_, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
        cv::erode ( imgThresholded_, imgThresholded_, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

        // Calculating the moments of the thresholded image
        objMoments_ = cv::moments( imgThresholded_ );

        objMoment01_ = objMoments_.m01;
        objMoment10_ = objMoments_.m10;
        objArea_     = objMoments_.m00;

        if (objArea_ > ballArea_)
        {
                x_ = objMoment10_ / objArea_;
                y_ = objMoment01_ / objArea_;

                // Drawing red square around the ball
                line( imgSquare_, cv::Point(x_ - ballRadius_, y_ + ballRadius_),        // Vertical lines
                                  cv::Point(x_ - ballRadius_, y_ - ballRadius_),        //
                                  cv::Scalar(0,0,255), 2);                              //
                                                                                        //
                line( imgSquare_, cv::Point(x_ + ballRadius_, y_ + ballRadius_),        //
                                  cv::Point(x_ + ballRadius_, y_ - ballRadius_),        //
                                  cv::Scalar(0,0,255), 2);                              //

                line( imgSquare_, cv::Point(x_ - ballRadius_, y_ + ballRadius_),        // Horizontal lines
                                  cv::Point(x_ + ballRadius_, y_ + ballRadius_),        //
                                  cv::Scalar(0,0,255), 2);                              //
                                                                                        //
                line( imgSquare_, cv::Point(x_ - ballRadius_, y_ - ballRadius_),        //
                                  cv::Point(x_ + ballRadius_, y_ - ballRadius_),        //
                                  cv::Scalar(0,0,255), 2);                              //
        }

        cv::imshow( "Thresholded Image", imgThresholded_ ); // Showing the thresholded image

        imgOriginal_ = imgOriginal_ + imgSquare_;
        cv::imshow( "Original", imgOriginal_ ); // Showing the original image

        imgSquare_ = cv::Mat::zeros( imgSquare_.size(), CV_8UC3 ); // Clearing the imgSquare_

}

int
Camera::getX()
{
        return x_;
}

int
Camera::getY()
{
        return y_;
}

void
Camera::setFail()
{
        fail_ = true;
}

bool
Camera::fail()
{
        return fail_;
}

void
Camera::setErrorStr( std::string errorString )
{
        errorString_ = errorString;
}

std::string
Camera::getErrorStr()
{
        return errorString_;
}
