#include "InputController.h"

InputController* InputController::mInputController = NULL;

InputController* InputController::getSingletonPtr(void) {
	return mInputController;
}

InputController::InputController(OIS::Keyboard *keyboard, OIS::Mouse *mouse) {
	Root::getSingletonPtr()->addFrameListener(this);
	mKeyboard = keyboard;
	mMouse = mouse;
	mInputController = this;
	if (mKeyboard) {
		mKeyboard->setEventCallback(this);
	}
	if (mMouse) {
		mMouse->setEventCallback(this);
	}
}

InputController::~InputController() {
}

bool InputController::frameStarted(const FrameEvent &evt) {
	if (mMouse) {
		mMouse->capture();
	}
	if (mKeyboard) {
		mKeyboard->capture();
	}
	return true;
}

void InputController::addKeyListener(OIS::KeyListener *listener) {
	keyListeners.push_back(listener);
}

void InputController::addMouseListener(OIS::MouseListener *listener) {
	mouseListeners.push_back(listener);
}

void InputController::removeKeyListener(OIS::KeyListener *listener) {
}

void InputController::removeMouseListener(OIS::MouseListener *listener) {
}

bool InputController::keyPressed(const OIS::KeyEvent &e) {
	for (size_t i = 0; i < keyListeners.size(); i++) {
		if(!keyListeners[i]->keyPressed(e))
			return false;
	}
	return true;
}

bool InputController::keyReleased(const OIS::KeyEvent &e) {
	for (size_t i = 0; i < keyListeners.size(); i++) {
		if(!keyListeners[i]->keyReleased(e))
			return false;
	}
	return true;
}

bool InputController::mouseMoved(const OIS::MouseEvent &e) {
	for (size_t i = 0; i < mouseListeners.size(); i++) {
		if(!mouseListeners[i]->mouseMoved(e))
			return false;
	}
	return true;
}

bool InputController::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
	for (size_t i = 0; i < mouseListeners.size(); i++) {
		if(!mouseListeners[i]->mousePressed(e, id))
			return false;
	}
	return true;
}

bool InputController::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
	for (size_t i = 0; i < mouseListeners.size(); i++) {
		if(!mouseListeners[i]->mouseReleased(e, id))
			return false;
	}
	return true;
}
