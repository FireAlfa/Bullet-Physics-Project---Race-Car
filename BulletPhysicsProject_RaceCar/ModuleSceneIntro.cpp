#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Bullet/include/BulletCollision/CollisionShapes/btCompoundShape.h"

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

	TrailerInfo trailerInfo;

	// Vehicle properties ---------------------------------------
	trailerInfo.vehicleInfo.chassis_size.Set(2.0f, 0.1f, 6.0f);
	trailerInfo.vehicleInfo.chassis_offset.Set(0, 0, 0);

	trailerInfo.vehicleInfo.mass = 100.0f;
	trailerInfo.vehicleInfo.suspensionStiffness = 15.88f;
	trailerInfo.vehicleInfo.suspensionCompression = 0.83f;
	trailerInfo.vehicleInfo.suspensionDamping = 0.88f;
	trailerInfo.vehicleInfo.maxSuspensionTravelCm = 1000.0f;
	trailerInfo.vehicleInfo.frictionSlip = 50.5;
	trailerInfo.vehicleInfo.maxSuspensionForce = 6000.0f;

	trailerInfo.vehicleInfo.box1 = { 2.0f, 0.1f, 6.0f };
	trailerInfo.vehicleInfo.box2 = { 0.1f, 1.9f, 6.0f };
	trailerInfo.vehicleInfo.box3 = { 0.1f, 1.9f, 6.0f };
	trailerInfo.vehicleInfo.box4 = { 2.0f, 1.9f, 0.1f };
	trailerInfo.vehicleInfo.box5 = { 2.0f, 1.9f, 0.1f };
	trailerInfo.vehicleInfo.origin = { 0.0f, 0.0f, 0.0f };

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;
	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	float half_width_remo = trailerInfo.vehicleInfo.chassis_size.x * 0.5f;
	float half_length_remo = trailerInfo.vehicleInfo.chassis_size.z * 0.5f;

	trailerInfo.vehicleInfo.num_wheels = 4;
	trailerInfo.vehicleInfo.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	trailerInfo.vehicleInfo.wheels[0].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
	trailerInfo.vehicleInfo.wheels[0].direction = direction;
	trailerInfo.vehicleInfo.wheels[0].axis = axis;
	trailerInfo.vehicleInfo.wheels[0].suspensionRestLength = suspensionRestLength;
	trailerInfo.vehicleInfo.wheels[0].radius = wheel_radius;
	trailerInfo.vehicleInfo.wheels[0].width = wheel_width;
	trailerInfo.vehicleInfo.wheels[0].front = false;
	trailerInfo.vehicleInfo.wheels[0].drive = false;
	trailerInfo.vehicleInfo.wheels[0].brake = false;
	trailerInfo.vehicleInfo.wheels[0].steering = false;

	// FRONT-RIGHT ------------------------
	trailerInfo.vehicleInfo.wheels[1].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius + 1.5);
	trailerInfo.vehicleInfo.wheels[1].direction = direction;
	trailerInfo.vehicleInfo.wheels[1].axis = axis;
	trailerInfo.vehicleInfo.wheels[1].suspensionRestLength = suspensionRestLength;
	trailerInfo.vehicleInfo.wheels[1].radius = wheel_radius;
	trailerInfo.vehicleInfo.wheels[1].width = wheel_width;
	trailerInfo.vehicleInfo.wheels[1].front = false;
	trailerInfo.vehicleInfo.wheels[1].drive = false;
	trailerInfo.vehicleInfo.wheels[1].brake = false;
	trailerInfo.vehicleInfo.wheels[1].steering = false;

	// REAR-LEFT ------------------------
	trailerInfo.vehicleInfo.wheels[2].connection.Set(half_width_remo - 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
	trailerInfo.vehicleInfo.wheels[2].direction = direction;
	trailerInfo.vehicleInfo.wheels[2].axis = axis;
	trailerInfo.vehicleInfo.wheels[2].suspensionRestLength = suspensionRestLength;
	trailerInfo.vehicleInfo.wheels[2].radius = wheel_radius;
	trailerInfo.vehicleInfo.wheels[2].width = wheel_width;
	trailerInfo.vehicleInfo.wheels[2].front = false;
	trailerInfo.vehicleInfo.wheels[2].drive = false;
	trailerInfo.vehicleInfo.wheels[2].brake = false;
	trailerInfo.vehicleInfo.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	trailerInfo.vehicleInfo.wheels[3].connection.Set(-half_width_remo + 0.3f * wheel_width, connection_height * 0.1 - 0.5, -half_length_remo + wheel_radius);
	trailerInfo.vehicleInfo.wheels[3].direction = direction;
	trailerInfo.vehicleInfo.wheels[3].axis = axis;
	trailerInfo.vehicleInfo.wheels[3].suspensionRestLength = suspensionRestLength;
	trailerInfo.vehicleInfo.wheels[3].radius = wheel_radius;
	trailerInfo.vehicleInfo.wheels[3].width = wheel_width;
	trailerInfo.vehicleInfo.wheels[3].front = false;
	trailerInfo.vehicleInfo.wheels[3].drive = false;
	trailerInfo.vehicleInfo.wheels[3].brake = false;
	trailerInfo.vehicleInfo.wheels[3].steering = false;

	remolque = App->physics->AddTrailer(trailerInfo);
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
	p.Render();


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->debug = !App->debug;
	}

	remolque->RenderTrailer();

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