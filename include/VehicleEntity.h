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

		bool updateState(float timeElapsed) {
			bool wasActivated = activated;
			cooldown = (cooldown - timeElapsed > 0 ? cooldown - timeElapsed : 0);
			duration = (duration - timeElapsed > 0 ? cooldown - timeElapsed : 0);
			if (wasActivated && duration == 0.0f) {
				activated = false;
				return true;
			}
			return false;
		}

		bool activate() {
			if (cooldown > 0.0f) return false;
			activated = true;
			cooldown = maxCooldown;
			duration = maxDuration;
			return true;
		}
	};

	static VehicleEntity* VehicleEntity::getSingletonPtr(void);
	static void VehicleEntity::setSingletonPtr(VehicleEntity *entity);

	VehicleEntity(SceneNode *node, String &name, String &mesh); 
	virtual ~VehicleEntity(); 

	virtual bool frameEvent(const FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	Entity *getEntity();

	// Vehicle functions
	virtual void activatePrimary() = 0;
	virtual void deactivatePrimary() = 0;

	virtual void activateSecondary() = 0;
	virtual void deactivateSecondary() = 0;

	VehicleMode getVehicleMode();
	bool isOnGround();

	// Override Primitve functions
	virtual FormFactor::BoundingBox worldBound() const;
	virtual bool canIntersect() const {return true;} 
	virtual bool intersects(FormFactor::Reference<Primitive> &other, std::vector<FormFactor::Reference<Primitive> > &objsHit, bool sameTest = false) const;

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