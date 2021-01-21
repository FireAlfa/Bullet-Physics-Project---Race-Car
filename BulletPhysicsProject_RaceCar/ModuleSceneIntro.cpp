#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	building.color = cBuilding;
	building.SetPos(40, 30, 40);
	building.size = { 20,60,20 };
	building.axis = true;
	App->physics->AddBody(building, 10000);

	building1.color = cBuilding;
	building1.SetPos(40, 30, 90);
	building1.size = { 20,60,20 };
	building1.axis = true;
	App->physics->AddBody(building1, 10000);

	building2.color = cBuilding;
	building2.SetPos(40, 30, 140);
	building2.size = { 20,60,20 };
	building2.axis = true;
	App->physics->AddBody(building2, 10000);

	building3.color = cBuilding;
	building3.SetPos(40, 30, 190);
	building3.size = { 20,60,20 };
	building3.axis = true;
	App->physics->AddBody(building3, 10000);

	tree.color = cTree;
	tree.SetPos(15, 0.25, 50);
	tree.SetRotation(90, vec3(0, 0, 1));
	tree.radius = 0.5;
	tree.height = 10;
	tree.axis = true;
	App->physics->AddBody(tree, 10000);

	tree1.color = cTree;
	tree1.SetPos(15, 0.25, 100);
	tree1.SetRotation(90, vec3(0, 0, 1));
	tree1.radius = 0.5;
	tree1.height = 10;
	tree1.axis = true;
	App->physics->AddBody(tree, 10000);

	VehicleInfo remolqueInfo;

	// Remolque properties ---------------------------------------
	remolqueInfo.chassis_size.Set(2.5f, 3, 9.f);
	remolqueInfo.chassis_offset.Set(0, 0, 0);

	remolqueInfo.mass = 100.0f;
	remolqueInfo.suspensionStiffness = 15.88f;
	remolqueInfo.suspensionCompression = 0.83f;
	remolqueInfo.suspensionDamping = 0.88f;
	remolqueInfo.maxSuspensionTravelCm = 1000.0f;
	remolqueInfo.frictionSlip = 50.5;
	remolqueInfo.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;
	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	float half_width_remo = remolqueInfo.chassis_size.x * 0.5f;
	float half_length_remo = remolqueInfo.chassis_size.z * 0.5f;

	remolqueInfo.num_wheels = 4;
	remolqueInfo.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	remolqueInfo.wheels[0].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
	remolqueInfo.wheels[0].direction = direction;
	remolqueInfo.wheels[0].axis = axis;
	remolqueInfo.wheels[0].suspensionRestLength = suspensionRestLength;
	remolqueInfo.wheels[0].radius = wheel_radius;
	remolqueInfo.wheels[0].width = wheel_width;
	remolqueInfo.wheels[0].front = false;
	remolqueInfo.wheels[0].drive = false;
	remolqueInfo.wheels[0].brake = false;
	remolqueInfo.wheels[0].steering = false;

	// FRONT-RIGHT ------------------------
	remolqueInfo.wheels[1].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
	remolqueInfo.wheels[1].direction = direction;
	remolqueInfo.wheels[1].axis = axis;
	remolqueInfo.wheels[1].suspensionRestLength = suspensionRestLength;
	remolqueInfo.wheels[1].radius = wheel_radius;
	remolqueInfo.wheels[1].width = wheel_width;
	remolqueInfo.wheels[1].front = false;
	remolqueInfo.wheels[1].drive = false;
	remolqueInfo.wheels[1].brake = false;
	remolqueInfo.wheels[1].steering = false;

	// REAR-LEFT ------------------------
	remolqueInfo.wheels[2].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
	remolqueInfo.wheels[2].direction = direction;
	remolqueInfo.wheels[2].axis = axis;
	remolqueInfo.wheels[2].suspensionRestLength = suspensionRestLength;
	remolqueInfo.wheels[2].radius = wheel_radius;
	remolqueInfo.wheels[2].width = wheel_width;
	remolqueInfo.wheels[2].front = false;
	remolqueInfo.wheels[2].drive = false;
	remolqueInfo.wheels[2].brake = false;
	remolqueInfo.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	remolqueInfo.wheels[3].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
	remolqueInfo.wheels[3].direction = direction;
	remolqueInfo.wheels[3].axis = axis;
	remolqueInfo.wheels[3].suspensionRestLength = suspensionRestLength;
	remolqueInfo.wheels[3].radius = wheel_radius;
	remolqueInfo.wheels[3].width = wheel_width;
	remolqueInfo.wheels[3].front = false;
	remolqueInfo.wheels[3].drive = false;
	remolqueInfo.wheels[3].brake = false;
	remolqueInfo.wheels[3].steering = false;

	remolque = App->physics->AddVehicle(remolqueInfo);
	remolque->SetPos(0.0f, 1.5f, 5.0f);
	remolque->collision_listeners.add(this);
	remolque->GetBody()->setUserPointer(remolque);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.color = cFloor;
	p.Render();


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->debug = !App->debug;
	}

	remolque->Render();
	building.Render();
	building1.Render();
	building2.Render();
	building3.Render();
	tree.Render();
	tree1.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == remolque || body2 == remolque)
	{
		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(btScalar(App->player->vehicle->info.bridge_offset.x), btScalar(App->player->vehicle->info.bridge_offset.y + 1), btScalar(App->player->vehicle->info.bridge_offset.z - 1.5)));
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(btScalar(remolque->info.chassis_offset.x), btScalar(remolque->info.chassis_offset.y), btScalar(remolque->info.chassis_offset.z + 2)));

		btGeneric6DofConstraint* cs = new btGeneric6DofConstraint(*App->player->vehicle->GetBody(), *remolque->GetBody(), frameInA, frameInB, false);

		cs->setDbgDrawSize(2.0f);
		App->physics->world->addConstraint(cs);
	}
}