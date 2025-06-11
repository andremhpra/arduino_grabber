#include <Arduino.h>
#include <Vector2.h>
#include <Arm.h>
#include <Wheels.h>
#include <Controller.h>

// Lower means smoother control.
#define LOOP_DELAY 10

// Wheels
Wheels wheels;

// Arm
Arm arm;

// Arm sensitivity
const int ARM_SENSITIVITY[4] = {
    1, // BASE
    1, // HORIZONTAL
    1, // VERTICAL
    3, // GRIPPER
};

// Controller
Controller controller(arm, wheels);

void initArm()
{
  // Neutralize the arm to prevent sudden movements
  // when switching from wheels mode to arm mode the first time.
  controller.setMode(CONTROLLER_MODE_ARM); // Attaches the arm.
  arm.neutral();

  controller.setMode(CONTROLLER_MODE_NONE); // Deattaches the arm.
}

void controlArm()
{
  Vector2<int> main_vector = controller.getMainVector();
  Vector2<int> alt_vector = controller.getAltVector();

  // BASE and HORIZONTAL.
  if (main_vector.x != 0)
    arm.increment(ARM_ID_BASE, -main_vector.x * ARM_SENSITIVITY[ARM_ID_BASE]);
  else if (main_vector.y != 0)
    arm.increment(ARM_ID_HORIZONTAL, main_vector.y * ARM_SENSITIVITY[ARM_ID_HORIZONTAL]);

  // GRIPPER and VERTICAL.
  if (alt_vector.x != 0)
    arm.increment(ARM_ID_GRIPPER, alt_vector.x * ARM_SENSITIVITY[ARM_ID_GRIPPER]);
  else if (alt_vector.y != 0)
    arm.increment(ARM_ID_VERTICAL, alt_vector.y * ARM_SENSITIVITY[ARM_ID_VERTICAL]);
}

void updateWheels()
{
  Vector2<int> main_vector = controller.getMainVector();
  Vector2<int> alt_vector = controller.getAltVector();

  // Stop any movement.
  if (main_vector == Vector2<int>(0, 0) && alt_vector == Vector2<int>(0, 0))
  {
    wheels.stop_movement();
  }
  // Strafing LEFT and RIGHT.
  else if (abs(main_vector.x) > 0)
  {
    wheels.set_movement(
        WHEELS_MODE_STRAFE,
        wheels.get_movement_direction(main_vector.x, WHEELS_DIRECTION_RIGHT, WHEELS_DIRECTION_LEFT));
  }
  // Moving FORWARD and BACKWARD.
  else if (abs(main_vector.y) > 0)
  {
    wheels.set_movement(
        WHEELS_MODE_LINEAR,
        wheels.get_movement_direction(main_vector.y, WHEELS_DIRECTION_FORWARD, WHEELS_DIRECTION_BACKWARD));
  }
  // Rotating LEFT and RIGHT.
  else if (abs(alt_vector.x) > 0)
  {
    wheels.set_movement(
        WHEELS_MODE_ROTATE,
        wheels.get_movement_direction(alt_vector.x, WHEELS_DIRECTION_RIGHT, WHEELS_DIRECTION_LEFT));
  }
}

void setup()
{
  Serial.begin(9600);
  initArm();

  controller.setMode(CONTROLLER_MODE_WHEELS);

  Serial.println("### READY ###");
}

void loop()
{
  bool vector_changed = controller.read();

  if (controller.getMode() == CONTROLLER_MODE_ARM)
    controlArm();
  else if (vector_changed and controller.getMode() == CONTROLLER_MODE_WHEELS)
    updateWheels();

  delay(LOOP_DELAY);
}
