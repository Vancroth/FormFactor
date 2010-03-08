#pragma once
#include <ode/ode.h>
#include <Ogre.h>
#include "GameEntity.h"
#include "PhysicsBody.h"
#include "Reference.h"

using namespace Ogre;

class VehicleEntity : public FormFactor::PhysicsBody
{
public: 
	VehicleEntity(SceneNode *node); 
	virtual ~VehicleEntity(); 

	virtual bool frameEvent(const FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit);
	virtual void updateGraphicalPosition(const FormFactor::Vector &amountShifted);
	virtual void setMass(float m);

protected:
	Entity *vehicle;
	float moveSpeed;
};