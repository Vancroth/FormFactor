#pragma once
#include <vector>
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class InputController : public FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputController(OIS::Keyboard *keyboard, OIS::Mouse *mouse);
	~InputController(void);

	bool frameStarted(const FrameEvent &evt);

	static InputController* getSingletonPtr(void);

	void addKeyListener(OIS::KeyListener *listener);
	void addMouseListener(OIS::MouseListener *listener);

	void removeKeyListener(OIS::KeyListener *listener);
	void removeMouseListener(OIS::MouseListener *listener);

	bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

protected:
	OIS::Keyboard *mKeyboard;
	OIS::Mouse *mMouse;
	std::vector<OIS::KeyListener*> keyListeners;
	std::vector<OIS::MouseListener*> mouseListeners;

	static InputController *mInputController;
};