#include "OgreApplication.h"
#include "InputController.h"

//-------------------------------------------------------------------------------------
OgreApplication::OgreApplication()
{
}

//-------------------------------------------------------------------------------------
OgreApplication::~OgreApplication()
{
	delete Root::getSingletonPtr();
}

//-------------------------------------------------------------------------------------
bool OgreApplication::initOgreCore()
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(Root::getSingletonPtr()->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = Root::getSingletonPtr()->initialise(true, "FormFactor");
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void OgreApplication::createSceneManager()
{
	// Create the SceneManager, in this case a generic one
	mSceneMgr = Root::getSingletonPtr()->createSceneManager(ST_GENERIC , "Default");
}
//-------------------------------------------------------------------------------------
void OgreApplication::createCamera()
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Vector3(0,0,0));
	mCamera->lookAt(Vector3(0,0,50));
	mCamera->setNearClipDistance(5);
	mCamera->setFarClipDistance(500);
}
//-------------------------------------------------------------------------------------
void OgreApplication::createFrameListener()
{
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	mRotateSpeed = 36;
	mMoveSpeed = 100;

	mStatsOn = true;
	mNumScreenShots = 0;
	mTimeUntilNextToggle = 0;
	mSceneDetailIndex = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mTranslateVector = Vector3::ZERO;
	mAniso = 1;
	mFiltering = TFO_BILINEAR;

	showDebugOverlay(true);
	Root::getSingletonPtr()->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void OgreApplication::createInputListener()
{
	size_t hWnd = 0;
	mWindow->getCustomAttribute("WINDOW", &hWnd);
	OIS::InputManager *inputManager = OIS::InputManager::createInputSystem(hWnd);

	OIS::Keyboard *keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
	OIS::Mouse *mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));

	new InputController(keyboard, mouse);

	unsigned int width, height, depth;
	int top, left;
	mWindow->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
//-------------------------------------------------------------------------------------
void OgreApplication::destroyScene()
{
}
//-------------------------------------------------------------------------------------
void OgreApplication::createViewports()
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void OgreApplication::addResourceLocations()
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
void OgreApplication::createResourceListener()
{

}
//-------------------------------------------------------------------------------------
void OgreApplication::initResources()
{
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void OgreApplication::go()
{
	if (!initialise())
		return;

	// showDebugOverlay(true); // Already called on OgreApplication::createFrameListener().

	Root::getSingletonPtr()->startRendering();

	// clean up
	destroyScene();
}
//-------------------------------------------------------------------------------------
bool OgreApplication::initialise()
{
	new Root("plugins.cfg","ogre.cfg","ogre.log");

	addResourceLocations();

	// if we cannot initialise Ogre, just abandon the whole deal
	if ( !initOgreCore() ) return false;

	createSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Initialise resources
	initResources();

	createInputListener();

	// Create the scene
	createScene();

	createFrameListener();

	return true;
};
//-------------------------------------------------------------------------------------
void OgreApplication::updateStats()
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";

	// update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = mWindow->getStatistics();

		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		//OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		//guiDbg->setCaption(mWindow->getDebugText());
	}
	catch(...)
	{
		// ignore
	}
}
//-------------------------------------------------------------------------------------
void OgreApplication::moveCamera()
{

	// Make all the changes to the camera
	// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
	mCamera->yaw(mRotX);
	mCamera->pitch(mRotY);
	mCamera->moveRelative(mTranslateVector);


}
//-------------------------------------------------------------------------------------
void OgreApplication::showDebugOverlay(bool show)
{
	if (mDebugOverlay)
	{
		if (show)
		{
			mDebugOverlay->show();
		}
		else
		{
			mDebugOverlay->hide();
		}
	}
}
//-------------------------------------------------------------------------------------
bool OgreApplication::frameStarted(const FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	if (mTimeUntilNextToggle >= 0)
		mTimeUntilNextToggle -= evt.timeSinceLastFrame;

	// If this is the first frame, pick a speed
	if (evt.timeSinceLastFrame == 0)
	{
		mMoveScale = 1;
		mRotScale = 0.1;
	}
	// Otherwise scale movement units by time passed since last frame
	else
	{
		// Move about 100 units per second,
		mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
		// Take about 10 seconds for full rotation
		mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
	}
	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Vector3::ZERO;

	moveCamera();

	return true;
}
//-------------------------------------------------------------------------------------
bool OgreApplication::frameEnded(const FrameEvent& evt)
{
	updateStats();
	return true;
}
//-------------------------------------------------------------------------------------