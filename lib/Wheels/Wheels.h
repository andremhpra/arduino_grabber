/*
    L3
*/

#ifndef WHEELS_H
#define WHEELS_H

#include <AFMotor.h>

// Ranging from 0 to 255.
#define WHEELS_SPEED 200

enum MovementMode
{
    WHEELS_MODE_LINEAR, // Move in a straight line.
    WHEELS_MODE_STRAFE, // Move sideways without rotating.
    WHEELS_MODE_ROTATE  // Do a rotation without any linear motion.
};

/*
    For linear motions, `LEFT` and `RIGHT` will not work.
    For strafing or any rotation, `FORWARD` and `BACKWARD` will not work.
*/
enum MovementDirection
{
    WHEELS_DIRECTION_FORWARD,
    WHEELS_DIRECTION_BACKWARD,
    WHEELS_DIRECTION_LEFT,
    WHEELS_DIRECTION_RIGHT,
};

class Wheels
{
private:
    AF_DCMotor front_right = AF_DCMotor(1);
    AF_DCMotor front_left = AF_DCMotor(2);
    AF_DCMotor rear_right = AF_DCMotor(3);
    AF_DCMotor rear_left = AF_DCMotor(4);

    bool is_testing = false;

public:
    Wheels();

    /*
        Helper function for controllers.
    */
    MovementDirection get_movement_direction(int value, MovementDirection positive, MovementDirection negative);

    /*
        Sets the movement type and direction of the wheels.
        Will be ignored if the mode or the
    */
    void set_movement(MovementMode mode, MovementDirection direction);
    void stop_movement();
    /*
        Tests each motor one by one.
    */
    void loop_movement();
};

#endif