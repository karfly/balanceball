/* 
This programm takes a string from a serial port, parses it
and move servo on a given angle.
Each message must be like (angle in degrees):
  "[SERVO1_ANGLE]:[SERVO2_ANGLE]$"
*/

char foo; // It's a magic trick with arduino. Without it program doesn't compile.

#include <Servo.h>


//::: Constants of the device :::\\
const int SERVO1_BALANCE_ANGLE = 68;
const int SERVO2_BALANCE_ANGLE = 63;

const int SERVO1_MIN_ANGLE = 32;
const int SERVO2_MIN_ANGLE = 26;

const int SERVO1_MAX_ANGLE = 104;
const int SERVO2_MAX_ANGLE = 100;

const int SERVO_MAP_MAX_ANGLE =  100;   // In percents
const int SERVO_MAP_MIN_ANGLE = -100;   //

const int SERVO1_PIN = 9;
const int SERVO2_PIN = 10;
//:::::::::::::::::::::::::::::::\\
 
Servo servo1;
Servo servo2;

String inData;  // Buffer for getting data
int servo1_angle = 0;
int servo2_angle = 0;
 
void setup() 
{
  Serial.begin(9600);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  servo1.write(68);  // Here must SERVOx_BALANCE_ANGLE,
  servo2.write(63);  // but arduino compiler is stupid
} 
 
void loop() 
{
  if (Serial.available() > 0) {
  
    // Reading angle of the servo1
    inData = Serial.readStringUntil(':');
    servo1_angle = inData.toInt();
    
    // Reading angle of the servo2
    inData = Serial.readStringUntil('$');
    servo2_angle = inData.toInt();

    // Checking if the angles are correct
    if (servo1_angle < SERVO_MAP_MIN_ANGLE || servo1_angle > SERVO_MAP_MAX_ANGLE ||
        servo2_angle < SERVO_MAP_MIN_ANGLE || servo2_angle > SERVO_MAP_MAX_ANGLE) {
      
      Serial.println("Angles are not correct. Not in the range of allowed values.");
      
    } else {
      
      // Mapping real angles to servs' range of angles
      servo1_angle = map(servo1_angle,
                         SERVO_MAP_MAX_ANGLE, SERVO_MAP_MIN_ANGLE,
                         SERVO1_MIN_ANGLE, SERVO1_MAX_ANGLE);
                         
      servo2_angle = map(servo2_angle,
                         SERVO_MAP_MAX_ANGLE, SERVO_MAP_MIN_ANGLE,
                         SERVO2_MIN_ANGLE, SERVO2_MAX_ANGLE);               
                         
      // Writing mapped angles to servos
      servo1.write(servo1_angle);
      servo2.write(servo2_angle);
      
      Serial.print("servo1: ");
      Serial.print(servo1_angle);
      
      Serial.print(" <> ");
      
      Serial.print("servo2: ");
      Serial.println(servo2_angle);
    }
  }
  
} 
