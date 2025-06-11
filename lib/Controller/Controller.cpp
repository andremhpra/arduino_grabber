/*
    Handles controlling the pin connections of the Arm and the Wheels.
*/

#include <Controller.h>
#include <Arduino.h>
#include <Vector2.h>

Controller::Controller(Arm &_arm, Wheels &_wheels) : arm(_arm), wheels(_wheels)
{
}

bool Controller::read()
{
    if (!Serial.available())
        // No controller update.
        return false;

    char command = Serial.read();

    // Debugging to test the wheels often.
    if (command == '`' and mode == CONTROLLER_MODE_WHEELS)
    {
        ControllerMode prev_mode = mode;

        setMode(CONTROLLER_MODE_WHEELS);
        wheels.loop_movement();

        // Set the mode back if it was not wheels.
        if (prev_mode != CONTROLLER_MODE_WHEELS)
            setMode(prev_mode);
    }

    Vector2<int> previous_main = main_vector;
    Vector2<int> previous_alt = alt_vector;

    handleCommand(command);
    Serial.println(command);

    if (main_vector != previous_main || alt_vector != previous_alt)
    {
        // One of the vectors have changed.
        return true;
    }

    // No change in vectors.
    return false;
}

void Controller::setMode(ControllerMode new_mode)
{
    if (mode == new_mode)
        return;

    mode = new_mode;

    // Debugging.
    switch (mode)
    {
    case CONTROLLER_MODE_ARM:
        Serial.println("--- ARM MODE");
        arm.attach();
        wheels.stop_movement();

        break;
    case CONTROLLER_MODE_WHEELS:
        Serial.println("--- WHEELS MODE");
        arm.detach();
        wheels.stop_movement(); // Prevents unwanted movement.

        break;
    case CONTROLLER_MODE_NONE:
        // This mode is only for initializing the mode to none, so it can be set to anything when initializing.
        Serial.println("---- NONE MODE");
        arm.detach();
        wheels.stop_movement();

        break;
    default:
        Serial.println("--- UNKNOWN MODE");
        break;
    }
}

ControllerMode Controller::getMode() const
{
    return mode;
}

Vector2<int> Controller::getMainVector() const
{
    return main_vector;
}

Vector2<int> Controller::getAltVector() const
{
    return alt_vector;
}

void Controller::handleCommand(char command)
{
    switch (command)
    {
    case 'W': // FORWARD
        main_vector = Vector2<int>(0, 1);
        break;
    case 'S': // BACKWARD
        main_vector = Vector2<int>(0, -1);
        break;
    case 'A': // LEFT
        main_vector = Vector2<int>(-1, 0);
        break;
    // RIGHT, 'F' is used rather than 'D' because when connecting to the bluetooth, the serial prints out "CONNECTED";
    // where the 'D' is interpreted as an instruction to move, which shouldn't.
    case 'F':
        main_vector = Vector2<int>(1, 0);
        break;
    case 'I': // ALT_FORWARD
        alt_vector = Vector2<int>(0, 1);
        break;
    case 'K': // ALT_BACKWARD
        alt_vector = Vector2<int>(0, -1);
        break;
    case 'J': // ALT_LEFT
        alt_vector = Vector2<int>(-1, 0);
        break;
    case ';': // ALT_RIGHT
        alt_vector = Vector2<int>(1, 0);
        break;
    case 'Z': // ARM_MODE
        setMode(CONTROLLER_MODE_ARM);
        break;
    case 'X': // WHEEL_MODE
        setMode(CONTROLLER_MODE_WHEELS);
        break;
    case '0': // MAIN_END
        main_vector = Vector2<int>(0, 0);
        break;
    case '1': // ALT_END
        alt_vector = Vector2<int>(0, 0);
        break;
    case '2': // CONTROLLER_END
        break;
    default:
        Serial.print("Invalid command \"");
        Serial.print(command);
        Serial.println("\"");
        break;
    }
}