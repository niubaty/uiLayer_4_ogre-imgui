#pragma once
#include <Ogre.h>
#include <imgui.h>
#include <OgreCommon.h>
#include <OgreApplicationContextBase.h>
#include "ImguiRenderable.h"
#include "ImguiManager.h"
#include <fstream>
#include <map>
#include <vector>

/*

ALERT
  The uiLayer need Media of UI or would crash any time.!!!
*/
class uiLayer
{
public:
	uiLayer();
	uiLayer(OgreBites::ApplicationContextBase *, Ogre::SceneManager*);
	~uiLayer();
	// imgui width of range
	int width;
	// imgui height of range
	int height;
	// wrap creating ImguiManager of Ogre
	void createImguiManager(OgreBites::ApplicationContextBase *,Ogre::SceneManager *);
	// wrap show the demo window of imgui
	void update_ShowDemoWindow(Ogre::Real);
	// loading the conversition, must be utf-8 coding
	void loadFromFile(const char * path);
	// delete all the content of conversition loading from the file
	void unload();
	// get next line of conversition
	std::string* nextFileContents();
	// set the trigger to true so we can move to next line of conversation
	void pullTrigger();
	// like slipt of python
	std::vector<std::string> split(const std::string &, const std::string &);
	void updateWindow(float);
	void InitCurContent();
	//place the UI charactor position by default
	void setUIpicPositionSet_default();
	//place the UI charactor position by user
	void setUIpicPositionSet(std::string, Ogre::Vector3&,Ogre::Vector3&);
	//set the conversition
	void setConversition(std::string);
	//control the charUI position
	void setCharUIPosition(std::string , Ogre::Vector3 &);
	//control the CharUI scale
	void setCharUIScale(std::string , Ogre::Vector3 &);
	//
	Ogre::SceneManager *mSceneMgr;
	//bool 
	bool conversationEnd = false;


protected:


	void setAllUIPicsUnVisble();
	void setUIPic(std::string id);

	std::vector<std::string *>		file_contents;
	std::vector<std::string *>::iterator		it_file_contents;

	bool						mTrigger = true;

	static std::string*			cur_file_contents;
	std::string					cur_title;
	std::string					cur_content;
	std::string					cur_id;
	std::string					delim;

	std::vector<std::string>	UIPicsId;
	std::map<std::string, Ogre::Vector3>	UIpicPositionSet;
	std::map<std::string, Ogre::Vector3>::iterator	it_UIpicPositionSet;
	std::map<std::string, Ogre::Vector3>	UIpicScaleSet;
	std::map<std::string, Ogre::Vector3>::iterator	it_UIpicScaleSet;

private:
	void __ShowCustomWindow(std::string&, std::string&, bool*);
};