#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

TrailerInfo::~TrailerInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube bridge(info.bridge_size.x, info.bridge_size.y, info.bridge_size.z);
	bridge.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bridge.transform);
	q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 b_offset(info.bridge_offset.x, info.bridge_offset.y, info.bridge_offset.z);
	b_offset = b_offset.rotate(q.getAxis(), q.getAngle());
	bridge.transform.M[12] += b_offset.getX();
	bridge.transform.M[13] += b_offset.getY();
	bridge.transform.M[14] += b_offset.getZ();


	chassis.Render();
	bridge.Render();
}

void PhysVehicle3D::RenderTrailer()
{
	Cylinder wheel;

	wheel.color = Blue;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube box1(info.box1.getX()*2, info.box1.getY() *2, info.box1.getZ() *2);
	box1.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&box1.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	box1.transform.M[12] += offset.getX();
	box1.transform.M[13] += offset.getY();
	box1.transform.M[14] += offset.getZ();

	Cube box2(info.box2.getX() * 2, info.box2.getY() * 2, info.box2.getZ() * 2);
	box2.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&box2.transform);
	q = info.t2->getRotation();
	offset = offset.rotate(q.getAxis(), q.getAngle());
	box2.transform.M[12] += offset.getX();
	box2.transform.M[13] += offset.getY();
	box2.transform.M[14] += offset.getZ();

	Cube box3(info.box3.getX() * 2, info.box3.getY() * 2, info.box3.getZ() * 2);
	box3.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&box3.transform);
	q = vehicle->getChassisWorldTransform().getRotation();
	offset = offset.rotate(q.getAxis(), q.getAngle());
	box3.transform.M[12] += offset.getX();
	box3.transform.M[13] += offset.getY();
	box3.transform.M[14] += offset.getZ();


	box1.Render();
	box2.Render();
	box3.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}