#include "FormFactorApplication.h"

#include <vector>

#include "VehicleEntity.h"
#include "LevelEntity.h"
#include "PowerUp.h"

#include "PhysicsBody.h"
#include "Primitive.h"

int sceneShift = 80;
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
	mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

	Light *light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(250, 150, 250));
	light->setDiffuseColour(ColourValue::White);
	light->setSpecularColour(ColourValue::White);

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	//mSceneMgr->setWorldGeometry("terrain.cfg");

	std::vector<FormFactor::Reference<FormFactor::Primitive> > primitives;

	// Create the player
	SceneNode *playerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Player", Vector3(0, sceneShift, 0));
	playerNode->setDirection(Vector3(-1, 0, 0));
	playerNode->showBoundingBox(true);
	VehicleEntity *vehicle = new VehicleEntity(playerNode);
	vehicle->start();
	//primitives.push_back(vehicle);

	SceneNode *cameraNode = playerNode->createChildSceneNode("PlayerCamera", Vector3(0, 0, -50));
	cameraNode->attachObject(mCamera);

	// Create the level
	SceneNode *levelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Level", Vector3(0, sceneShift-25, 0));
	FormFactor::LevelEntity *level = new FormFactor::LevelEntity(levelNode, primitives);
	level->start();
	
	// Build main collision tree
	tree = new FormFactor::KdTree(primitives, 40, 1, .5f, 4, 20);
	FormFactor::PhysicsBody::setTree(tree);
}
//-------------------------------------------------------------------------------------
bool FormFactorApplication::frameStarted(const FrameEvent& evt)
{
	// Do physics update
	FormFactor::PhysicsBody::simulatePhysics(evt.timeSinceLastFrame*1000);  // put time in ms


    return OgreApplication::frameStarted(evt);
}
//-------------------------------------------------------------------------------------

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