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

	DefineDeliveryPoints();

	//
	// Create buildings
	//
	{
		// Central grid
		float posZ = 0;
		float posX = 0;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				posZ = 4 * TILE_SIZE * i;
				posX = 4 * TILE_SIZE * j;
				CreateBuilding(2 * TILE_SIZE - (2 * TILE_SIZE / 2) - posX, 6 * TILE_SIZE, -2 * TILE_SIZE + (2 * TILE_SIZE / 2) + posZ, { 2 * TILE_SIZE, 6 * TILE_SIZE, 2 * TILE_SIZE }, true);
			}
		}

		// Left column
		for (int i = 0; i < 4; ++i)
		{
			posZ = 4 * TILE_SIZE * i;	
			CreateBuilding(14 * TILE_SIZE - (3 * TILE_SIZE / 2), 3 * TILE_SIZE, -2 * TILE_SIZE + (2 * TILE_SIZE / 2) + posZ, { 3 * TILE_SIZE, 3 * TILE_SIZE, 2 * TILE_SIZE }, true);
		}

		// City hall
		CreateBuilding(7 * TILE_SIZE - (TILE_SIZE / 2), TILE_SIZE , 9 * TILE_SIZE + (TILE_SIZE / 2), { TILE_SIZE, TILE_SIZE, TILE_SIZE }, true);
		CreateBuilding(9 * TILE_SIZE - (5 * TILE_SIZE / 2), 2 * TILE_SIZE, 10 * TILE_SIZE + (2 * TILE_SIZE / 2), { 5 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE }, true);
		CreateBuilding(8 * TILE_SIZE - (3 * TILE_SIZE / 2), 1.5f * TILE_SIZE, 12 * TILE_SIZE + (TILE_SIZE / 2), { 3 * TILE_SIZE, 1.5f * TILE_SIZE, TILE_SIZE }, true);
		
		// City hall front building
		CreateBuilding(9 * TILE_SIZE - (5 * TILE_SIZE / 2), 2.5f * TILE_SIZE, -2 * TILE_SIZE + (2 * TILE_SIZE / 2), { 5 * TILE_SIZE, 2.5 * TILE_SIZE , 2 * TILE_SIZE }, true);
		
		// Right column
		for (int i = 0; i < 8; ++i)
		{
			posZ = 3 * TILE_SIZE * i;
			CreateBuilding(-16 * TILE_SIZE - TILE_SIZE, 2 * TILE_SIZE / 2.0f, -7 * TILE_SIZE + (2 * TILE_SIZE / 2) + posZ, {TILE_SIZE, 2 * TILE_SIZE / 2, 2 * TILE_SIZE }, true);
		}

		// Centre back row
		CreateBuilding(14 * TILE_SIZE - (3 * TILE_SIZE / 2), 4 * TILE_SIZE, -8 * TILE_SIZE + (4 * TILE_SIZE / 2), { 3 * TILE_SIZE, 4 * TILE_SIZE, 4 * TILE_SIZE }, true);
		CreateBuilding(9 * TILE_SIZE - (5 * TILE_SIZE / 2), 1.5f * TILE_SIZE, -8 * TILE_SIZE + (4 * TILE_SIZE / 2), { 5 * TILE_SIZE, 1.5f * TILE_SIZE, 4 * TILE_SIZE }, true);
		for (int i = 0; i < 2; ++i)
		{
			posX = 4 * TILE_SIZE * i;
			CreateBuilding(2 * TILE_SIZE - (2 * TILE_SIZE / 2) - posX, 4 * TILE_SIZE, -8 * TILE_SIZE + (4 * TILE_SIZE / 2), { 2 * TILE_SIZE, 4 * TILE_SIZE, 4 * TILE_SIZE }, true);
		}

		// Back row
		CreateBuilding(14 * TILE_SIZE - (4 * TILE_SIZE / 2), 3.5f * TILE_SIZE, -13 * TILE_SIZE + (3 * TILE_SIZE / 2), { 4 * TILE_SIZE, 3.5f * TILE_SIZE, 3 * TILE_SIZE }, true);
		for (int i = 0; i < 3; ++i)
		{
			posX = 5 * TILE_SIZE * i;
			CreateBuilding(9 * TILE_SIZE - (3 * TILE_SIZE / 2) - posX, 2.8f * TILE_SIZE, -13 * TILE_SIZE + (3 * TILE_SIZE / 2), { 3 * TILE_SIZE, 2.8f * TILE_SIZE, 3 * TILE_SIZE }, true);
		}
		CreateBuilding(-6 * TILE_SIZE - (3 * TILE_SIZE / 2), 4 * TILE_SIZE, -13 * TILE_SIZE + (4 * TILE_SIZE / 2), { 3 * TILE_SIZE, 4 * TILE_SIZE, 4 * TILE_SIZE }, true);
		CreateBuilding(-10 * TILE_SIZE - (2 * TILE_SIZE / 2), 2.5f * TILE_SIZE, -13 * TILE_SIZE + (3 * TILE_SIZE / 2), { 2 * TILE_SIZE, 2.5f * TILE_SIZE, 3 * TILE_SIZE }, true);
	}


	//
	// Create parks
	//
	{
		CreateBuilding(9 * TILE_SIZE - (5 * TILE_SIZE / 2), 0.2f, TILE_SIZE + (7 * TILE_SIZE / 2), { 5 * TILE_SIZE, 0.2f, 7 * TILE_SIZE }, true, Green);
		CreateBuilding(-6 * TILE_SIZE - (7 * TILE_SIZE / 2), 0.2f, -7 * TILE_SIZE + (4 * TILE_SIZE / 2), { 7 * TILE_SIZE, 0.2f, 4 * TILE_SIZE }, true, Green);

		/*tree.color = cTree;
		tree.SetPos(15, 0.25, 50);
		tree.SetRotation(90, vec3(0, 0, 1));
		tree.radius = 0.5;
		tree.height = 10;
		tree.axis = true;
		App->physics->AddBody(tree, 10000);*/

		/*tree1.color = cTree;
		tree1.SetPos(15, 0.25, 100);
		tree1.SetRotation(90, vec3(0, 0, 1));
		tree1.radius = 0.5;
		tree1.height = 10;
		tree1.axis = true;
		App->physics->AddBody(tree, 10000);*/
	}


	//
	// Create roundabouts
	//
	{
		CreateBuilding(0 * TILE_SIZE - (2 * TILE_SIZE / 2), 0.5f, 13 * TILE_SIZE + (2 * TILE_SIZE / 2), { 2 * TILE_SIZE, 0.5f, 2 * TILE_SIZE }, true, cRoundabout);
		CreateBuilding(-13 * TILE_SIZE - (3 * TILE_SIZE / 2), 0.5f, -13 * TILE_SIZE + (3 * TILE_SIZE / 2), { 3 * TILE_SIZE, 0.5f, 3 * TILE_SIZE }, true, cRoundabout);
		CreateBuilding(-14 * TILE_SIZE - (TILE_SIZE / 2), 3.0f, -12 * TILE_SIZE + (TILE_SIZE / 2), { TILE_SIZE / 2, 2.0f, TILE_SIZE / 2 }, true, cStatue);
	}

	return ret;
}

