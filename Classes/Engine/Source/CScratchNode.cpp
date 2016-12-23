#include "../Include/CScratchNode.h"

using namespace cocos2d;

namespace LM
{

  CScratchNode::CScratchNode(const std::string& a_rSpriteFilename,
			     EAnchor a_eAnchor,
			     int a_iWidth,
			     int a_iHeight,
			     int a_iXPosition,
			     int a_iYPosition) :
    CSpriteNode(a_rSpriteFilename,
		a_eAnchor,
		a_iWidth,
		a_iHeight,
		a_iXPosition,
		a_iYPosition)
  {
  }


  void CScratchNode::Init()
  {
    
    
    m_pDrawNode = DrawNode::create();
    auto clippingNode = ClippingNode::create(m_pDrawNode);
    auto sprite = Sprite::create(m_sSpriteFilename);
    clippingNode->setAlphaThreshold(0.0f);
    clippingNode->addChild(sprite);
    m_pCocosEntity = sprite;

    PopulateParent(true, false);
    cocos2d::Scene* pScene = GetParentScene();
    if (pScene)
    {
        pScene->addChild(clippingNode, 0);
    }


    clippingNode->setContentSize(sprite->getBoundingBox().size);
    //clippingNode->setPosition(clippingNode->getPosition());

    CNode::Init();
  }


  void CScratchNode::DrawTouch(const Vec2& a_oTouchLocation)
  {
    m_pDrawNode->drawDot(a_oTouchLocation - Vec2(100, 100), 200, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
  }

} // namespace LM
