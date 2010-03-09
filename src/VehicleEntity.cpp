#include "VehicleEntity.h"

const int VehicleEntity::movementUpSpeed = 5;
const int VehicleEntity::movementSpeed = 10;
const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -40);

VehicleEntity::VehicleEntity(SceneNode *node) : FormFactor::PhysicsBody(node, true, 25) {
	vehicle = mSceneMgr->createEntity("Vehicle", "scout.mesh");
	mNode->attachObject(vehicle);

	onGround = false;
	this->addForce(thrust);
}

VehicleEntity::~VehicleEntity() {
	mSceneMgr->destroyEntity(vehicle);
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	Quaternion quat; Vector3 src;
	switch(evt.key) {
		case OIS::KC_LEFT: this->setVelocity(Vector3(-movementSpeed, 0, 0)); break;
		case OIS::KC_RIGHT: this->setVelocity(Vector3(movementSpeed, 0, 0)); break;
		case OIS::KC_UP: if(true)this->setVelocity(Vector3(0, movementUpSpeed, 0)); break;
		case OIS::KC_DOWN: this->setVelocity(Vector3(0, -movementUpSpeed, 0)); break;
		case OIS::KC_W: this->setVelocity(Vector3(0, 0, -movementSpeed)); break;
		case OIS::KC_S: this->setVelocity(Vector3(0, 0, movementSpeed)); break;
		
		case OIS::KC_A: mNode->yaw(Degree(-90)); break;
		case OIS::KC_D: mNode->yaw(Degree(90)); break;
		
	}

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


