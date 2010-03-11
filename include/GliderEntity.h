#pragma once
#include <Ogre.h>
#include "VehicleEntity.h"
#include "LevelTileEntity.h"

using namespace Ogre;

class GliderEntity : public VehicleEntity
{
public:
	GliderEntity(SceneNode *node);
	~GliderEntity(void);

	virtual bool keyPressed(const OIS::KeyEvent &evt);

	virtual bool activatePrimaryAbility();
	virtual void primaryAbility(Ogre::Real timeElapsed);

	virtual bool activateSecondaryAbility();
	virtual void secondaryAbility(Ogre::Real timeElapsed);

	virtual void collideWithLevelTile(FormFactor::LevelTileEntity *levelEntity);
};
