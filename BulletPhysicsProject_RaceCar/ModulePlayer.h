#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 500.0f
#define TURN_DEGREES 25.0f * DEGTORAD
#define BRAKE_POWER 40.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	enum Gear
	{
		PARKING = 0,
		DRIVE,
		REVERSE
	};

public:

	Gear gearState = PARKING;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool engine = true;

	uint gearFx;
	uint engineFx;
};