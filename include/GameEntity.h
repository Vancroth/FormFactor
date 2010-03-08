#pragma once
#include <Ogre.h>
#include <OIS/OIS.h>
#include "InputController.h"
#include "Reference.h"

using namespace Ogre;

class GameEntity : public FrameListener, public OIS::KeyListener, public OIS::MouseListener, public FormFactor::ReferenceCounted
{
public:
	GameEntity() {mNode = NULL;}

	GameEntity(SceneNode *node) {
		Root::getSingletonPtr()->addFrameListener(this);
		mSceneMgr = Root::getSingletonPtr()->getSceneManager("Default");
		mNode = node;
		InputController::getSingletonPtr()->addKeyListener(this);
		InputController::getSingletonPtr()->addMouseListener(this);
	}

	virtual ~GameEntity(void) {
		Root::getSingletonPtr()->removeFrameListener(this);
		InputController::getSingletonPtr()->removeKeyListener(this);
		InputController::getSingletonPtr()->removeMouseListener(this);
	}

	virtual void start() {
		started = true;
	}

	virtual void stop() {
		started = false;
	}
	
	bool isStarted() {
		return started;
	}

	bool frameStarted(const FrameEvent &evt) {
		if (this->started) {
			return frameEvent(evt);
		}
		return true;
	}

	virtual bool keyPressed(const OIS::KeyEvent &evt) {
		return true;
	}

	virtual bool keyReleased(const OIS::KeyEvent &evt) {
		return true;
	}

	virtual bool mouseMoved(const OIS::MouseEvent &evt) {
		return true;
	}

	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id ) {
		return true;
	}

	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id ) {
		return true;
	}

	virtual bool frameEvent(const FrameEvent &evt) = 0;

protected:
	bool started;
	SceneManager *mSceneMgr;
	SceneNode *mNode;
};