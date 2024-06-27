#include "main.h"
using namespace pros;
using namespace std; // See if can remove the pros :: LCD thing
#define VISION_PORT 13 
#define EXAMPLE_SIG 1 

void on_center_button();

// Initialization function to set up the robot's components.
void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "Hello PROS User!");
    pros::lcd::register_btn1_cb(on_center_button);
}

// Autonomous function for autonomous mode.
void autonomous() {}

// Operator control function for manual control via the controller.
void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
	
    // setting color signatures 
	pros::Vision vision_sensor (VISION_PORT);
  	pros::vision_signature_s_t RED_SIG =
    pros::Vision::signature_from_utility(EXAMPLE_SIG, 8973, 11143, 10058, -2119, -1053, -1586, 5.4, 0);
	vision_sensor.set_signature(EXAMPLE_SIG, &RED_SIG);
    vision_sensor.set_zero_point(pros::E_VISION_ZERO_CENTER);

    // Define the motors. Replace 'x' with your actual motor ports.
    pros::Motor frontLeft(12);   // Front left motor
    pros::Motor frontRight(2);  // Front right motor
    pros::Motor backLeft(11);    // Back left motor
    pros::Motor backRight(1);   // Back right motor

    // Correct motor directions depending on your physical setup
    frontRight.set_reversed(true);
    backRight.set_reversed(true);

    while (true) {
        // Read controller joysticks
        int vertical = master.get_analog(ANALOG_LEFT_Y);   // Forward and backward
        int horizontal = master.get_analog(ANALOG_LEFT_X); // Strafe left and right
        int rotation = master.get_analog(ANALOG_RIGHT_X);  // Rotation

        // Calculate motor values
        int frontLeftValue  = vertical + horizontal + rotation;
        int backLeftValue   = vertical - horizontal + rotation;
        int frontRightValue = vertical - horizontal - rotation;
        int backRightValue  = vertical + horizontal - rotation;

        // Set motor speeds
        frontLeft.move(frontLeftValue);
        backLeft.move(backLeftValue);
        frontRight.move(frontRightValue);
        backRight.move(backRightValue);

        // retrieving objects by signature 
		vision_object_s_t rtn = vision_sensor.get_by_sig(0, EXAMPLE_SIG);
    	// Gets the largest object of the EXAMPLE_SIG signature
    	std::cout << "sig: " << rtn.signature << std::endl;
   	    // Prints "sig: 1"
    	delay(2);

        // Prevent CPU hogging
        pros::delay(20);
    }
}

void disabled() {}
void competition_initialize() {}

// Callback for the center button on the LCD.
void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}