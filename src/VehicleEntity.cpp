#include "VehicleEntity.h"

const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -100);

VehicleEntity::VehicleEntity(SceneNode *node) : FormFactor::PhysicsBody(node, true, 100) {
	vehicle = mSceneMgr->createEntity("Vehicle", "scout.mesh");
	mNode->attachObject(vehicle);

	primaryCooldown = 5;
	secondaryCooldown = 10;

	onGround = false;
	this->addForce(thrust);
}

VehicleEntity::~VehicleEntity() {
	mSceneMgr->destroyEntity(vehicle);
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	curPrimaryCooldown -= evt.timeSinceLastEvent;
	curSecondaryCooldown -= evt.timeSinceLastEvent;
	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	Quaternion quat; Vector3 src;
	switch(evt.key) {		
		case OIS::KC_A: mNode->yaw(Degree(-90)); break;
		case OIS::KC_D: mNode->yaw(Degree(90)); break;
	}

	return true;
}


bool VehicleEntity::mouseMoved(const OIS::MouseEvent &evt) {
	this->setVelocityX(evt.state.X.rel);
	return true;
}

bool VehicleEntity::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
	switch (id) {
		// Activate primary ability
		case OIS::MB_Left: primaryAbility(); break;
	    // Activate secondary ability
		case OIS::MB_Right: secondaryAbility(); break;
	}
	return true;
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool VehicleEntity::primaryAbility() {
	if (curPrimaryCooldown > 0) return false;
 	this->setVelocityY(25);
	curPrimaryCooldown = primaryCooldown;
	return true;
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool VehicleEntity::secondaryAbility() {
	if (curSecondaryCooldown > 0) return false;
 	this->setVelocityZ(50);
	curSecondaryCooldown = secondaryCooldown;
	return true;
}

FormFactor::BoundingBox VehicleEntity::worldBound() const {
	return FormFactor::BoundingBox(vehicle->getWorldBoundingBox(true));
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
		this->addForce(-FormFactor::PhysicsBody::gravity.force);
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


