#include "VehicleEntity.h"

#include "SmokeEmitter.h"

const FormFactor::Vector VehicleEntity::thrust = FormFactor::Vector(0, 0, -100.f);

VehicleEntity::VehicleEntity(SceneNode *node) : FormFactor::PhysicsBody(node, true, 20) {
	gliderVehicle = mSceneMgr->createEntity("Glider", "scout.mesh");
	tankVehicle = mSceneMgr->createEntity("Tank", "tank.mesh");

	primaryCooldown = 5;
	secondaryCooldown = 10;

	curRoll = 0;

	curMode = NONE;

	transform(GLIDER);

	onGround = false;
	this->addForce(thrust);

	// Add smoke emitter
	float sideDist = worldBound().getMaxPoint()[0] - worldBound().getOrigin()[0];
	float upDist = worldBound().getMaxPoint()[1] - worldBound().getOrigin()[1];
	float backDist = worldBound().getMaxPoint()[2] - worldBound().getOrigin()[2];
	FormFactor::SmokeEmitter *smokey = new FormFactor::SmokeEmitter(node->createChildSceneNode(Vector3(-sideDist*.5f, -upDist*2, -backDist*.1)), FormFactor::Vector(0, 0, -1));
}

VehicleEntity::~VehicleEntity() {
	mSceneMgr->destroyEntity(gliderVehicle);
	mSceneMgr->destroyEntity(tankVehicle);
}

bool VehicleEntity::frameEvent(const FrameEvent &evt) {
	curPrimaryCooldown -= evt.timeSinceLastEvent;
	curSecondaryCooldown -= evt.timeSinceLastEvent;
	return true;
}


bool VehicleEntity::keyPressed(const OIS::KeyEvent &evt) {
	Quaternion quat; Vector3 src;
	switch(evt.key) {
		case OIS::KC_1: transform(GLIDER); break;
		case OIS::KC_2: transform(TANK); break;
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
	//if (evt.state.X.rel < 0) {
	//	if (curRow < 5);
	//	mNode->roll(Degree(-5));
	//}
	//if (evt.state.X.rel > 0) {
	//	mNode->roll(Degree(45));
	//}
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
 * Returns TRUE if the ability is the vehicle changes forms, FALSE otherwise
 */
bool VehicleEntity::transform(VehicleMode mode) {
	if (curMode == mode) return false;

	if (curMode != NONE) {
		switch (curMode) {
			case GLIDER: mNode->removeChild("Glider"); break;
			case TANK: mNode->removeChild("Tank"); break;
		}
	}

	switch (mode) {
		case GLIDER: curVehicle = gliderVehicle; break;
		case TANK: curVehicle = tankVehicle; break;
	}
	mNode->attachObject(curVehicle);

	curMode = mode;
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
 	this->setVelocityZ(-50);
	curSecondaryCooldown = secondaryCooldown;
	return true;
}

FormFactor::BoundingBox VehicleEntity::worldBound() const {
	return FormFactor::BoundingBox(curVehicle->getWorldBoundingBox(true));
}

bool VehicleEntity::intersects(FormFactor::Reference<FormFactor::Primitive> &other, std::vector<FormFactor::Reference<FormFactor::Primitive> > &objsHit, bool sameTest) const {
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
		FormFactor::Vector N = -FormFactor::PhysicsBody::gravity.force * mass;

		// Normal force
		this->addForce(N);	

		// Friction force
		float magOfN = N.length();
		FormFactor::Vector negVel = -getVelocity(); negVel.y = 0; negVel.normalize();
		this->addForce(negVel * magOfN * objHit->getCoefficientOfFriction());
	} else {
		int i = 0;
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


