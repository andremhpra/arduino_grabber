#include <Wheels.h>
#include <Arduino.h>
#include <AFMotor.h>

// Constructor
Wheels::Wheels()
{
    front_left.setSpeed(WHEELS_SPEED);
    front_right.setSpeed(WHEELS_SPEED);
    rear_left.setSpeed(WHEELS_SPEED);
    rear_right.setSpeed(WHEELS_SPEED);
}

MovementDirection Wheels::get_movement_direction(int value, MovementDirection positive, MovementDirection negative)
{
    return (value > 0) ? positive : negative;
}

const int DURATION = 3 * 1000;
void Wheels::loop_movement()
{
    if (is_testing)
        return;

    is_testing = true;

    Serial.println("### TESTING EACH WHEELS ###");
    Serial.println("[DEBUG]: FRONT-LEFT");
    front_left.run(FORWARD);
    delay(DURATION);

    Serial.println("[DEBUG]: FRONT-RIGHT");
    front_left.run(RELEASE);
    front_right.run(FORWARD);
    delay(DURATION);

    Serial.println("[DEBUG]: REAR-LEFT");
    front_right.run(RELEASE);
    rear_left.run(FORWARD);
    delay(DURATION);

    Serial.println("[DEBUG]: REAR-RIGHT");
    rear_left.run(RELEASE);
    rear_right.run(FORWARD);
    delay(DURATION);

    rear_right.run(RELEASE);
    Serial.println("[DEBUG]: FINISHED");

    stop_movement(); // just incase.

    is_testing = false;
}

void Wheels::stop_movement()
{
    // Stop all motors
    front_left.run(RELEASE);
    front_right.run(RELEASE);
    rear_left.run(RELEASE);
    rear_right.run(RELEASE);
}

void Wheels::set_movement(MovementMode mode, MovementDirection direction)
{
    Serial.print("SET MOVEMENT TO: ");
    Serial.println(mode);
    stop_movement();

    // Set motor directions and speeds based on the movement mode and direction
    switch (mode)
    {
    case WHEELS_MODE_LINEAR:
        if (direction == WHEELS_DIRECTION_FORWARD || direction == WHEELS_DIRECTION_BACKWARD)
        {
            if (direction == WHEELS_DIRECTION_FORWARD)
            {
                front_left.run(FORWARD);
                front_right.run(FORWARD);
                rear_left.run(FORWARD);
                rear_right.run(FORWARD);
            }
            else
            {
                front_left.run(BACKWARD);
                front_right.run(BACKWARD);
                rear_left.run(BACKWARD);
                rear_right.run(BACKWARD);
            }
        }
        break;

    case WHEELS_MODE_ROTATE:
        if (direction == WHEELS_DIRECTION_LEFT || direction == WHEELS_DIRECTION_RIGHT)
        {
            // Set motor directions for strafing
            if (direction == WHEELS_DIRECTION_LEFT)
            {
                front_left.run(BACKWARD);
                front_right.run(FORWARD);
                rear_left.run(FORWARD);
                rear_right.run(BACKWARD);
            }
            else
            {
                front_left.run(FORWARD);
                front_right.run(BACKWARD);
                rear_left.run(BACKWARD);
                rear_right.run(FORWARD);
            }
        }
        break;

    case WHEELS_MODE_STRAFE:
        if (direction == WHEELS_DIRECTION_LEFT || direction == WHEELS_DIRECTION_RIGHT)
        {
            // Set motor directions for rotation
            if (direction == WHEELS_DIRECTION_LEFT)
            {
                front_left.run(BACKWARD);
                front_right.run(FORWARD);
                rear_left.run(BACKWARD);
                rear_right.run(FORWARD);
            }
            else
            {
                front_left.run(FORWARD);
                front_right.run(BACKWARD);
                rear_left.run(FORWARD);
                rear_right.run(BACKWARD);
            }
        }
        break;

    default:
        // Stop all motors
        Serial.print("INVALID MOVEMENT MODE: \"");
        Serial.print(mode);
        Serial.println("\"");
        stop_movement();
    }
}