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

	//
	// Create buildings
	//
	{
		CreateBuilding(80.0f, 5.0f, 80.0f, { 20, 5, 20 }, true);
		CreateBuilding(80.0f, 5.0f, 180.0f, { 20, 5, 20 }, true);
		CreateBuilding(80.0f, 5.0f, 280.0f, { 20, 5, 20 }, true);
		CreateBuilding(80.0f, 5.0f, 380.0f, { 20, 5, 20 }, true);

		//CreateBuilding(0.0f, 5.0f, -20.0f, { 20, 5, 20 }, true);

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
	}
	//
	// Create trailer
	//
	{
		VehicleInfo trailerInfo;

		// Trailer properties ---------------------------------------
		trailerInfo.chassis_size.Set(2.5f, 3, 9.f);
		trailerInfo.chassis_offset.Set(0, 0, 0);

		trailerInfo.mass = 100.0f;
		trailerInfo.suspensionStiffness = 15.88f;
		trailerInfo.suspensionCompression = 0.83f;
		trailerInfo.suspensionDamping = 0.88f;
		trailerInfo.maxSuspensionTravelCm = 1000.0f;
		trailerInfo.frictionSlip = 50.5;
		trailerInfo.maxSuspensionForce = 6000.0f;

		// Wheel properties ---------------------------------------
		float connection_height = 1.2f;
		float wheel_radius = 0.6f;
		float wheel_width = 0.5f;
		float suspensionRestLength = 1.2f;
		vec3 direction(0, -1, 0);
		vec3 axis(-1, 0, 0);

		float half_width_remo = trailerInfo.chassis_size.x * 0.5f;
		float half_length_remo = trailerInfo.chassis_size.z * 0.5f;

		trailerInfo.num_wheels = 4;
		trailerInfo.wheels = new Wheel[4];

		// FRONT-LEFT ------------------------
		trailerInfo.wheels[0].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
		trailerInfo.wheels[0].direction = direction;
		trailerInfo.wheels[0].axis = axis;
		trailerInfo.wheels[0].suspensionRestLength = suspensionRestLength;
		trailerInfo.wheels[0].radius = wheel_radius;
		trailerInfo.wheels[0].width = wheel_width;
		trailerInfo.wheels[0].front = false;
		trailerInfo.wheels[0].drive = false;
		trailerInfo.wheels[0].brake = false;
		trailerInfo.wheels[0].steering = false;

		// FRONT-RIGHT ------------------------
		trailerInfo.wheels[1].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
		trailerInfo.wheels[1].direction = direction;
		trailerInfo.wheels[1].axis = axis;
		trailerInfo.wheels[1].suspensionRestLength = suspensionRestLength;
		trailerInfo.wheels[1].radius = wheel_radius;
		trailerInfo.wheels[1].width = wheel_width;
		trailerInfo.wheels[1].front = false;
		trailerInfo.wheels[1].drive = false;
		trailerInfo.wheels[1].brake = false;
		trailerInfo.wheels[1].steering = false;

		// REAR-LEFT ------------------------
		trailerInfo.wheels[2].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
		trailerInfo.wheels[2].direction = direction;
		trailerInfo.wheels[2].axis = axis;
		trailerInfo.wheels[2].suspensionRestLength = suspensionRestLength;
		trailerInfo.wheels[2].radius = wheel_radius;
		trailerInfo.wheels[2].width = wheel_width;
		trailerInfo.wheels[2].front = false;
		trailerInfo.wheels[2].drive = false;
		trailerInfo.wheels[2].brake = false;
		trailerInfo.wheels[2].steering = false;

		// REAR-RIGHT ------------------------
		trailerInfo.wheels[3].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
		trailerInfo.wheels[3].direction = direction;
		trailerInfo.wheels[3].axis = axis;
		trailerInfo.wheels[3].suspensionRestLength = suspensionRestLength;
		trailerInfo.wheels[3].radius = wheel_radius;
		trailerInfo.wheels[3].width = wheel_width;
		trailerInfo.wheels[3].front = false;
		trailerInfo.wheels[3].drive = false;
		trailerInfo.wheels[3].brake = false;
		trailerInfo.wheels[3].steering = false;

		remolque = App->physics->AddVehicle(trailerInfo);
		remolque->SetPos(0.0f, 1.5f, 5.0f);
		remolque->collision_listeners.add(this);
		remolque->GetBody()->setUserPointer(remolque);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro Scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Draw the world floor
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.color = cFloor;
	p.Render();

	// Check debug mode
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->debug = !App->debug;
	}

	// Render all things
	p2List_item<Cube>* buildingItem = buildings.getFirst();
	while (buildingItem)
	{
		buildingItem->data.Render();
		buildingItem = buildingItem->next;
	}
	remolque->Render();
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

void ModuleSceneIntro::CreateBuilding(float x, float y, float z, vec3 size, bool axis)
{
	Cube cube;
	cube.color = cBuilding;
	cube.SetPos(x/2.0f, y/2.0f, z/2.0f);
	cube.size = size;
	cube.axis = axis;
	buildings.add(cube);

	App->physics->AddBuilding(cube, 10000);
}