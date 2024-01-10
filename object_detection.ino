/**
 * Analog Controlled Object Detection System
 * 
 * @author Toby Griffin
 * @version 1.0
 */

// Included libraries
#include <LiquidCrystal.h>
#include <SR04.h>
#include <Servo.h>

// pin declarations to UNO for ultrasonic sensor module
#define TRIG_PIN 4
#define ECHO_PIN 5

// Create SR04 object (ultrasonic sensor module)
SR04 sr = SR04(ECHO_PIN, TRIG_PIN);
// holds distance at which an object is detected
long distance;
// the max distance at which an object can be detected
int max_dist_detected = 10;

// Create LCD object
//                RS E D4  D5  D6  D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Create servo object
Servo myserv;
// sets initial postion of servo to 90 degrees
int initial_position = 90;

// Joystick module pin number
const int x_pin = A0;
// Keeps track of position the joystick is in on the x-axis
int x_pos;

// Forward declaration of functions
void moveSensor();
void printDetection();

// set up for loop() function, only ran once
void setup() {
  // set x-axis connection for joystick as an input
  pinMode(x_pin, INPUT);
  // Attach servo object to pin
  myserv.attach(3);
  // initialize lcd display with 16 cols, 2 rows
  lcd.begin(16, 2);

}

// main code to run repeatedly
void loop() {
  // put your main code here, to run repeatedly:

  // Read the x-axis value of the joystick
  x_pos = analogRead(x_pin);

  // Calculates distance of an object is cm
  distance = sr.Distance();

  // Functions called to handle system logic
  moveSensor();
  printDetection(max_dist_detected);

}

/**
 * Function to handle the movement of servo motor
 */
void moveSensor() {
  // if joystick position is less than 300 (moved to the left)
  if (x_pos < 300){
        // check if initial pos is greater than 180
        if (initial_position > 180) { } // if so, do nothing (prevent going above upper limit)
        else{ 
            // if initial pos is not too high, increase it by 20
            initial_position = initial_position + 10; 
            // set servo pos to updated initial pos
            myserv.write ( initial_position ) ; 
            // delay for stability
            delay (75) ; 
        } 
    } 
    // if joystick pos is greater than 700 (moved to the right)
    if (x_pos > 700){
        // check if initial pos is less than 10
        if (initial_position < 10){  }  // if so, do nothing (prevent going below lower limit)
        else{
            // if initial pos is not too low, decrease it by 10
            initial_position = initial_position - 10;
            // set servo pos to updated initial pos
            myserv.write ( initial_position ) ;
            // delay for stability
            delay (75) ;
        }
    }
}

/**
 * Function to print the detection of an object to the LCD screen
 *
 * @param max_distance integer that represents the maximum range of detection in cm
 */
void printDetection(int max_distance) {
  // Only prints detection if object is within 10 cm
  if (distance <= max_distance) {
      // sets cursor t0 column 0, row 0
      lcd.setCursor(0, 0);
      lcd.print("Detected: ");
      // sets cursor to column 0, row 1
      lcd.setCursor(0, 1);
      lcd.print("Location: ");
      // set cursor to column 10, row 1
      lcd.setCursor(10, 1);
      lcd.print(initial_position);
    }
    else {
      // clears LCD screen if no object is detected
      lcd.clear();
    }
}
