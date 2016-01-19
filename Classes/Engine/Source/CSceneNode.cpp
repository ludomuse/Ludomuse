#include "../Include/CSceneNode.h"
#include "../Include/CSpriteNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CMenuNode.h"

using namespace cocos2d;

namespace LM
{

CSceneNode::CSceneNode()
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
//
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
  

  
  for (CNode* pChildNode : m_vChildren){
	  CEntityNode* pEntity = (CEntityNode*) pChildNode;
		  if (pEntity) 
		  {
			  pEntity->Init();
		  }
  }

  return true;

}


void CSceneNode::menuCloseCallback(Ref* pSender)
{
  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}

} // namespace LM
