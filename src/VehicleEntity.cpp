#include "VehicleEntity.h"
#include "GliderEntity.h"
#include "SmokeEmitter.h"

const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -100);
VehicleEntity* VehicleEntity::vehicle = NULL;

VehicleEntity* VehicleEntity::getSingletonPtr(void) {
	return vehicle;
}

void VehicleEntity::setSingletonPtr(VehicleEntity *entity) {
	vehicle = entity;
}

VehicleEntity::VehicleEntity(SceneNode *cNode, SceneNode *vNode, String &name, String &mesh) : FormFactor::PhysicsBody(cNode, true, 500) {
	cameraNode = cNode;
	vehicleNode = vNode;

	vehicleEntity = mSceneMgr->createEntity(name, mesh);

	xAcceleration = 0;
	yAcceleration = 0;
	zAcceleration = 0;

	curRoll = 0;

	onGround = false;
	clearPhysicsState();

	// Add smoke emitter
	float sideDist = worldBound().getMaxPoint()[0] - worldBound().getOrigin()[0];
	float upDist = worldBound().getMaxPoint()[1] - worldBound().getOrigin()[1];
	float backDist = worldBound().getMaxPoint()[2] - worldBound().getOrigin()[2];
	//FormFactor::SmokeEmitter *smokey = new FormFactor::SmokeEmitter(mNode->createChildSceneNode(Vector3(-sideDist*.5f, -upDist*2, -backDist*.1)), FormFactor::Vector(0, 0, -1));
}

VehicleEntity::~VehicleEntity() {
}

void VehicleEntity::attachVehicle() {
	VehicleEntity::setSingletonPtr(this);
	vehicleNode->detachAllObjects();
	vehicleNode->attachObject(vehicleEntity);
	start();
}

Entity *VehicleEntity::getEntity() {
	return vehicleEntity;
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	//float rollAmount = rollDirection * evt.timeSinceLastFrame;
	//if (curRoll + rollAmount > -45 && curRoll + rollAmount < 45) {
	//	vehicleNode->roll(Degree(rollAmount));
	//	curRoll += rollDirection;
	//}
	this->clearPhysicsState();

	// Handle abilities
	if (primary.updateState(evt.timeSinceLastFrame)) { deactivatePrimary(); }
	if (secondary.updateState(evt.timeSinceLastFrame)) { deactivateSecondary(); }

	// Handle movement
	this->addForce(FormFactor::Vector(xAcceleration, yAcceleration, zAcceleration));

	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	Quaternion quat; Vector3 src;
	switch(evt.key) {
		case OIS::KC_A: mNode->yaw(Degree(-90)); break;
		case OIS::KC_D: mNode->yaw(Degree(90)); break;

		case OIS::KC_UP: this->setVelocity(FormFactor::Vector(0, 50, 0)); break;
		case OIS::KC_DOWN: this->setVelocity(FormFactor::Vector(0, -50, 0)); break;
		case OIS::KC_LEFT: this->setVelocity(FormFactor::Vector(-50, 0, 0)); break;
		case OIS::KC_RIGHT: this->setVelocity(FormFactor::Vector(50, 0, 0)); break;

	}

	return true;
}

bool VehicleEntity::mouseMoved(const OIS::MouseEvent &evt) {
	// TODO: Make the vehicle bank left and right while moving, currently attached to camera so causes problems
	if (evt.state.X.rel < 0) {
		rollDirection = -1;
	} else if (evt.state.X.rel > 0) {
		rollDirection = 1;
	}
	xAcceleration = (evt.state.X.rel * evt.state.X.rel) * 3;
	if (evt.state.X.rel < 0) {
		xAcceleration = -xAcceleration;
	}
	return true;
}

bool VehicleEntity::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) {
	switch (id) {
		// Activate primary ability
		case OIS::MB_Left: if (primary.activate()) { activatePrimary(); }; break;
	    // Activate secondary ability
		case OIS::MB_Right: if (secondary.activate()) { activateSecondary(); }; break;
	}
	return true;
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

bool VehicleEntity::intersects(FormFactor::Reference<Primitive> &other, std::vector<FormFactor::Reference<Primitive> > &objsHit, bool sameTest) const {
	objsHit.push_back(const_cast<VehicleEntity*>(this));
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
	}
	this->coefficientOfFriction = objHit->getCoefficientOfFriction();
	FormFactor::Vector newVel = objHit->handleVehicleCollision(this->vel, this->mass, dir);
	this->setVelocity(newVel);
}

void VehicleEntity::clearPhysicsState() {
	this->clearForces();
	this->addForce(thrust);
	if (onGround) {
		FormFactor::Vector N = -FormFactor::PhysicsBody::gravity.force * mass;

		// Normal force
		this->addForce(N);

		// Friction force
		float magOfN = N.length();
		FormFactor::Vector negVel = -getVelocity(); negVel.y = 0; negVel.normalize();
		this->addForce(negVel * magOfN * this->getCoefficientOfFriction());
	}
	onGround = false;
}
	
void VehicleEntity::updateGraphicalPosition(const FormFactor::Vector &shiftAmt) {
	mNode->translate(shiftAmt.getOgreVector());
}


