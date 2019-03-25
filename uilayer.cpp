#include "imgui.h"
#include "uilayer.h"

uiLayer::uiLayer() {

}

uiLayer::uiLayer(OgreBites::ApplicationContextBase * app, Ogre::SceneManager* mgr) {
	createImguiManager(app, mgr);
}

void uiLayer::createImguiManager(OgreBites::ApplicationContextBase * app,Ogre::SceneManager* mgr) {
	Ogre::ImguiManager::createSingleton();
	app->addInputListener(Ogre::ImguiManager::getSingletonPtr());
	Ogre::ImguiManager::getSingleton().init(mgr);
	mSceneMgr = mgr;
	width = app->getRenderWindow()->getWidth();
	height = app->getRenderWindow()->getHeight();
	//这里临时添加到了ogre-imgui/ImguiManager.cpp/createFontTexture 以后找到原因要改掉
	//assert(_access("../Media/ui/fontsyouyuan.ttf", 0) != -1, "../Media/ui/fonts/youyuan.ttf not existing");
	//ImGui::GetIO().Fonts->AddFontFromFileTTF("../Media/ui/fonts/youyuan.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
}

void uiLayer::setConversition(std::string path) {
	loadFromFile(path.c_str());
	InitCurContent();
	setUIpicPositionSet_default();
}

void uiLayer::InitCurContent() {
	cur_title = std::string(u8" ");
	cur_content = std::string(u8" ");
	cur_id = std::string(u8"-1");
	delim = std::string(u8" ");
}


void uiLayer::update_ShowDemoWindow(Ogre::Real dt) {
	Ogre::ImguiManager::getSingleton().newFrame(
		dt,
		Ogre::Rect(0, 0, width, height)
	);
	ImGui::ShowDemoWindow();
}

void uiLayer::loadFromFile(const char * path) {
	std::ifstream infile;
	infile.open(path, std::ios::in);
	assert(infile.is_open());
	while (!infile.eof())
	{
		char buffer[512];
		infile.getline(buffer, 512);
		std::string *str = new std::string(buffer);
		file_contents.push_back(str);
	}

	it_file_contents = file_contents.begin();
}

void uiLayer::unload() {
	for (std::vector<std::string *>::iterator it = file_contents.begin();it != file_contents.end();it++)
		delete *it;
}

std::string* uiLayer::nextFileContents() {
	if (mTrigger) {
		if (it_file_contents != file_contents.end()) {
			cur_file_contents = *it_file_contents;
			it_file_contents++;
		}
		else {
			cur_file_contents = NULL;
		}
	}
	return cur_file_contents;
}

void uiLayer::pullTrigger() {
	mTrigger = true;
}

std::vector<std::string> uiLayer::split(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> res;
	if (str == "")
		return res;
	//在字符串末尾也加入分隔符，方便截取最后一段
	std::string strs = str + pattern;
	size_t pos = strs.find(pattern);

	while (pos != strs.npos)
	{
		std::string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(pattern);
	}

	return res;
}

void uiLayer::updateWindow(float dt) {

	Ogre::ImguiManager::getSingleton().newFrame(
		dt,
		Ogre::Rect(0, 0, width, height)
	);

	std::string* cur = nextFileContents();
	if (cur != NULL) {
		std::vector<std::string> strlist = split(*cur, delim);
		//cur_id or strlist[0] makes sure which material texture should select
		cur_id = strlist[0];
		cur_title = strlist[1];
		cur_content = strlist[2];
		setUIPic(cur_id);
	}
	else {
		//disable all UIpic
		//add this setAllUPicsUnvisble() here may set unvisalbe every frame and it's acceptable because it's near to the end
		setAllUIPicsUnVisble();
		//cur_title = std::string(u8" ");
		//cur_content = std::string(u8" ");
		conversationEnd = true;
	}
	//std::cout << cur_title << " " << cur_content << std::endl;
	__ShowCustomWindow(cur_title, cur_content, 0);

	mTrigger = false;
}

