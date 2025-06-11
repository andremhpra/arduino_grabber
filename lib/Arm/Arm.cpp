#include <Arm.h>
#include <Arduino.h>

// Constructor
Arm::Arm()
{
    // for (int i = 0; i < 4; i++)
    // {
    //     current_angles[i] = servos[i].read();
    //     servos[i].write(current_angles[i]);

    //     Serial.println(id_to_name(i) + " INIT ANGLE: " + String(current_angles[i]));
    // }
}

void Arm::neutral()
{
    for (int i = 0; i < 4; i++)
    {
        // Initialize the current angle as the neutral.
        current_angles[i] = ARM_DATAS[i].neutral;
        servos[i].write(current_angles[i]);
    }
    delay(100);
}

// Attach servos to their respective pins
void Arm::attach()
{
    Serial.println("ARM ATTACHED");
    for (int i = 0; i < 4; i++)
    {
        if (servos[i].attached())
            continue;

        servos[i].attach(ARM_DATAS[i].pin);
    }
}

/*
    Free the pwm pins from the arm's servos.
    Should be used for switching pwm pin ownership.
*/
void Arm::detach()
{
    Serial.println("ARM DETACHED");
    for (int i = 0; i < 4; i++)
    {
        // Keep the gripper's power to hold item when moving.
        if (i == ARM_ID_GRIPPER)
            continue;

        servos[i].detach();
    }
}

// Increment the angle of a specific servo
void Arm::increment(ArmId id, int by)
{
    if (by == 0)
    {
        // No change, ignore.
        return;
    }

    int prev_angle = current_angles[id];

    // Make sure the new angle doesn't go above/below the range.
    current_angles[id] = constrain(current_angles[id] + by, ARM_DATAS[id].range.start, ARM_DATAS[id].range.end);

    if (prev_angle == current_angles[id])
    {
        // No change, ignore.
        return;
    }

    // Go to the new angle.
    servos[id].write(current_angles[id]);
    Serial.println(id_to_name(id) + " NEW ANGLE: " + String(current_angles[id]));
}

/*
    Convert ArmId to a string name

    For debugging purpose, so when
*/
String Arm::id_to_name(int id)
{
    if (id == 0)
    {
        return "BASE";
    }
    else if (id == 1)
    {
        return "HORIZONTAL";
    }
    else if (id == 2)
    {
        return "VERTICAL";
    }
    else if (id == 3)
    {
        return "GRIPPER";
    }

    return "[INVALID ID]";
}