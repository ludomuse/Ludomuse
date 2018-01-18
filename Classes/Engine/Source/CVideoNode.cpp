#include "../Include/CVideoNode.h"
#include "cocos2d.h"
#include "../Include/CMacroManager.h"

#include <CProjectManager.h>

#ifdef __ANDROID__
#include "../../Modules/Networking/android/Include/LmJniJavaFacade.h"
#include "../../Modules/Networking/android/Include/LmJniCppFacade.h"
using namespace cocos2d::experimental::ui;
#endif



namespace LM
{

CVideoNode::CVideoNode(const std::string& a_rFilename,
                       EAnchor a_eAnchor,
                       int a_iWidth,
                       int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iWidth, a_iXPosition, a_iXPosition),
    m_sVideoFilename(a_rFilename)
{
  
}

void CVideoNode::Init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  //VideoPlayer* player = VideoPlayer::create();
  //player->setFileName(m_sVideoFilename);
  //m_pCocosEntity = player;
  //CCLOG("init VideoPlayer entity");
  //player->play();
	CCLOG("run video activity");
	LmJniJavaFacade::showVideo(m_sVideoFilename);
#endif
  //PopulateParent(false);
  //CNode::Init();
}



void CVideoNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    rapidjson::Value spriteNode(rapidjson::kObjectType);
    spriteNode.AddMember("type", "Video", a_rAllocator);
    if(!m_sID.empty())
    {
        spriteNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);

    std::string temp = m_sVideoFilename;
    std::string projectPath = CProjectManager::Instance()->GetProjectPath();
    int index = temp.find(projectPath);
    if(index != std::string::npos)
    {
        temp.erase(index, projectPath.length());
    }
    else
    {
        std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/templates/";
        int index2 = temp.find(templatePath);
        if(index2 != std::string::npos)
        {
            temp.erase(index2, templatePath.length());
        }
    }
    std::string* string = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("source", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);

    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

    if(!this->m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if(!this->m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    spriteNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(spriteNode, a_rAllocator);

}


} // namaspace LM