void ModuleSceneIntro::DefineDeliveryPoints()
{
	//Every city point of deliver and collect defined
	deliveryPoints.PushBack(btVector3(-7 * TILE_SIZE, 0, 4 * TILE_SIZE));
	deliveryPoints.PushBack(btVector3(-4*TILE_SIZE, 0, TILE_SIZE*11));
	deliveryPoints.PushBack(btVector3(4*TILE_SIZE, 0, 5*TILE_SIZE));

}

void ModuleSceneIntro::CreateDeliverySensor(float x, float y, float z)
{
	Cube cubeSensor;
	cubeSensor.color = Blue;
	cubeSensor.SetPos( x - 3.5, y + 2.5, z + 3.5);
	cubeSensor.size = vec3(7,5,7);
	cubeSensor.axis = true;

	deliverySensor = App->physics->AddBuilding(cubeSensor, 10000);
	deliverySensor->collision_listeners.add(this);
	deliverySensor->GetBody()->setUserPointer(deliverySensor);
}

void ModuleSceneIntro::CreateTrailer(float x, float y, float z)
{
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
		remolque->SetPos(x, y, z);
		remolque->collision_listeners.add(this);
		remolque->GetBody()->setUserPointer(remolque);
	}
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
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		App->player->map = !App->player->map;
	}

	// Render all things
	p2List_item<Cube>* buildingItem = buildings.getFirst();
	while (buildingItem)
	{
		buildingItem->data.Render();
		buildingItem = buildingItem->next;
	}
	if (remolque != nullptr)
	{
		remolque->Render();
	}
	if (deliverySensor != nullptr)
	{
		deliverySensor->cube.Render();
	}
	/*tree.Render();
	tree1.Render();*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if ((body1 == remolque && body2 == App->player->vehicle) || (body2 == remolque && body1 == App->player->vehicle) && isJoint == false)
	{
		App->player->GenerateDeliveryPoint();
		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(btScalar(App->player->vehicle->info.bridge_offset.x), btScalar(App->player->vehicle->info.bridge_offset.y + 1), btScalar(App->player->vehicle->info.bridge_offset.z - 1.5)));
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(btScalar(remolque->info.chassis_offset.x), btScalar(remolque->info.chassis_offset.y), btScalar(remolque->info.chassis_offset.z + 2)));

		cs = new btGeneric6DofConstraint(*App->player->vehicle->GetBody(), *remolque->GetBody(), frameInA, frameInB, false);

		cs->setDbgDrawSize(2.0f);
		App->physics->world->addConstraint(cs);
		isJoint = true;
	}
	if ((body1 == deliverySensor && body2 == App->player->vehicle) || (body2 == deliverySensor && body1 == App->player->vehicle))
	{
		if (cs != nullptr)
		{
			App->physics->world->removeConstraint(cs);
			cs = nullptr;
		}
		if (remolque != nullptr)
		{
			App->physics->vehicles.del(App->physics->vehicles.findNode(remolque));
			App->physics->world->removeVehicle(remolque->vehicle);
			App->physics->world->removeRigidBody(remolque->GetBody());
			remolque = nullptr;
			App->player->GenerateCollectPoint();
		}
	}
}

void ModuleSceneIntro::CreateBuilding(float x, float y, float z, vec3 size, bool axis, Color color)
{
	Cube cube;
	cube.color = color;
	cube.SetPos(x, y/2.0f, z);
	cube.size = size;
	cube.axis = axis;
	buildings.add(cube);

	App->physics->AddBuilding(cube, 10000);
}