#include "TankEntity.h"

TankEntity::TankEntity(SceneNode *node) : VehicleEntity(node)
{
	vehicle = this;

	vehicleEntity = mSceneMgr->createEntity("Tank", "scout.mesh");

	vehicleNode = mNode->createChildSceneNode("Tank", Vector3(0, -5, -100));
	vehicleNode->setDirection(0, 0, 1);
	vehicleNode->showBoundingBox(true);
	vehicleNode->attachObject(vehicleEntity);

	primary.init(5, 10);
	secondary.init(5, 10);
}

TankEntity::~TankEntity(void)
{
	mSceneMgr->destroyEntity(vehicleEntity);
}

bool TankEntity::keyPressed(const OIS::KeyEvent &evt) {
	switch(evt.key) {
	}

	return true;
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool TankEntity::activatePrimaryAbility() {
	if (!primary.activate()) return false;
	fprintf(stderr, "Primary ability activated\n");

	yAcceleration = 200;

	return true;
}

void TankEntity::primaryAbility(Ogre::Real timeElapsed) {
	primary.duration -= timeElapsed;
	if (primary.duration < 0) {
		primary.activated = false;
		yAcceleration = 0;
	}
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool TankEntity::activateSecondaryAbility() {
	if (!secondary.activate()) return false;
	fprintf(stderr, "Secondary ability activated\n");

	// Store the previous velocity before the acceleration
	moveSpeed = getVelocity().z;
	zAcceleration = -1000;

	return true;
}

void TankEntity::secondaryAbility(Ogre::Real timeElapsed) {
	secondary.duration -= timeElapsed;
	if (secondary.duration < 0) {
		secondary.activated = false;
		zAcceleration = 0;
		this->setVelocityZ(moveSpeed);
	}
}

void TankEntity::collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity) {
	switch (levelEntity->getTerrainType()) {
		case FormFactor::LevelTileEntity::LAVA: break;
	}
}