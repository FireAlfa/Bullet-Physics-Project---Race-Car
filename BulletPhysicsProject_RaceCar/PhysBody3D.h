#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class Module;


// =================================================
struct BuildingInfo
{
	vec3 pos;
	vec3 size;
	bool axis;
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Render();
	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	btRigidBody* GetBody();

private:

	btRigidBody* body = nullptr;

public:

	BuildingInfo bInfo;
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__