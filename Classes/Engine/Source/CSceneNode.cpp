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

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  // auto closeItem = MenuItemImage::create(
  //     "CloseNormal.png",
  //     "CloseSelected.png",
  //     CC_CALLBACK_1(CSceneNode::menuCloseCallback, this));
    
  // closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
  //                             origin.y + closeItem->getContentSize().height/2));

  // create menu, it's an autorelease object
  // auto menu = Menu::create(closeItem, NULL);
  // menu->setPosition(Vec2::ZERO);
  // this->addChild(menu, 1);

  
  CMenuNode* pMenuNode = new CMenuNode("CloseNormal.png",
                                       "CloseSelected.png",
                                       [](Ref* pSender){
                                         Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                                         exit(0);
#endif
                                       },
                                       10,
                                       10);

  AddChildNode(pMenuNode);
  pMenuNode->Init();

  /////////////////////////////
  // 3. add your codes below...

  // add a label shows "Hello World"
  // create and initialize a label
    
  // auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
  // position the label on the center of the screen
  // label->setPosition(Vec2(origin.x + visibleSize.width/2,
  //                         origin.y + visibleSize.height - label->getContentSize().height));

  CLabelNode* oLabel = new CLabelNode("Hello World", "fonts/Marker Felt.ttf", 24,
                    origin.x + visibleSize.width/2,
                    origin.y + visibleSize.height/2);
  // add the label as a child to this layer
  AddChildNode(oLabel);
  oLabel->Init();

  // add "HelloWorld" splash screen"
  // auto sprite = Sprite::create("HelloWorld.png");

  // position the sprite on the center of the screen
  // sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

  CSpriteNode* oSprite = new CSpriteNode("HelloWorld.png",
                      origin.x + visibleSize.width/2,
                      origin.y + visibleSize.height/2);
  // add the sprite as a child to this layer
  AddChildNode(oSprite);
  oSprite->Init();
  

  // TODO true solution, uncomment when TreeBuilder is OK
  /*DoForEachChildNode([](CNode* a_pChild) {
	  CEntityNode* pEntity = (CEntityNode*) a_pChild;
	  if (pEntity) 
	  {
		  pEntity->Init();
	  }
  });*/

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
