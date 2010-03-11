#include "GliderEntity.h"

GliderEntity::GliderEntity(SceneNode *node) : VehicleEntity(node)
{
	vehicle = this;

	vehicleEntity = mSceneMgr->createEntity("Glider", "scout.mesh");

	vehicleNode = mNode->createChildSceneNode("Glider", Vector3(0, -5, -100));
	vehicleNode->setDirection(0, 0, 1);
	vehicleNode->showBoundingBox(true);
	vehicleNode->attachObject(vehicleEntity);

	primary.init(1, 5);
	secondary.init(1, 5);
}

GliderEntity::~GliderEntity(void)
{
	mSceneMgr->destroyEntity(vehicleEntity);
}

bool GliderEntity::keyPressed(const OIS::KeyEvent &evt) {
	switch(evt.key) {
	}

	return true;
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool GliderEntity::activatePrimaryAbility() {
	if (!primary.activate()) return false;
	fprintf(stderr, "Primary ability activated\n");

	yAcceleration = 200;

	return true;
}

void GliderEntity::primaryAbility(Ogre::Real timeElapsed) {
	if (primary.duration < 0) {
		primary.activated = false;
		yAcceleration = 0;
	}
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
bool GliderEntity::activateSecondaryAbility() {
	if (!secondary.activate()) return false;
	fprintf(stderr, "Secondary ability activated\n");

	// Store the previous velocity before the acceleration
	moveSpeed = getVelocity().z;
	zAcceleration = -1000;

	return true;
}

void GliderEntity::secondaryAbility(Ogre::Real timeElapsed) {
	if (secondary.duration < 0) {
		secondary.activated = false;
		zAcceleration = 0;
		this->setVelocityZ(moveSpeed);
	}
}

void GliderEntity::collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity) {
	switch (levelEntity->getTerrainType()) {
		case FormFactor::LevelTileEntity::LAVA: destroy(); break;
	}
}