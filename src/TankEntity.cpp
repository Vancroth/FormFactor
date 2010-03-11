#include "TankEntity.h"

TankEntity::TankEntity(SceneNode *node) : VehicleEntity(node, String("Tank"), String("scout.mesh"))
{
	curMode = TANK;

	vehicleNode->setDirection(0, 0, 1);

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
void TankEntity::activatePrimary() {
}

void TankEntity::deactivatePrimary() {
}

/**
 * Returns TRUE if the ability is activated successfully, FALSE otherwise
 */
void TankEntity::activateSecondary() {
}

void TankEntity::deactivateSecondary() {
}

void TankEntity::collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity) {
	switch (levelEntity->getTerrainType()) {
		case FormFactor::LevelTileEntity::LAVA: break;
	}
}