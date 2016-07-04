#include "../Include/CSceneNode.h"
#include "../Include/CSpriteNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CMenuNode.h"
#include "../Include/CInfoNode.h"

#include <QDebug>
using namespace cocos2d;

namespace LM
{

CSceneNode::CSceneNode(std::string a_sID, bool a_bDebugMode) : 
	m_sID(a_sID),
	m_bIsSynced(false), 
	m_bDashboardTrigger(false),
	m_bDebugMode(a_bDebugMode)
{
}

Scene* CSceneNode::CreateScene()
{
  // autoreleased
  m_pScene = Scene::create();
  // autoreleased
  auto oLayer = CSceneNode::create();

  m_pScene->addChild(oLayer);

  return m_pScene;

}


cocos2d::Scene* CSceneNode::GetScene()
{
  return m_pScene;
}

bool CSceneNode::init()
{
  if (!cocos2d::Layer::init())
  {
    return false;
  }
//  TODO REMOVE
//  Size visibleSize = Director::getInstance()->getVisibleSize();
//  Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//
//  auto lfCallback = [](Ref* pSender) 
//  {
//	  Director::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	  exit(0);
//#endif
//
//  };
//  
//  CMenuNode* pMenuNode = new CMenuNode("CloseNormal.png",
//                                       "CloseSelected.png",
//                                       lfCallback,
//	                                   EAnchor::FLOAT,
//                                       10,
//                                       10);
//
//  AddChildNode(pMenuNode);
//  pMenuNode->Init();
//
//
//  CLabelNode* oLabel = new CLabelNode("Hello World", "fonts/Marker Felt.ttf", 24,
//	                EAnchor::FLOAT,
//                    origin.x + visibleSize.width/2,
//                    origin.y + visibleSize.height/2);
//  // add the label as a child to this layer
//  AddChildNode(oLabel);
//  oLabel->Init();
//
//  CSpriteNode* oSprite = new CSpriteNode("HelloWorld.png",
//	                  EAnchor::FLOAT,
//                      origin.x + visibleSize.width/2,
//                      origin.y + visibleSize.height/2);
//  // add the sprite as a child to this layer
//  AddChildNode(oSprite);
//  oSprite->Init();
  

  CCLOG("init scene : %s", m_sID.c_str());
  
  CNode::Init();

  if (m_bDebugMode && m_sID != "none")
	  DisplayDebugInfo();


  return true;

}



const std::string& CSceneNode::GetSceneID() const
{
	return m_sID;
}

void CSceneNode::SetSynced(bool a_bIsSynced)
{
	m_bIsSynced = a_bIsSynced;
}

bool CSceneNode::IsSynced()
{
	return m_bIsSynced;
}



void CSceneNode::DisplayDebugInfo()
{
	Label* pLabel = Label::createWithTTF(m_sID, "fonts/arial.ttf", 12);

	Vec2 oOrigin = Director::getInstance()->getVisibleOrigin();
	Size oVisibleSize = Director::getInstance()->getVisibleSize();

	pLabel->setAlignment(TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0, 1));
	pLabel->setPosition(Vec2(20 + oOrigin.x, oOrigin.y + oVisibleSize.height - 20));

	m_pScene->addChild(pLabel, 2);
}

void CSceneNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    rapidjson::Value newScene(rapidjson::kObjectType);
    newScene.AddMember("scene", rapidjson::Value(this->m_sID.c_str(), this->m_sID.length()), a_rAllocator);
    rapidjson::Value navigation(rapidjson::kArrayType);
    rapidjson::Value content(rapidjson::kArrayType);
    rapidjson::Value information(rapidjson::kArrayType);


    for(CNode* currentNode : this->m_vChildren)
    {
        // Special process (navigation and information)
        qDebug("cast en CMenuNode");
        CMenuNode* pMenuNode = dynamic_cast<CMenuNode*>(currentNode);
        if(pMenuNode)
        {
            pMenuNode->ToJson(navigation, a_rAllocator);
            continue;
        }
        qDebug("cast en CInfoNode");
        CInfoNode* pInfoNode = dynamic_cast<CInfoNode*>(currentNode);
        if(pInfoNode)
        {
            pInfoNode->ToJson(information, a_rAllocator);
            continue;
        }
        qDebug("Default comp");
        content.PushBack("content Node", a_rAllocator);
        // Else do default process (content)

    }

    newScene.AddMember("navigation", navigation, a_rAllocator);
    newScene.AddMember("content", content, a_rAllocator);
    newScene.AddMember("information", information, a_rAllocator);


    a_rParent.PushBack(newScene, a_rAllocator);
}

} // namespace LM
