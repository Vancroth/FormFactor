#pragma once
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class OgreApplication : public FrameListener
{ 
public: 
	OgreApplication(); 
	virtual ~OgreApplication(); 

	virtual void go(); 

protected: 
	virtual bool initialise(); 

	virtual bool initOgreCore(); 

	virtual void createSceneManager(); 
	virtual void createCamera(); 
	virtual void createViewports(); 
	virtual void createResourceListener(); 
	virtual void createFrameListener();
	virtual void createInputListener();

	virtual void createScene() = 0; // pure virtual - this has to be overridden 
	virtual void destroyScene(); 

	virtual void addResourceLocations(); 
	virtual void initResources(); 

	virtual void updateStats(); 
	void showDebugOverlay(bool show); 

	virtual void moveCamera(); 

	// FrameListener overrides 
	virtual bool frameStarted(const FrameEvent& evt); 
	virtual bool frameEnded(const FrameEvent& evt); 

	Camera* mCamera;
	SceneManager* mSceneMgr; 
	RenderWindow* mWindow; 
	Real mMoveSpeed; 
	Degree mRotateSpeed; 
	Overlay* mDebugOverlay; 
	Vector3 mTranslateVector; 
	float mMoveScale; 
	Degree mRotScale; 
	Radian mRotX, mRotY; 

private: 
	Real mTimeUntilNextToggle; 
	int mSceneDetailIndex ; 
	bool mStatsOn; 
	unsigned int mNumScreenShots; 
	TextureFilterOptions mFiltering; 
	int mAniso; 
};