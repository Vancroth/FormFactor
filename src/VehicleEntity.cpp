#include "VehicleEntity.h"
#include "GliderEntity.h"

const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -100);
VehicleEntity* VehicleEntity::vehicle = NULL;

VehicleEntity* VehicleEntity::getSingletonPtr(void) {
	return vehicle;
}

void VehicleEntity::setSingletonPtr(VehicleEntity *entity) {
	vehicle = entity;
}

VehicleEntity::VehicleEntity(SceneNode *node) : FormFactor::PhysicsBody(node, true, 500) {
	vehicle = this;

	cameraNode = mNode;

	moveSpeed = 0;

	xVelocity = 0;
	xAcceleration = 0;

	yVelocity = 0;
	yAcceleration = 0;

	zVelocity = 0;
	zAcceleration = 0;

	curRoll = 0;
	curMode = NONE;

	onGround = false;
	this->addForce(thrust);
}

VehicleEntity::~VehicleEntity() {
}

Entity *VehicleEntity::getEntity() {
	return vehicleEntity;
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	//float rollAmount = rollDirection * evt.timeSinceLastEvent;
	//if (curRoll + rollAmount > -45 && curRoll + rollAmount < 45) {
	//	vehicleNode->roll(Degree(rollAmount));
	//	curRoll += rollDirection;
	//}
	this->clearPhysicsState();

	// Handle abilities
	primary.cooldown -= evt.timeSinceLastEvent;
	primary.duration -= evt.timeSinceLastEvent;
	if (primary.activated) {
		primaryAbility(evt.timeSinceLastEvent);
	}

	secondary.cooldown -= evt.timeSinceLastEvent;
	secondary.duration -= evt.timeSinceLastEvent;
	if (secondary.activated) {
		secondaryAbility(evt.timeSinceLastEvent);
	}

	// Handle movement
	this->addForce(FormFactor::Vector(xAcceleration, yAcceleration, zAcceleration));

	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	return true;
}

bool VehicleEntity::mouseMoved(const OIS::MouseEvent &evt) {
	// TODO: Make the vehicle bank left and right while moving, currently attached to camera so causes problems
	if (evt.state.X.rel < 0) {
		rollDirection = -5;
	} else if (evt.state.X.rel > 0) {
		rollDirection = 5;
	}
	xAcceleration = (evt.state.X.rel * evt.state.X.rel) / 4;
	if (evt.state.X.rel < 0) {
		xAcceleration = -xAcceleration;
	}
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

void VehicleEntity::destroy() {
	stop();
}

VehicleEntity::VehicleMode VehicleEntity::getVehicleMode() {
	return curMode;
}

bool VehicleEntity::isOnGround() {
	return onGround;
}

FormFactor::BoundingBox VehicleEntity::worldBound() const {
	return FormFactor::BoundingBox(vehicleEntity->getWorldBoundingBox(true));
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
	
void VehicleEntity::updateGraphicalPosition(const FormFactor::Vector &shiftAmt) {
	mNode->translate(shiftAmt.getOgreVector());
}


