#include <Ogre.h>
#include <OgreApplicationContext.h>
#include "uilayer.h"
#include <iostream>

class ImguiExample : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	uiLayer ui;
	ImguiExample() : OgreBites::ApplicationContext("OgreImguiExample")
	{
	}

	bool frameStarted(const Ogre::FrameEvent& evt)
	{
		OgreBites::ApplicationContext::frameStarted(evt);

		ui.updateWindow2(evt.timeSinceLastFrame);
		return true;
	}

#ifndef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	void locateResources()
	{
		OgreBites::ApplicationContext::locateResources();
		// we have to manually specify the shaders
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
			"../resources", "FileSystem", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
	}
#endif

	void setup()
	{
		OgreBites::ApplicationContext::setup();
		addInputListener(this);

		//Ogre::ImguiManager::createSingleton();
		//addInputListener(Ogre::ImguiManager::getSingletonPtr());

		// get a pointer to the already created root
		Ogre::Root* root = getRoot();
		Ogre::SceneManager* scnMgr = root->createSceneManager();
		//Ogre::ImguiManager::getSingleton().init(scnMgr);
		//ui = uiLayer(this,scnMgr);
		ui = uiLayer(this, scnMgr);
		//ui.setConversition(std::string("../Media/ui/Conversition/test.txt"));
		ui.conversitionAddTalk(std::string(u8"1"), std::string(u8"你好"), std::string(u8"世界"));
		ui.conversitionAddTalk(std::string(u8"2"), std::string(u8"你好2"), std::string(u8"世界2"));
		ui.conversitionAddTalk(std::string(u8"1"), std::string(u8"你好"), std::string(u8"世界"));
		ui.conversitionAddTalk(std::string(u8"2"), std::string(u8"你好2"), std::string(u8"世界2"));
		ui.conversitionAddTalk(std::string(u8"1"), std::string(u8"你好"), std::string(u8"世界"));
		ui.addUIpicPositionSet(std::string(u8"1"), Ogre::Vector3(-10, 0, -20), Ogre::Vector3(0.02, 0.02, 0.02));
		ui.addUIpicPositionSet(std::string(u8"2"), Ogre::Vector3(5, 0, -20), Ogre::Vector3(0.02, 0.02, 0.02));


		// register our scene with the RTSS
		Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		shadergen->addSceneManager(scnMgr);


		Ogre::Light* light = scnMgr->createLight("MainLight");
		Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		lightNode->setPosition(0, 10, 15);
		lightNode->attachObject(light);


		Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		camNode->setPosition(0, 0, 15);
		camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

		Ogre::Camera* cam = scnMgr->createCamera("myCam");
		cam->setNearClipDistance(5); // specific to this sample
		cam->setAutoAspectRatio(true);
		camNode->attachObject(cam);
		getRenderWindow()->addViewport(cam);

		Ogre::Entity* ent = scnMgr->createEntity("Sinbad.mesh");
		Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
		node->attachObject(ent);
	}

	bool keyPressed(const OgreBites::KeyboardEvent& evt)
	{
		if (evt.keysym.sym == 27)
		{
			getRoot()->queueEndRendering();
		}
		else if (evt.keysym.sym == int('a')) {
			ui.pullTrigger();
		}
		else if (evt.keysym.sym == int('=')) {
			static Ogre::Vector3 v = Ogre::Vector3(-10, 0, -20);
			v += Ogre::Vector3(0, 0, 0.5);
			ui.setCharUIPosition(std::string("2"), v);
		}
		return true;
	}
};


int main(int argc, char *argv[])
{
	ImguiExample app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();

	return 0;
}
