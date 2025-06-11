#ifndef ARM_H
#define ARM_H

#include <Servo.h>
#include <Arduino.h>

#define NEUTRAL_SPEED 40
#define INCREMENT_SPEED 80

// Used to refer which arm joint to move.
enum ArmId
{
	ARM_ID_BASE,
	ARM_ID_HORIZONTAL,
	ARM_ID_VERTICAL,
	ARM_ID_GRIPPER
};

// Joint's servo range.
struct ServoRange
{
	int start;
	int end;
};

// Constants for each of the arm's joint.
struct ArmData
{
	int pin;
	ServoRange range;
	int neutral;
};

const ArmData ARM_DATAS[4] = {
	{2, ServoRange{0, 180}, 120},	// BASE
	{10, ServoRange{60, 180}, 120}, // HORIZONTAL
	{9, ServoRange{70, 180}, 80},	// VERTICAL
	{4, ServoRange{0, 100}, 0},		// GRIPPER
};

class Arm
{
private:
	// Indices are based off `ArmId`.
	Servo servos[4];
	int current_angles[4];

	String id_to_name(int id);

public:
	Arm();

	void neutral();
	void attach();
	void detach();

	/*
		Make sure `neutral` has been called first.

		Increments the joint by `by`.
	*/
	void increment(ArmId id, int by);
};

#endif