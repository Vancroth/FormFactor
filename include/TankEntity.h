#pragma once
#include <Ogre.h>
#include "VehicleEntity.h"
#include "LevelTileEntity.h"

using namespace Ogre;

class TankEntity : public VehicleEntity
{
public:
	TankEntity(SceneNode *node);
	~TankEntity(void);

	virtual bool keyPressed(const OIS::KeyEvent &evt);

	virtual void activatePrimary();
	virtual void deactivatePrimary();

	virtual void activateSecondary();
	virtual void deactivateSecondary();

	virtual void collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity);
};
