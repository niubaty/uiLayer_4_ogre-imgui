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



void ImguiManager::createFontTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    if(io.Fonts->Fonts.empty())
        io.Fonts->AddFontDefault();
	
#ifdef USE_FREETYPE



=====>

void ImguiManager::createFontTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    if(io.Fonts->Fonts.empty())
        io.Fonts->AddFontDefault();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../Media/ui/fonts/youyuan.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());

#ifdef USE_FREETYPE