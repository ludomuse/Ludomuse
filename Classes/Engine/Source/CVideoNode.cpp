#include "../Include/CVideoNode.h"
#include "cocos2d.h"
#include "../Include/CMacroManager.h"

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



} // namaspace LM
