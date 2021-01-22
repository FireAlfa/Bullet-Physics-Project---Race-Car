#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List.h"

#define MAX_SNAKE 2
#define TILE_SIZE 10.0f

struct PhysBody3D;
struct PhysMotor3D;
struct PhysVehicle3D;
class btVector3;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateBuilding(float x, float y, float z, vec3 size, bool axis, Color color = cBuilding);

public:
	
	p2List<Cube> buildings;

	void DefineDeliveryPoints();

public:
	

	p2DynArray<btVector3*> deliveryPoints;

	Cylinder tree;
	Cylinder tree1;

	PhysVehicle3D* remolque;
};
