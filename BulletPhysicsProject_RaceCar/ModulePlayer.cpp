#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//Load all engine sound effects
	gearFx = App->audio->LoadFx("Assets/FX/gear_shift.wav");
	engineFx = App->audio->LoadFx("Assets/FX/truck_engine.wav");
	engineOnFx = App->audio->LoadFx("Assets/FX/truck_on.wav");
	engineOffFx = App->audio->LoadFx("Assets/FX/truck_off.wav");
	engineAccelerationFx = App->audio->LoadFx("Assets/FX/truck_acceleration.wav");
	engineMaxSpeedFx = App->audio->LoadFx("Assets/FX/truck_max_speed.wav");
	engineStopFx = App->audio->LoadFx("Assets/FX/truck_stop.wav");

	//Play stop engine sound
	App->audio->PlayFx(engineOnFx);
	App->audio->PlayFx(engineFx, -1);

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5f, 3, 2.f);
	car.chassis_offset.Set(0, 1.5, 0);
	car.bridge_size.Set(2.5f, 0.7f, 3.f);
	car.bridge_offset.Set(0.0f, 0.37f, -2.5f);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 3);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 3);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0.0f, 1.5f, 0.0f);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	if (engine == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			switch (gearState)
			{
			case ModulePlayer::PARKING:
				break;
			case ModulePlayer::DRIVE:
				App->audio->PlayFx(engineAccelerationFx);
				if (vehicle->GetKmh() < 90)
				{
					acceleration = MAX_ACCELERATION;
				}
				break;
			case ModulePlayer::REVERSE:
				if (vehicle->GetKmh() > -20)
				{
					acceleration = -MAX_ACCELERATION;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			//App->audio->PlayFx(engineStopFx);
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			brake = BRAKE_POWER;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			App->audio->PlayFx(gearFx);
			switch (gearState)
			{
			case ModulePlayer::PARKING:
				gearState = DRIVE;
				break;
			case ModulePlayer::DRIVE:
				gearState = REVERSE;
				break;
			case ModulePlayer::REVERSE:
				gearState = PARKING;
				break;
			default:
				break;
			}
		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (engine != true)
		{
			App->audio->PlayFx(engineOnFx);
			App->audio->ResumeFx();
			engine = true;
		}
		else
		{
			App->audio->PauseFx();
			App->audio->PlayFx(engineOffFx);
			engine = false;
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h, Gear: %d, Engine: %d", vehicle->GetKmh(), gearState, engine);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}