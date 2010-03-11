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

	virtual bool activatePrimaryAbility();
	virtual void primaryAbility(Ogre::Real timeElapsed);

	virtual bool activateSecondaryAbility();
	virtual void secondaryAbility(Ogre::Real timeElapsed);

	virtual void collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity);
};
