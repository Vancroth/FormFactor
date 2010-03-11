#pragma once
#include <Ogre.h>
#include <OIS/OIS.h>
#include "InputController.h"
#include "Reference.h"

using namespace Ogre;

class GameEntity : public FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	GameEntity() {}

	virtual ~GameEntity(void) {}

	virtual void start() {
		Root::getSingletonPtr()->addFrameListener(this);
		InputController::getSingletonPtr()->addKeyListener(this);
		InputController::getSingletonPtr()->addMouseListener(this);
		started = true;
	}

	virtual void stop() {
		Root::getSingletonPtr()->removeFrameListener(this);
		InputController::getSingletonPtr()->removeKeyListener(this);
		InputController::getSingletonPtr()->removeMouseListener(this);
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
};
