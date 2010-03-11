#pragma once
#include <Ogre.h>
#include "GameEntity.h"
#include "PhysicsBody.h"
#include "Reference.h"

using namespace Ogre;



class VehicleEntity : public FormFactor::PhysicsBody
{
public: 
	enum VehicleMode { NONE, GLIDER, TANK };

	// Holds state information about an ability
	struct VehicleAbility {
		float maxCooldown;
		float cooldown;
		float maxDuration;
		float duration;
		bool activated;

		VehicleAbility() {
		}

		void initAbility(float maxCD, float maxD) {
			maxCooldown = maxCD;
			maxDuration = maxD;
			activated = false;
			cooldown = 0;
			duration = 0;
		}

		bool activate() {
			if (cooldown > 0) return false;
			activated = true;
			cooldown = maxCooldown;
			duration = maxDuration;
			return true;
		}
	};

	static VehicleEntity* VehicleEntity::getSingletonPtr(void);

	VehicleEntity(SceneNode *node); 
	virtual ~VehicleEntity(); 

	virtual bool frameEvent(const FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	// Vehicle functions
	virtual void destroy();
	virtual bool transform(VehicleMode mode);

	virtual bool activatePrimaryAbility();
	virtual void primaryAbility(Ogre::Real timeElapsed);

	virtual bool activateSecondaryAbility();
	virtual void secondaryAbility(Ogre::Real timeElapsed);

	virtual VehicleMode getVehicleMode();
	virtual bool isOnGround();

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir);
	virtual void updateGraphicalPosition(const FormFactor::Vector &amountShifted);
	virtual void clearPhysicsState();

protected:
	Entity *gliderVehicle;
	Entity *tankVehicle;
	Entity *curVehicle;

	float moveSpeed;

	float xVelocity;
	float xAcceleration;

	float yVelocity;
	float yAcceleration;

	float zVelocity;
	float zAcceleration;

	bool onGround;

	VehicleAbility primary;
	VehicleAbility secondary;

	float curRoll;

	VehicleMode curMode;
	SceneNode *cameraNode;
	SceneNode *vehicleNode;

	static const FormFactor::Vector VehicleEntity::thrust;
	static VehicleEntity *vehicle;
};