#pragma once
#include <Ogre.h>
#include "GameEntity.h"
#include "PhysicsBody.h"
#include "Reference.h"
#include "LevelTileEntity.h"

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
			activated = false;
			cooldown = 0;
			duration = 0;
		}

		void init(float maxCD, float maxD) {
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
	static void VehicleEntity::setSingletonPtr(VehicleEntity *entity);

	VehicleEntity(SceneNode *node); 
	virtual ~VehicleEntity(); 

	virtual bool frameEvent(const FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	virtual Entity *getEntity();

	// Vehicle functions
	virtual void destroy();

	virtual bool activatePrimaryAbility() = 0;
	virtual void primaryAbility(Ogre::Real timeElapsed) = 0;

	virtual bool activateSecondaryAbility() = 0;
	virtual void secondaryAbility(Ogre::Real timeElapsed) = 0;

	VehicleMode getVehicleMode();
	bool isOnGround();

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const;

	// Override PhysicsBody functions
	virtual void handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir);
	virtual void updateGraphicalPosition(const FormFactor::Vector &shiftAmt);
	virtual void clearPhysicsState();

	// Other collision functions
	virtual void collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity) = 0;

protected:
	Entity *vehicleEntity;

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
	float rollDirection;

	VehicleMode curMode;
	SceneNode *cameraNode;
	SceneNode *vehicleNode;

	static const FormFactor::Vector VehicleEntity::thrust;
	static VehicleEntity *vehicle;
};