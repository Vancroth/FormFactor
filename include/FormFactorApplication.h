#pragma once
#include "OgreApplication.h"

#include "KdTree.h"
#include "Reference.h"

class FormFactorApplication : public OgreApplication
{
public:
	FormFactorApplication(void);
	virtual ~FormFactorApplication(void);

protected:
	virtual void createScene();
	virtual bool frameStarted(const FrameEvent& evt);

private:
	FormFactor::Reference<FormFactor::KdTree> tree;
};
