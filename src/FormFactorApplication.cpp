#include "FormFactorApplication.h"

#include <vector>

#include "GliderEntity.h"
#include "TankEntity.h"
#include "LevelEntity.h"
#include "PowerUp.h"

#include "PhysicsBody.h"
#include "Primitive.h"

int sceneShift = 50;
SceneNode *cameraNode;
VehicleEntity *vehicle;

//-------------------------------------------------------------------------------------
FormFactorApplication::FormFactorApplication()
{
}
//-------------------------------------------------------------------------------------
FormFactorApplication::~FormFactorApplication()
{
}
//-------------------------------------------------------------------------------------
void FormFactorApplication::createScene()
{
	InputController::getSingletonPtr()->addKeyListener(this);

	mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerCamera", Vector3(0, sceneShift, 40));
	mCamera->setDirection(0, 0, -1);
	cameraNode->attachObject(mCamera);

	Light *light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);
	cameraNode->attachObject(light);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	//mSceneMgr->setWorldGeometry("terrain.cfg");

	std::vector<FormFactor::Reference<FormFactor::Primitive> > primitives;

	// Create the player
	vehicle = new GliderEntity(cameraNode);
	vehicle->start();
	//primitives.push_back(vehicle);

	// Create the level
	SceneNode *levelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Level", Vector3(0, sceneShift-30, 0));
	FormFactor::LevelEntity *level = new FormFactor::LevelEntity(levelNode, primitives);
	level->start();
	
	// Build main collision tree
	tree = new FormFactor::KdTree(primitives, 40, 1, .5f, 4, 20);
	FormFactor::PhysicsBody::setTree(tree);
}
//-------------------------------------------------------------------------------------
void FormFactorApplication::destroyScene() {
	mSceneMgr->destroyAllMovableObjects();
	InputController::getSingletonPtr()->removeKeyListener(this);
}
//-------------------------------------------------------------------------------------
bool FormFactorApplication::frameStarted(const FrameEvent& evt)
{
	// Do physics update
	FormFactor::PhysicsBody::simulatePhysics(evt.timeSinceLastFrame*1000);  // put time in ms


    return OgreApplication::frameStarted(evt);
}
//-------------------------------------------------------------------------------------
bool FormFactorApplication::keyPressed(const OIS::KeyEvent &evt)
{
	switch(evt.key) {
		case OIS::KC_ESCAPE: destroyScene(); exit(0);
		case OIS::KC_0:
			delete vehicle;
			vehicle = new TankEntity(cameraNode);
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool FormFactorApplication::keyReleased(const OIS::KeyEvent &evt)
{
	return true;
}

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    FormFactorApplication app;

    try {
        app.go();
    } catch(Exception& e) {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }

    return 0;
}