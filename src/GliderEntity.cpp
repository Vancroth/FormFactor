#include "GliderEntity.h"

GliderEntity::GliderEntity(SceneNode *cNode, SceneNode *vNode) : VehicleEntity(cNode, vNode, String("Glider"), String("scout.mesh"))
{
	curMode = GLIDER;

	primary.init(1, 5);
	secondary.init(2, 5);
}

GliderEntity::~GliderEntity(void)
{
	mNode->detachObject(vehicleEntity);
	mSceneMgr->destroyEntity(vehicleEntity);
}

bool GliderEntity::keyPressed(const OIS::KeyEvent &evt) {
	switch(evt.key) {
	}

	return true;
}

void GliderEntity::activatePrimary() {
	yAcceleration = 800;
}

void GliderEntity::deactivatePrimary() {
	yAcceleration = 0;
}

void GliderEntity::activateSecondary() {
	// Store the previous velocity before the acceleration
	moveSpeed = getVelocity().z;
	zAcceleration = -1000;
}

void GliderEntity::deactivateSecondary() {
	this->setVelocityZ(moveSpeed);
	zAcceleration = 0;
}

void GliderEntity::collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity) {
	switch (levelEntity->getTerrainType()) {
		case FormFactor::LevelTileEntity::LAVA: break;
	}
}