void uiLayer::setUIPic(std::string id) {
	//切记uiLayer是需要配套资源的
	if (mTrigger) {
		//disable all UIpic
		setAllUIPicsUnVisble();
		if (!mSceneMgr->hasSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(id))) {
			Ogre::Entity *ent = mSceneMgr->createEntity(Ogre::String(u8"uipicent") + Ogre::String(id), Ogre::SceneManager::PT_PLANE);
			std::map<Ogre::String, Ogre::Camera*> cams = mSceneMgr->getCameras();
			std::map<Ogre::String, Ogre::Camera*>::iterator it = cams.begin();
			Ogre::SceneNode *entNode = it->second->getParentSceneNode()->createChildSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(id));
			entNode->attachObject(ent);
			UIPicsId.push_back(id);
		}

		Ogre::SceneNode *nd = mSceneMgr->getSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(id));
		nd->setPosition(UIpicPositionSet.at(id));
		nd->setScale(UIpicScaleSet.at(id));
		nd->setVisible(true);
		Ogre::Entity *en = mSceneMgr->getEntity(Ogre::String(u8"uipicent") + Ogre::String(id));
		en->setMaterialName((std::string(u8"UI/char") + id).c_str());
	}

}

void uiLayer::setAllUIPicsUnVisble() {
	for (std::vector<std::string>::iterator it = UIPicsId.begin();it != UIPicsId.end();it++)
		mSceneMgr->getSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(*it))->setVisible(false);

}



void uiLayer::setUIpicPositionSet_default() {
	UIpicPositionSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"0"), Ogre::Vector3(-10, 0, -20)));
	UIpicPositionSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"1"), Ogre::Vector3(10, 0, -20)));
	UIpicPositionSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"2"), Ogre::Vector3(-5, 0, -20)));
	UIpicScaleSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"0"), Ogre::Vector3(0.02, 0.02, 0.02)));
	UIpicScaleSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"1"), Ogre::Vector3(0.03, 0.03, 0.03)));
	UIpicScaleSet.insert(std::pair<std::string, Ogre::Vector3>(std::string(u8"2"), Ogre::Vector3(0.02, 0.02, 0.02)));
}


void uiLayer::setUIpicPositionSet(std::string id,Ogre::Vector3 &pos,Ogre::Vector3 &scale) {
	UIpicPositionSet.at(id) = pos;
	UIpicScaleSet.at(id) = scale;
}



void uiLayer::setCharUIPosition(std::string id,Ogre::Vector3 &v) {
	Ogre::SceneNode *nd = mSceneMgr->getSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(id));
	nd->setPosition(v);
}

void uiLayer::setCharUIScale(std::string id, Ogre::Vector3 &v) {
	Ogre::SceneNode *nd = mSceneMgr->getSceneNode(Ogre::String(u8"uipicnd") + Ogre::String(id));
	nd->setScale(v);
}



std::string* uiLayer::cur_file_contents(NULL);





//private methode
void uiLayer::__ShowCustomWindow(std::string &title, std::string &content, bool* p_open)
{
	//window size
	static int corner = 3;
	int DISTANCE = 10;
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - DISTANCE * 2, ImGui::GetIO().DisplaySize.y / 4 - DISTANCE), 1);

	//font size
	ImGuiIO& io = ImGui::GetIO();
	(void)io;


	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin("Conversition", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	//ImGui::PushFont(atlas->Fonts[0]);
	ImGui::PushFont(io.Fonts->Fonts[1]);
	io.Fonts->Fonts[1]->Scale = 1;
	ImGui::Text("%s", title.c_str());
	ImGui::PopFont();

	ImGui::PushFont(io.Fonts->Fonts[1]);
	io.Fonts->Fonts[1]->Scale = 1;
	ImGui::TextWrapped("%s", content.c_str());
	ImGui::PopFont();


	// End of ShowDemoWindow()
	ImGui::End();

}

uiLayer::~uiLayer() {
	unload();
}