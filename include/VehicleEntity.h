#pragma once
#include <Ogre.h>
#include "GameEntity.h"
#include "PhysicsBody.h"
#include "Reference.h"

using namespace Ogre;



class VehicleEntity : public FormFactor::PhysicsBody
{
public: 
	enum VehicleMode {
		NONE, GLIDER, TANK
	};

	VehicleEntity(SceneNode *node); 
	virtual ~VehicleEntity(); 

	virtual bool frameEvent(const FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	// Vehicle functions
	virtual bool transform(VehicleMode mode);
	virtual bool primaryAbility();
	virtual bool secondaryAbility();

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir);
	virtual void updateGraphicalPosition(const FormFactor::Vector &shiftAmt);
	virtual void clearPhysicsState();

protected:
	Entity *gliderVehicle;
	Entity *tankVehicle;
	Entity *curVehicle;

	float moveSpeed;
	bool onGround;

	float primaryCooldown;
	float secondaryCooldown;

	float curPrimaryCooldown;
	float curSecondaryCooldown;

	float curRoll;

	VehicleMode curMode;

	static const FormFactor::Vector VehicleEntity::thrust;


};