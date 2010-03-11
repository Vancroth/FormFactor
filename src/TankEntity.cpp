#include "TankEntity.h"

TankEntity::TankEntity(SceneNode *cNode, SceneNode *vNode) : VehicleEntity(cNode, vNode, String("Tank"), String("tank.mesh"))
{
	curMode = TANK;

	lavaShieldEnabled = false;

	primary.init(5, 10);
	secondary.init(5, 10);
}

TankEntity::~TankEntity(void)
{
	mNode->detachObject(vehicleEntity);
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
	lavaShieldEnabled = true;
}

void TankEntity::deactivatePrimary() {
	lavaShieldEnabled = false;
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
		case FormFactor::LevelTileEntity::LAVA: 
			//if (!lavaShieldEnabled) stop();
			break;
	}
}