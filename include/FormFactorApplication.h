#pragma once
#include "OgreApplication.h"

#include "KdTree.h"
#include "Reference.h"

class FormFactorApplication : public OgreApplication, public OIS::KeyListener
{
public:
	FormFactorApplication(void);
	virtual ~FormFactorApplication(void);

protected:
	virtual void createScene();
	virtual void destroyScene();
	virtual bool frameStarted(const FrameEvent& evt);
	
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool keyReleased(const OIS::KeyEvent &evt);

private:
	FormFactor::Reference<FormFactor::KdTree> tree;
};
