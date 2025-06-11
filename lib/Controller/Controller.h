#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Vector2.h>
#include <Arm.h>
#include <Wheels.h>

enum ControllerMode
{
    CONTROLLER_MODE_WHEELS,
    CONTROLLER_MODE_ARM,
    CONTROLLER_MODE_NONE,
};

class Controller
{
private:
    /*
        Methods of them will be called to avoid conflicts
        due to using the same PWM pin for multiple motors when switching modes.
    */
    Arm &arm;
    Wheels &wheels;

public:
    Controller(Arm &_arm, Wheels &_wheels);

    /*
        Reads the Serial and updates one of the `main_vector` or the `alt_vector` accordingly.
        Returns *true* if one of the vectors has chagned.
    */
    bool read();
    // Changing modes will log to the Serial.
    void setMode(ControllerMode mode);
    ControllerMode getMode() const;

    // Read-only
    Vector2<int> getMainVector() const;
    // Read-only
    Vector2<int> getAltVector() const;

private:
    // This is to ignore if the current mode is same as the new mode to set.
    bool is_first_set_mode = true;
    ControllerMode mode = CONTROLLER_MODE_NONE;
    Vector2<int>
        main_vector = Vector2<int>(0, 0);
    Vector2<int> alt_vector = Vector2<int>(0, 0);

    void handleCommand(char command);
};

#endif