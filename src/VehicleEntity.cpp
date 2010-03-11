#include "VehicleEntity.h"

const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -100);
VehicleEntity* VehicleEntity::vehicle = NULL;

VehicleEntity* VehicleEntity::getSingletonPtr(void) {
	return vehicle;
}

VehicleEntity::VehicleEntity(SceneNode *node) : FormFactor::PhysicsBody(node, true, 100) {
	vehicle = this;

	cameraNode = mNode;

	vehicleNode = cameraNode->createChildSceneNode("Player", Vector3(0, 0, -100));
	vehicleNode->setDirection(0, 0, 1);
	vehicleNode->showBoundingBox(true);

	gliderVehicle = mSceneMgr->createEntity("Glider", "scout.mesh");
	tankVehicle = mSceneMgr->createEntity("Tank", "tank.mesh");

	moveSpeed = 0;

	xVelocity = 0;
	xAcceleration = 0;

	yVelocity = 0;
	yAcceleration = 0;

	zVelocity = 0;
	zAcceleration = 0;

	curRoll = 0;
	curMode = NONE;

	transform(GLIDER);

	onGround = false;
	this->addForce(thrust);
}

VehicleEntity::~VehicleEntity() {
	mSceneMgr->destroyEntity(gliderVehicle);
	mSceneMgr->destroyEntity(tankVehicle);
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	this->clearPhysicsState();

	// Handle abilities
	if (primary.cooldown > 0) {
		primary.cooldown -= evt.timeSinceLastEvent;
	}
	if (primary.activated) {
		primaryAbility(evt.timeSinceLastEvent);
	}

	if (secondary.cooldown > 0) {
		secondary.cooldown -= evt.timeSinceLastEvent;
	}
	if (secondary.activated) {
		secondaryAbility(evt.timeSinceLastEvent);
	}

	// Handle movement
	this->addForce(FormFactor::Vector(xAcceleration, yAcceleration, zAcceleration));

	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	Quaternion quat; Vector3 src;
	switch(evt.key) {
		case OIS::KC_1: transform(GLIDER); break;
		case OIS::KC_2: transform(TANK); break;
		case OIS::KC_A: cameraNode->yaw(Degree(-90)); break;
		case OIS::KC_D: cameraNode->yaw(Degree(90)); break;
	}

	return true;
}

bool VehicleEntity::mouseMoved(const OIS::MouseEvent &evt) {
	// TODO: Make the vehicle bank left and right while moving, currently attached to camera so causes problems
	//if (evt.state.X.rel < 0) {
	//	if (curRow < 5);
	//	mNode->roll(Degree(-5));
	//}
	//if (evt.state.X.rel > 0) {
	//	mNode->roll(Degree(45));
	//}
	xAcceleration = evt.state.X.rel * 10.0;
	return true;
}

bool VehicleEntity::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
	switch (id) {
		// Activate primary ability
		case OIS::MB_Left: activatePrimaryAbility(); break;
	    // Activate secondary ability
		case OIS::MB_Right: activateSecondaryAbility(); break;
	}
	return true;
}

/**
 * Returns TRUE if the ability is the vehicle changes forms, FALSE otherwise
 */
bool VehicleEntity::transform(VehicleMode mode) {
	if (curMode == mode) return false;

	if (curMode != NONE) {
		switch (curMode) {
			case GLIDER:
				vehicleNode->removeChild("Glider");
				primary.initAbility(5, 10);
				secondary.initAbility(5, 10);
				break;
			case TANK:
				vehicleNode->removeChild("Tank");
				primary.initAbility(5, 10);
				secondary.initAbility(5, 10);
				break;
		}
	}

	switch (mode) {
		case GLIDER: curVehicle = gliderVehicle; break;
		case TANK: curVehicle = tankVehicle; break;
	}
	vehicleNode->attachObject(curVehicle);

	curMode = mode;
	return true;
}

void VehicleEntity::destroy() {
	stop();
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool VehicleEntity::activatePrimaryAbility() {
	if (!primary.activate()) return false;

	yAcceleration = 250;

	return true;
}

void VehicleEntity::primaryAbility(Ogre::Real timeElapsed) {
	primary.duration -= timeElapsed;
	if (primary.duration < 0) {
		primary.activated = false;
		yAcceleration = 0;
	}
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool VehicleEntity::activateSecondaryAbility() {
	if (!secondary.activate()) return false;

	// Store the previous velocity before the acceleration
	moveSpeed = getVelocity().z;
	zAcceleration = -500;

	return true;
}

void VehicleEntity::secondaryAbility(Ogre::Real timeElapsed) {
	secondary.duration -= timeElapsed;
	if (secondary.duration < 0) {
		secondary.activated = false;
		zAcceleration = 0;
		this->setVelocityZ(moveSpeed);
	}
}

VehicleEntity::VehicleMode VehicleEntity::getVehicleMode() {
	return curMode;
}

bool VehicleEntity::isOnGround() {
	return onGround;
}

FormFactor::BoundingBox VehicleEntity::worldBound() const {
	return FormFactor::BoundingBox(curVehicle->getWorldBoundingBox(true));
}

bool VehicleEntity::intersects(FormFactor::Reference<FormFactor::Primitive> &other, FormFactor::Reference<FormFactor::Primitive> &objHit) const {
	objHit = const_cast<VehicleEntity*>(this);
	if(other->canIntersect()) {
		return worldBound().intersects(other->worldBound());
	} else {
		std::vector<FormFactor::Reference<FormFactor::Primitive> > refined;
		other->fullyRefine(refined);
		for(unsigned int i = 0; i < refined.size(); i++) 
			if(worldBound().intersects(refined[i]->worldBound())) return true;
		return false;
	}
}

void VehicleEntity::handleCollision(FormFactor::Reference<FormFactor::Primitive> &objHit, const FormFactor::Vector &dir) {
	if(!onGround && dir.y==-1) {
		onGround = true;
		FormFactor::Vector N = -FormFactor::PhysicsBody::gravity.force;

		// Normal force
		this->addForce(N);
	}
	FormFactor::Vector newVel = objHit->handleVehicleCollision(this->vel, this->mass, dir);
	this->setVelocity(newVel);
}

void VehicleEntity::clearPhysicsState() {
	forces.clear();
	this->addForce(thrust);
	onGround = false;
}
	
void VehicleEntity::updateGraphicalPosition(const FormFactor::Vector &amountShifted) {
	mNode->translate(amountShifted.getOgreVector());
}


