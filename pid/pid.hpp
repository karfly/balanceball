#ifndef PID_HPP
#define PID_HPP

#include <string>

// Input for pid is error(t) = x0 - x (difference beetwen required value (x0) and current value (x))
class PID
{

public:
        PID();
        PID( const char* configPath );                // configPath - path to the PID config file

        ~PID();

        void initByFile( const char* configPath );    // configPath - path to the PID config file

        void compute( double input );           // Computing the output signal

        double getOutput();                     // Get the output after computing

        bool fail();
        std::string getErrorStr();

private:
        void setFail();
        void setErrorStr( std::string errorString );

        double wP_;                     // P, I, D weights
        double wI_;                     //
        double wD_;                     //

        double lastInput_;              // Last input, given 1 period ago
        double integralSum_;            // Integral sum

        double output_;                 // Output after computing

        double period_;                 // Refreshing period in milliseconds

        double minOutput_;              // Output is limited
        double maxOutput_;              //

        bool fail_;                     // True on fail
        std::string errorString_;       // If fail_ is true, here is a valid reason of the error
};

#endif // PID_HPP
